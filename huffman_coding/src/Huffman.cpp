/*
 *  Visualizacion del arbol:
 *       R
 *     /   \
 *    o     o
 *   / \    / \   
 *  6   5  4   o
 *            /  \
 *           3    o
 *               / \
 *              2   1
 */
#include "Huffman.hpp"

Huffman::Huffman()
{
	for (int i = 0; i < 128; i++) {
		array_nodos[i] = new Nodo_Huffman;
		array_nodos[i]->ch = i;
		array_nodos[i]->frec = 0;
	}
}

void Huffman::codificar(const std::string& input)
{
	input_file_name = input;
	obtener_frecuencias();
	llenar_cola();
	construir_arbol();
	crear_codes(raiz, "");
	//escribir_cola();
	print_nodos();
}

void Huffman::decodificar(const std::string& input_file_name)
{
	return;
}

void Huffman::obtener_frecuencias()
{
	input_file.open(input_file_name);
	char ch;
	while (input_file.get(ch)) {
		std::cout << ch;
		array_nodos[ch]->frec++;
	}
}

void Huffman::llenar_cola()
{
	for (int i = 0; i < 128; i++) {
		if(array_nodos[i]->frec > 0)
			cola.push(array_nodos[i]);
	}
}

void Huffman::construir_arbol()
{
	nodo_ptr nodo1, nodo2, tmp;
	std::priority_queue<nodo_ptr, std::vector<nodo_ptr>, comparar> temp(cola);
	while (temp.size() > 1) {
		nodo1 = temp.top(); temp.pop();
		nodo2 = temp.top(); temp.pop();

		tmp = new Nodo_Huffman;
		tmp->frec = nodo1->frec + nodo2->frec;
		// Se ubican las menores frecuencias a la derecha
		// (Ver visualización del arbol)
		tmp->right = nodo1;
		tmp->left = nodo2;

		temp.push(tmp);
	}
	raiz = temp.top(); temp.pop();
}

void Huffman::crear_codes(nodo_ptr nodo, std::string code)
{
	std::cout << code << std::endl;
	if (nodo->left == nullptr && nodo->right == nullptr) {
		nodo->code = code;
		return;
	}
	crear_codes(nodo->left, code + '0');
	crear_codes(nodo->right, code + '1');
}

void Huffman::print_nodos()
{
	for (int i = 0; i < 128; i++) {
		if (array_nodos[i]->frec) {
			print_ch(array_nodos[i]->ch);
			std::cout << ": " << array_nodos[i]->code << std::endl;
			std::cout << std::endl;
		}
	}
}

void Huffman::print_ch(char ch)
{
	std::cout << '\'';
	if (ch == ' ') {
		std::cout << '_';
	} else if (ch == '\n') {
		std::cout << "";
	} else {
		std::cout << ch;
	}
	std::cout << '\'';
}
