#include <iostream>
#include <fstream>
#include <chrono>

#include "Huffman.hpp"

using namespace std::chrono;

int main(int argc, char **argv) {
	if (argc < 3) {
		std::cerr << "Uso: encoder [input_file] [output_file] (opcional)[repeticiones]" << std::endl;
		return 1;
	}
	
	std::ifstream input_file;
	std::ofstream output_file;
	int repeticiones;

	long double duracion_total = 0;
	long double duracion_promedio;


	// Si no se especifica el numero de repeticiones, se codifica una vez.
	if (argc == 4) {
		repeticiones = atoi(argv[3]);
	} else
		repeticiones = 1;

	// Abrimos el archivo original
	input_file.open(argv[1], std::ios::binary);
	if (!input_file.is_open())
	{
		std::cerr << "encoder error: No se pudo abrir el archivo.\n";
		return 2;
	}

	// Abrimos el archivo de salida en modo binario
	output_file.open(argv[2], std::ios::binary);
	if (!output_file.is_open())
	{
		std::cerr << "encoder error: No se puede escribir.\n";
		return 2;
	}
	
	// Se inicializa el codificador
	HuffmanCode *coder = new HuffmanCode();
	bool success;

	for (int i = 0; i < repeticiones; i++) {
		auto start = high_resolution_clock::now();
		success = coder->codificar(input_file, output_file);
		if (!success) {
			std::cerr << "encoder error: Error en la codificacion.\n";
			return 3;
		}
		auto stop = high_resolution_clock::now();
		duracion_total += duration_cast<milliseconds>(stop - start).count();
	
		// Reseteamos todo
		input_file.clear();
		input_file.seekg(0, std::ios::beg);
		output_file.clear();
		output_file.seekp(0);
		delete coder;
		coder = new HuffmanCode();

	}
	
	duracion_promedio = duracion_total / repeticiones;
	std::cout << duracion_promedio;

	return 0;
}
