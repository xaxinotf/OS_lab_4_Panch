#include <unistd.h>
#include <iostream>
#include <new>

int main() {
    const char* data = new (std::nothrow) char[28]{"I love Polishuk and EPAM"};
    if (!data) {
        std::cerr << "Memory allocation failed." << std::endl;
        return 1;
    }

    std::cout << "pid: " << getpid() << "\nData address: " << static_cast<const void*>(data)
              << "\nData size: " << sizeof("I love Polishuk and EPAM\n") << std::endl;
    std::cin.get();

    delete[] data;
    return 0;
}
