#include <iostream>
#include <filesystem>
#include <format>
#include <string>
#include <chrono>
#include <windows.h> // For creation time on Windows

// Suppress MSVC warning for gmtime
//#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
//#endif

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
int main() {
    std::string rootPath = "C:\\"; // Starting at C: drive
    int skippedCount = 0;

	std::cout << "Hostname: " << getHostname() << "\n"; // Call to getHostname to suppress unused function warning

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