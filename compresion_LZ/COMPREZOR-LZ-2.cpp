#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "LZ77Compression.h"

using namespace std;
using namespace std::chrono;

const size_t BUFFER_SIZE = 1024 * 1024 * 1024; // 1 MB buffer size

int main() {
    // Abrir archivo de entrada en modo binario
    ifstream inputFile("english.txt", ios::binary);
    if (!inputFile.is_open()) {
        cout << "Unable to open input file" << endl;
        return 1;
    }

    // Abrir archivo de salida en modo binario
    ofstream compressedFile("compressed.bin", ios::binary);
    if (!compressedFile.is_open()) {
        cout << "Unable to open output file" << endl;
        return 1;
    }

    // Medir el tiempo de compresión
    auto start = high_resolution_clock::now();

    // Leer y comprimir el archivo en partes
    string buffer;
    buffer.resize(BUFFER_SIZE);
    while (inputFile.read(&buffer[0], BUFFER_SIZE) || inputFile.gcount()) {
        buffer.resize(inputFile.gcount());
        vector<int> compressedData = compress(buffer);
        for (int code : compressedData) {
            compressedFile.write(reinterpret_cast<const char*>(&code), sizeof(code));
        }
    }

    // Cerrar archivos
    inputFile.close();
    compressedFile.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Compression completed in " << duration.count() << " milliseconds." << endl;

    return 0;
}

