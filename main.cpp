#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <cstring>
#include <cerrno>
#include <array>

int main() {
    const char* dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    std::array<const char*, 3> evval = {
            "RELEASED",
            "PRESSED ",
            "REPEATED"
    };

    int fd = open(dev, O_RDONLY);
    if (fd == -1) {
        std::cerr << "Cannot open " << dev << ": " << strerror(errno) << ".\n";
        return EXIT_FAILURE;
    }

    input_event ev;
    while (true) {
        ssize_t n = read(fd, &ev, sizeof(ev));
        if (n == (ssize_t)-1) {
            if (errno == EINTR) {
                continue; // Interrupted system call
            } else {
                std::cerr << "Read error: " << strerror(errno) << ".\n";
                break;
            }
        } else if (n != sizeof(ev)) {
            errno = EIO; // Input/output error
            std::cerr << "Mismatched read size: " << strerror(errno) << ".\n";
            break;
        }

        if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2) {
            std::cout << evval[ev.value] << " 0x" << std::hex << ev.code
                      << " (" << std::dec << ev.code << ")\n";
        }
    }

    close(fd);
    return EXIT_FAILURE;
}
