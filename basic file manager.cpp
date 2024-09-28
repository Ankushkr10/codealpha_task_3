#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

namespace fs = std::filesystem;

class FileManager {
public:
    void run() {
        std::string command;
        std::string current_path = fs::current_path().string();

        while (true) {
            std::cout << "Current Directory: " << current_path << "\n";
            std::cout << "> ";
            std::getline(std::cin, command);

            if (command == "exit") {
                break;
            } else if (command == "ls") {
                listFiles(current_path);
            } else if (command.substr(0, 3) == "cd ") {
                current_path = changeDirectory(command.substr(3), current_path);
            } else if (command.substr(0, 6) == "mkdir ") {
                createDirectory(command.substr(6), current_path);
            } else if (command.substr(0, 4) == "cp ") {
                copyFile(command.substr(3), current_path);
            } else if (command.substr(0, 3) == "mv ") {
                moveFile(command.substr(3), current_path);
            } else {
                std::cout << "Unknown command.\n";
            }
        }
    }

private:
    void listFiles(const std::string& path) {
        for (const auto& entry : fs::directory_iterator(path)) {
            std::cout << entry.path().filename().string() << "\n";
        }
    }

    std::string changeDirectory(const std::string& dir, const std::string& current_path) {
        fs::path new_path = fs::path(current_path) / dir;
        if (fs::exists(new_path) && fs::is_directory(new_path)) {
            return new_path.string();
        } else {
            std::cout << "Directory does not exist.\n";
            return current_path;
        }
    }

    void createDirectory(const std::string& dir, const std::string& current_path) {
        fs::path new_dir = fs::path(current_path) / dir;
        if (fs::create_directory(new_dir)) {
            std::cout << "Directory created: " << new_dir.string() << "\n";
        } else {
            std::cout << "Failed to create directory or it already exists.\n";
        }
    }

    void copyFile(const std::string& file_info, const std::string& current_path) {
        size_t pos = file_info.find(" ");
        if (pos == std::string::npos) {
            std::cout << "Usage: cp <source> <destination>\n";
            return;
        }
        std::string src = file_info.substr(0, pos);
        std::string dest = file_info.substr(pos + 1);

        fs::path src_path = fs::path(current_path) / src;
        fs::path dest_path = fs::path(current_path) / dest;

        if (fs::exists(src_path)) {
            fs::copy(src_path, dest_path);
            std::cout << "Copied " << src << " to " << dest << "\n";
        } else {
            std::cout << "Source file does not exist.\n";
        }
    }

    void moveFile(const std::string& file_info, const std::string& current_path) {
        size_t pos = file_info.find(" ");
        if (pos == std::string::npos) {
            std::cout << "Usage: mv <source> <destination>\n";
            return;
        }
        std::string src = file_info.substr(0, pos);
        std::string dest = file_info.substr(pos + 1);

        fs::path src_path = fs::path(current_path) / src;
        fs::path dest_path = fs::path(current_path) / dest;

        if (fs::exists(src_path)) {
            fs::rename(src_path, dest_path);
            std::cout << "Moved " << src << " to " << dest << "\n";
        } else {
            std::cout << "Source file does not exist.\n";
        }
    }
};

int main() {
    FileManager fileManager;
    fileManager.run();
}
