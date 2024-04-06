#include <iostream>
#include <iomanip>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <vector>

// Usage: ./process_reader pid address size
int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <pid> <address> <size>" << std::endl;
        return -1;
    }

    std::string pid{argv[1]};
    auto start = std::stoull(argv[2], nullptr, 16);
    auto size = std::stoull(argv[3]);
    auto filename = "/proc/" + pid + "/mem";
    std::cout << "Filename: " << filename << "\nAddr is: " << (void*)start << "\nSize: " << size << std::endl;

    int fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error opening file: " << strerror(errno) << std::endl;
        return -2;
    }

    off_t res = lseek(fd, (off_t)start, SEEK_SET);
    if (res == (off_t)-1) {
        std::cerr << "Error seeking in file: " << strerror(errno) << std::endl;
        close(fd);
        return -3;
    }

    std::vector<char> buffer(size);
    ssize_t read_res = read(fd, buffer.data(), buffer.size());
    if (read_res == -1) {
        std::cerr << "Error reading file: " << strerror(errno) << std::endl;
        close(fd);
        return -4;
    }

    std::string output(buffer.begin(), buffer.begin() + read_res);
    std::cout << "Data is: " << std::quoted(output) << "\n";

    close(fd);
    return 0;
}
