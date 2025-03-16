#include <iostream>
#include <filesystem>
#include <format>
#include <string>

namespace fs = std::filesystem;

int main() {
    std::string rootPath = "C:\\"; // Starting at C: drive
    int skippedCount = 0;

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

                    // Print using std::format for clean output
                    std::cout << std::format(
                        "Full Path: {}\nFilename: {}\nSize: {} bytes\n------------------------\n",
                        fullPath, filename, size
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