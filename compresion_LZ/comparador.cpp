#include <iostream>
#include <fstream>

void compareFiles(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1);
    std::ifstream f2(file2);

    if (!f1.is_open() || !f2.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return;
    }

    char char1, char2;
    int position = 0;

    while (f1.get(char1) && f2.get(char2)) {
        if (char1 != char2) {
            std::cout << "Difference at position " << position << ":\n";
            std::cout << "File 1: '" << char1 << "' (ASCII: " << static_cast<int>(char1) << ")\n";
            std::cout << "File 2: '" << char2 << "' (ASCII: " << static_cast<int>(char2) << ")\n";
            return;
        }
        ++position;
    }

    // Check if one file is longer than the other
    if (f1.get(char1)) {
        std::cout << "File 1 is longer than File 2. Next character in File 1: '"
                  << char1 << "' (ASCII: " << static_cast<int>(char1) << ")\n";
    } else if (f2.get(char2)) {
        std::cout << "File 2 is longer than File 1. Next character in File 2: '"
                  << char2 << "' (ASCII: " << static_cast<int>(char2) << ")\n";
    } else {
        std::cout << "Files are identical.\n";
    }

    f1.close();
    f2.close();
}

int main() {
    std::string file1, file2;
    std::cout << "Enter the path of the first file: ";
    std::cin >> file1;
    std::cout << "Enter the path of the second file: ";
    std::cin >> file2;

    compareFiles(file1, file2);

    return 0;
}

