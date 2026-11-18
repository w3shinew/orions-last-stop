#include <chrono>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>
#include <string>
#include <system_error>

namespace fs = std::filesystem;

static std::string format_time(fs::file_time_type value) {
    const auto system_time = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        value - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
    const std::time_t time = std::chrono::system_clock::to_time_t(system_time);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif
    std::ostringstream out;
    out << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return out.str();
}

int main(int argc, char** argv) {
    std::cout << "Galactic2 — file metadata inspector\n";
    if (argc != 2) {
        std::cerr << "Usage: galactic2 <file-path>\n";
        return 2;
    }
    const fs::path path = fs::u8path(argv[1]);
    std::error_code ec;
    if (!fs::exists(path, ec) || ec) {
        std::cerr << "Error: path does not exist or cannot be accessed.\n";
        return 1;
    }
    if (!fs::is_regular_file(path, ec) || ec) {
        std::cerr << "Error: currently only regular files are supported.\n";
        return 1;
    }
    const auto size = fs::file_size(path, ec);
    if (ec) { std::cerr << "Error reading file size: " << ec.message() << '\n'; return 1; }
    const auto modified = fs::last_write_time(path, ec);
    if (ec) { std::cerr << "Error reading modification time: " << ec.message() << '\n'; return 1; }

    std::cout << "Path:        " << path.string() << '\n'
              << "Name:        " << path.filename().string() << '\n'
              << "Extension:   " << (path.has_extension() ? path.extension().string() : "(none)") << '\n'
              << "Size:        " << size << " bytes\n"
              << "Modified:    " << format_time(modified) << '\n';
#ifdef _WIN32
    const DWORD attrs = GetFileAttributesW(path.c_str());
    if (attrs != INVALID_FILE_ATTRIBUTES) {
        std::cout << "Attributes:  "
                  << ((attrs & FILE_ATTRIBUTE_READONLY) ? "Read-only " : "")
                  << ((attrs & FILE_ATTRIBUTE_HIDDEN) ? "Hidden " : "")
                  << ((attrs & FILE_ATTRIBUTE_SYSTEM) ? "System " : "")
                  << ((attrs & FILE_ATTRIBUTE_ARCHIVE) ? "Archive " : "")
                  << ((attrs & FILE_ATTRIBUTE_DIRECTORY) ? "Directory " : "")
                  << '\n';
    }
#endif
    return 0;
}
