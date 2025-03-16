#include <iostream>
#include <filesystem>
#include <format>
#include <string>
#include <chrono>


// Suppress MSVC warning for gmtime
//#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
//#endif


#ifndef _WIN32
#include <unistd.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#endif

#include <windows.h> // For creation time on Windows. Note this has to come AFTER Winsock2.h

namespace fs = std::filesystem;

// Convert FILETIME to a readable string
std::string fileTimeToString(const FILETIME& ft) {
    SYSTEMTIME st;
    FileTimeToSystemTime(&ft, &st);
    return std::format("{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

// Get creation time using Windows API
std::string getCreationTime(const fs::path& path) {
    HANDLE hFile = CreateFileA(path.string().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return "Access Denied";
    }
    FILETIME ftCreate;
    if (!GetFileTime(hFile, &ftCreate, NULL, NULL)) {
        CloseHandle(hFile);
        return "Unknown";
    }
    CloseHandle(hFile);
    return fileTimeToString(ftCreate);
}

//// Convert fs::file_time_type to a readable string unsafe!
//std::string fsTimeToString(const fs::file_time_type& ft) {
//    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
//        ft - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
//    std::time_t tt = std::chrono::system_clock::to_time_t(sctp);
//    std::tm* gmt = std::gmtime(&tt);
//    return std::format("{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}",
//        gmt->tm_year + 1900, gmt->tm_mon + 1, gmt->tm_mday,
//        gmt->tm_hour, gmt->tm_min, gmt->tm_sec);
//}

std::string fsTimeToString(const fs::file_time_type& ft) {
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ft - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
    std::time_t tt = std::chrono::system_clock::to_time_t(sctp);
    std::tm gmt;
    if (gmtime_s(&gmt, &tt) != 0) {
        // Handle error
        return "Unknown";
    }
    return std::format("{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}",
        gmt.tm_year + 1900, gmt.tm_mon + 1, gmt.tm_mday,
        gmt.tm_hour, gmt.tm_min, gmt.tm_sec);
}

// Get the hostname in a portable way
std::string getHostname() {
#ifdef _WIN32
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);
    if (GetComputerNameA(computerName, &size)) {
        return std::string(computerName);
    }
    return "Unknown Host (Windows)";
#else
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return std::string(hostname);
    }
    return "Unknown Host (POSIX)";
#endif
}

// Get the first non-loopback IPv4 address, returns a std::string
std::string getIPAddress_old() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return "IP Unknown (WSAStartup failed)";
    }

    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        WSACleanup();
        return "IP Unknown (gethostname failed)";
    }

    struct addrinfo hints = { 0 }, * res = nullptr;
    hints.ai_family = AF_INET; // IPv4 only
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, nullptr, &hints, &res) != 0) {
        WSACleanup();
        return "IP Unknown (getaddrinfo failed)";
    }

    char ipStr[INET_ADDRSTRLEN];
    for (struct addrinfo* p = res; p != nullptr; p = p->ai_next) {
        struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
        if (inet_ntop(AF_INET, &(ipv4->sin_addr), ipStr, sizeof(ipStr)) != nullptr) {
            std::string ip(ipStr);
            if (ip != "127.0.0.1") { // Skip loopback
                freeaddrinfo(res);
                WSACleanup();
                return ip;
            }
        }
    }
    freeaddrinfo(res);
    WSACleanup();
    return "IP Unknown (no non-loopback address)";
#else
    struct ifaddrs* ifaddr, * ifa;
    if (getifaddrs(&ifaddr) == -1) {
        return "IP Unknown (getifaddrs failed)";
    }

    char ipStr[INET_ADDRSTRLEN];
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr || ifa->ifa_addr->sa_family != AF_INET) {
            continue; // Skip non-IPv4
        }
        struct sockaddr_in* sa = (struct sockaddr_in*)ifa->ifa_addr;
        if (inet_ntop(AF_INET, &(sa->sin_addr), ipStr, sizeof(ipStr)) != nullptr) {
            std::string ip(ipStr);
            if (ip != "127.0.0.1") { // Skip loopback
                freeifaddrs(ifaddr);
                return ip;
            }
        }
    }
    freeifaddrs(ifaddr);
    return "IP Unknown (no non-loopback address)";
#endif
}


// Check if an IP has Internet connectivity by attempting a connection to 8.8.8.8
bool hasInternetConnectivity(const std::string& ip) {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) return false;

