#include "Huffman.hpp"

int main(int argc, char** argv)
{
	if (argc != 2) {
		return 1;
	}
	std::string file = argv[1];
	Huffman* coder = new Huffman();
	coder->test_codificacion_decodificacion(file);
}
