#include <fstream>
#include <iostream>

struct Disk {
    std::string format;

    static Disk parse(std::ifstream &);
};

void partOne(const Disk &);
void partTwo(Disk &);
int main() {
    std::ifstream input("../input.txt");
    if (!input.is_open()) return -1;

    Disk disk = Disk::parse(input);
    input.close();

    partOne(disk);
    partTwo(disk);

    return 0;
}

void partOne(const Disk &disk) {
    std::cout << disk.format << std::endl;
}

void partTwo(Disk &disk) {

}

Disk Disk::parse(std::ifstream &input) {
    Disk disk;

    std::string line;
    while (std::getline(input, line)) {
        disk.format += line;
    }

    return disk;
}
