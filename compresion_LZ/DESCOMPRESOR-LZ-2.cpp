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
    ifstream compressedInputFile("compressed.bin", ios::binary);
    if (!compressedInputFile.is_open()) {
        cout << "Unable to open compressed file" << endl;
        return 1;
    }

    // Abrir archivo de salida en modo binario
    ofstream decompressedFile("output.txt", ios::binary);
    if (!decompressedFile.is_open()) {
        cout << "Unable to open output file" << endl;
        return 1;
    }

    // Medir el tiempo de descompresión
    auto start = high_resolution_clock::now();

    // Leer y descomprimir el archivo en partes
    vector<int> compressedData;
    int code;
    while (compressedInputFile.read(reinterpret_cast<char*>(&code), sizeof(code))) {
        compressedData.push_back(code);
    }
    string decompressedText = decompress(compressedData);
    decompressedFile.write(decompressedText.c_str(), decompressedText.size());

    // Cerrar archivos
    compressedInputFile.close();
    decompressedFile.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Decompression completed in " << duration.count() << " milliseconds." << endl;

    return 0;
}

