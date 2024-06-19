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
	std::cerr << "Uso: compresor [input_file] [output_file] (opcional)[repeticiones]" << std::endl;
    }
    
    ifstream inputFile;
    ofstream compressedFile;
    int repeticiones;

    long double duracion_total = 0;
    long double duracion_promedio;

    // Si no se especifica el numero de repeticiones, se codifica una vez.
    if (argc == 4) {
        repeticiones = atoi(argv[3]);
    } else
	repeticiones = 1;

    // Abrimos el archivo original
    inputFile.open(argv[1], std::ios::binary);
    if (!inputFile.is_open()) {
	std::cerr << "compresor error: No se pudo abrir el archivo.\n";
	return 2;
    }

    // Abrimos el archivo de salida en modo binario
    compressedFile.open(argv[2], std::ios::binary);
    if (!compressedFile.is_open()) {
	std::cerr << "compresor error: No se puede escribir.\n";
	return 2;
    }

    for (int i = 0; i < repeticiones; i++) {
        // Medir el tiempo de compresión
	auto start = high_resolution_clock::now();
	
	// Leer y comprimir el archivo en partes
	string buffer = "";
	buffer.resize(BUFFER_SIZE);
	while (inputFile.read(&buffer[0], BUFFER_SIZE) || inputFile.gcount()) {
	    buffer.resize(inputFile.gcount());
	    vector<int> compressedData = compress(buffer);
	    for (int code : compressedData) {
		compressedFile.write(reinterpret_cast<const char*>(&code), sizeof(code));
	    }
	}
    	auto stop = high_resolution_clock::now();
    	duracion_total += duration_cast<milliseconds>(stop - start).count();
	
	// Reseteamos los archivos
	inputFile.clear();
	inputFile.seekg(0, std::ios::beg);
	compressedFile.clear();
	compressedFile.seekp(0);
    }
    // Cerrar archivos
    inputFile.close();
    compressedFile.close();

    // Printeamos el tiempo promedio en milisegundos
    duracion_promedio = duracion_total / repeticiones;
    cout << duracion_promedio;

    return 0;
}

