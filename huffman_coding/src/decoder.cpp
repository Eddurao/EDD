#include <iostream>
#include <fstream>

#include <Huffman.hpp>
int main(int argc, char **argv) {
	if (argc < 3) {
		std::cerr << "Uso: decoder [input_file] [output_file]" << std::endl;
		return 1;
	}

	std::ifstream input_file;
	std::ofstream output_file;

	input_file.open(argv[1], std::ios::binary);
	if (!input_file.is_open())
	{
		std::cerr << "decoder error: No se pudo abrir el archivo.\n";
		return 2;
	}

	output_file.open(argv[2], std::ios::binary);
	if (!output_file.is_open())
	{
		std::cerr << "decoder error: No se puede escribir.\n";
		return 2;
	}
	
	HuffmanCode *coder = new HuffmanCode();
	bool success;
	success = coder->ExpandFile(input_file, output_file);
	if (!success) {
		std::cerr << "decoder error: Error en la decodificacion.\n";
		return 3;
	}

	return 0;
}