#ifdef _WIN32
    // Set non-blocking mode on Windows
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);
#else
    // Set non-blocking mode on POSIX
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(53); // DNS port
    inet_pton(AF_INET, "8.8.8.8", &server.sin_addr); // Google DNS

    // Attempt to connect
    int result = connect(sock, (struct sockaddr*)&server, sizeof(server));
    bool connected = false;

    if (result == -1) {
#ifdef _WIN32
        if (WSAGetLastError() == WSAEWOULDBLOCK) {
#else
        if (errno == EINPROGRESS) {
#endif
            // Connection in progress, use select to check
            fd_set writefds;
            FD_ZERO(&writefds);
            FD_SET(sock, &writefds);
            struct timeval tv = { 2, 0 }; // 2-second timeout
            if (select(sock + 1, nullptr, &writefds, nullptr, &tv) > 0) {
                int so_error;
                socklen_t len = sizeof(so_error);
                getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&so_error, &len);
                connected = (so_error == 0);
            }
        }
        }
    else {
        connected = true; // Immediate success (rare in non-blocking)
    }

#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    return connected;
    }



// Get the first non-loopback IPv4 address with Internet connectivity
std::string getIPAddress() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return "IP Unknown (WSAStartup failed)";
    }

    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        WSACleanup();
        return "IP Unknown (gethostname failed)";
    }

    struct addrinfo hints = { 0 }, * res = nullptr;
    hints.ai_family = AF_INET; // IPv4 only
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, nullptr, &hints, &res) != 0) {
        WSACleanup();
        return "IP Unknown (getaddrinfo failed)";
    }

    char ipStr[INET_ADDRSTRLEN];
    for (struct addrinfo* p = res; p != nullptr; p = p->ai_next) {
        struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
        if (inet_ntop(AF_INET, &(ipv4->sin_addr), ipStr, sizeof(ipStr)) != nullptr) {
            std::string ip(ipStr);
            if (ip != "127.0.0.1" && hasInternetConnectivity(ip)) { // Check connectivity
                freeaddrinfo(res);
                WSACleanup();
                return ip;
            }
        }
    }
    freeaddrinfo(res);
    WSACleanup();
    return "IP Unknown (no Internet-connected address)";
#else
    struct ifaddrs* ifaddr, * ifa;
    if (getifaddrs(&ifaddr) == -1) {
        return "IP Unknown (getifaddrs failed)";
    }

    char ipStr[INET_ADDRSTRLEN];
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr || ifa->ifa_addr->sa_family != AF_INET) {
            continue; // Skip non-IPv4
        }
        struct sockaddr_in* sa = (struct sockaddr_in*)ifa->ifa_addr;
        if (inet_ntop(AF_INET, &(sa->sin_addr), ipStr, sizeof(ipStr)) != nullptr) {
            std::string ip(ipStr);
            if (ip != "127.0.0.1" && hasInternetConnectivity(ip)) { // Check connectivity
                freeifaddrs(ifaddr);
                return ip;
            }
        }
    }
    freeifaddrs(ifaddr);
    return "IP Unknown (no Internet-connected address)";
#endif
}



int main() {
    std::string rootPath = "C:\\"; // Starting at C: drive
    int skippedCount = 0;

	std::cout << "Hostname: " << getHostname() << "\n"; // Call to getHostname to suppress unused function warning
    std::cout << std::format("IP Address: {}\n", getIPAddress());
    std::cout << "Scanning drive " << rootPath << "...\n";

    try {
        // Recursively scan all files, skipping permission-denied items
        for (const auto& entry : fs::recursive_directory_iterator(rootPath, fs::directory_options::skip_permission_denied)) {
            try {
                if (entry.is_regular_file()) {
                    fs::path path = entry.path();
                    std::string fullPath = path.string();
                    std::string filename = path.filename().string();
                    uint64_t size = fs::file_size(path);
                    std::string creationTime = getCreationTime(path); // Windows-specific creation time
                    std::string lastModifiedTime = fsTimeToString(fs::last_write_time(path)); // Last modified time

                    // Print using std::format with creation and modified times
                    std::cout << std::format(
                        "Full Path: {}\nFilename: {}\nSize: {} bytes\n"
                        "Created: {}\nLast Modified: {}\n------------------------\n",
                        fullPath, filename, size, creationTime, lastModifiedTime
                    );
                }
            }
            catch (const fs::filesystem_error& e) {
                // Skip inaccessible files and log
                std::cerr << std::format("Skipped (access denied or error): {} - {}\n", entry.path().string(), e.what());
                skippedCount++;
            }
            catch (const std::exception& e) {
                // Catch other unexpected errors
                std::cerr << std::format("Skipped (unexpected error): {} - {}\n", entry.path().string(), e.what());
                skippedCount++;
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << std::format("Error scanning directory: {}\n", e.what());
    }

    std::cout << std::format("\nScan complete. Skipped {} items due to permissions or errors.\n", skippedCount);
    return 0;
}