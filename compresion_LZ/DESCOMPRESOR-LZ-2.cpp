#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "LZ77Compression.h"

using namespace std;
using namespace std::chrono;

const size_t BUFFER_SIZE = 1024 * 1024 * 1024; // 1 MB buffer size

int main(int argc, char **argv) {
    if (argc < 3) {
	std::cerr << "Uso: descompresor [input_file] [output_file] (opcional)[repeticiones]" << std::endl;
    }

    ifstream compressedInputFile;
    ofstream decompressedFile;
    int repeticiones;

    long double duracion_total = 0;
    long double duracion_promedio;

    // Si no se especifica el numero de repeticiones, se codifica una vez.
    if (argc == 4) {
        repeticiones = atoi(argv[3]);
    } else
	repeticiones = 1;

    // Abrimos el archivo original
    compressedInputFile.open(argv[1], std::ios::binary);
    if (!compressedInputFile.is_open()) {
	std::cerr << "compresor error: No se pudo abrir el archivo.\n";
	return 2;
    }

    // Abrimos el archivo de salida en modo binario
    decompressedFile.open(argv[2], std::ios::binary);
    if (!decompressedFile.is_open()) {
	std::cerr << "compresor error: No se puede escribir.\n";
	return 2;
    }

    for (int i = 0; i < repeticiones; i++) {
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

    	auto stop = high_resolution_clock::now();
    	duracion_total += duration_cast<milliseconds>(stop - start).count();

	// Reseteamos los archivos
	compressedInputFile.clear();
	compressedInputFile.seekg(0, std::ios::beg);
	decompressedFile.clear();
	decompressedFile.seekp(0);
	
	compressedData.clear();
    }

    // Cerrar archivos
    compressedInputFile.close();
    decompressedFile.close();

    // Printeamos el tiempo promedio en milisegundos
    duracion_promedio = duracion_total / repeticiones;
    cout << duracion_promedio;

    return 0;
}

