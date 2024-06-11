#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <fstream>
#include <queue>
#include <vector>
#include <iostream>

struct Nodo_Huffman {
	char ch;
	int frec;
	std::string code;

	Nodo_Huffman* left;
	Nodo_Huffman* right;

	Nodo_Huffman()
	{
		left = right = nullptr;
	};
};
typedef Nodo_Huffman* nodo_ptr;

class Huffman {
private:
	nodo_ptr array_nodos[128];	// Para los 128 caracteres de la tabla ASCII
	std::string input_file_name; //, output_file_name;
	std::ifstream input_file;
	//ofstream output_file;
	
	nodo_ptr raiz;

public:
	class comparar {
	public:
		bool operator()(const nodo_ptr& n1, const nodo_ptr& n2)
		{
			return n1->frec > n2->frec;	
		}
	};
	// La cola guardará los nodos, considerando priorizando las menores frecuencias.
	std::priority_queue<nodo_ptr, std::vector<nodo_ptr>, comparar> cola;


	void obtener_frecuencias();
	void llenar_cola();
	void construir_arbol();
	void crear_codes(nodo_ptr nodo, std::string code);

	// debug
	void print_nodos();
	void print_ch(char ch);

public:
	Huffman();
	void codificar(const std::string& input_file_name);
	void decodificar(const std::string& input_file_name);

};

#endif /* HUFFMAN_HPP */
