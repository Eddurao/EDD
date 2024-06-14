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
#include <cstdint>
#include "Huffman.hpp"


Huffman::Huffman()
{
	for (int i = 0; i < 128; i++) {
		array_nodos[i] = new Nodo_Huffman;
		array_nodos[i]->ch = i;
		array_nodos[i]->frec = 0;
	}
}


// Codificacion

void Huffman::codificar(const std::string& input, const std::string& output)
{
	input_file_name = input;
	output_file_name = output;
	_codificar();
}


void Huffman::_codificar()
{
	obtener_frecuencias();
	llenar_cola();
	construir_arbol();
	crear_codes(raiz, "");
	print_nodos();
	escribir_codificado();
}


void Huffman::obtener_frecuencias()
{
	input_file.open(input_file_name, std::ios::in);
	if (!input_file)
		return;

	char ch;
	while (input_file.get(ch)) {
		array_nodos[ch]->frec++;
	}
	input_file.close();
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
	if (nodo->left == nullptr && nodo->right == nullptr) {
		nodo->code = code;
		return;
	}
	crear_codes(nodo->left, code + '0');
	crear_codes(nodo->right, code + '1');
}

void Huffman::escribir_codes()
{
	// TODO
}

void Huffman::escribir_codificado()
{
	input_file.open(input_file_name, std::ios::in);
	if (!input_file)
		return;
	// std::ios::binary para que escriba EXACTAMENTE lo que le decimos
	output_file.open(output_file_name, std::ios::out | std::ios::binary);
	if (!output_file)
		return;

	/* Se deben escribir los codigos correspondientes a cada caracter del
	 * texto concatenados. Dado que la minima unidad que somos capaces de
	 * escribir es 1 byte, debemos agrupar codigos de 8 bits = 1 byte, para
	 * escribir.
	 *
	 * En buffer se acumulan los 1s y 0s, en str se arman los bytes a
	 * a escribir */
	std::string buffer = "", str = "";
	texto_codificado = "";
	char ch;
	while (input_file.get(ch)) {
		buffer += array_nodos[ch]->code;
		//std::cout << array_nodos[ch]->code << ' ';
		while (buffer.size() > 8) {
			// Formamos un byte
			texto_codificado += (char)str_binario_a_decimal(buffer.substr(0,8));
			/*std::cout << "Byte = ";
			std::cout << str_binario_a_decimal(buffer.substr(0,8));
			std::cout << '\n'; */

			// Removemos 8 bits del buffer
			buffer = buffer.substr(8);
		}
	}
	std::cout << '\n';
	// Es posible que queden < 8 bits, en el buffer, llenamos con 0s
	int bits_padding = 8 - buffer.size();
	if (buffer.size() < 8) {
		buffer.append(bits_padding, '0');
	}
	/*for (int i = 0; i < bits_padding; i++)
		std::cout << "0";
	std::cout << " Byte = ";
	std::cout << str_binario_a_decimal(buffer);
	std::cout << '\n'; */
	texto_codificado += (char)str_binario_a_decimal(buffer);
	texto_codificado += (char)bits_padding; // Necesario para la decodificacion
	//std::cout << "Codificacion:\n";
	//std::cout << str << "|final\n\n\n";

	output_file.write(texto_codificado.c_str(), texto_codificado.size());
	input_file.close();
	output_file.close();
}


// Decodificacion

void Huffman::decodificar(const std::string& input, const std::string& output)
{
	input_file_name = input;
	output_file_name = output;
	obtener_codigos();
	reconstruir_arbol();
	_decodificar();
}

void Huffman::obtener_codigos()
{
	// TODO
}

void Huffman::reconstruir_arbol()
{
	// TODO
}

void Huffman::_decodificar(bool leer_prefix)
{
	input_file.open(input_file_name, std::ios::in | std::ios::binary);
	if (!input_file)
		return;

	char padding_ceros;
	input_file.seekg(-1, std::ios::end);
	input_file.read(&padding_ceros, 1);
	std::streampos file_size = input_file.tellg();
	std::cout << "File size: " << file_size << "\nPadding ceros: " << (int)padding_ceros << std::endl;

	input_file.seekg(0, std::ios::beg);

	if (leer_prefix) {
		// TODO
	}
	
	//input_file.seekg(-1, std::ios::end);
	//char bits_padding;
	//input_file.get(bits_padding);
	//unsigned int file_size = input_file.tellg(); // Tamaño del archivo

	//input_file.seekg(0);	//TODO
	
	// Decodificacion del texto
	nodo_ptr current;
	char byte;
	unsigned int bits_restantes = 0;
	//while (!input_file.eof()) {
	for(size_t i = 0; i < file_size || bits_restantes > 0;) {
		current = raiz;
		while (!current->es_hoja()) {
			if (bits_restantes == 0) {
				input_file.read(reinterpret_cast<char*>(&byte), 1);
				//std::cout << std::hex << (int)byte;
				if (i < file_size - 2)
					bits_restantes = 8;
				// Para el penultimo byte, se ignoran los ceros de relleno
				else {
					bits_restantes = 8 - padding_ceros;
					i++; // Evitamos procesar el ultimo byte
				}
				i++;
			}

			if (byte & 0x80) { // true si 8vo bit es uno
				//std::cout << '1';
				current = current->right;
			} else {
				//std::cout <<'0';
				current = current->left;
			}
			byte = (byte << 1) & 0xFF;
			bits_restantes--;
		}
		std::cout << current->ch;
	}
	

}

void Huffman::escribir_decodificado()
{
	output_file.open(output_file_name, std::ios::out);
	if (!output_file)
		return;
	// TODO
}


// Auxiliares

int Huffman::str_binario_a_decimal(const std::string& binario)
{
	int sum = 0;
	for (size_t i = 0; i < binario.size(); i++) {
		// '0' = 48, '1' = 49
		sum = sum*2 + (binario[i] - '0');
	}
	return sum;
}

std::string Huffman::char_a_str_binario(char ch)
{
	// TODO
	return "";
}


// Debug

void Huffman::print_nodos()
{
	for (int i = 0; i < 128; i++) {
		if (array_nodos[i]->frec) {
			print_ch(array_nodos[i]->ch);
			std::cout << ": " << array_nodos[i]->code << std::endl;
			std::cout << std::endl;
		}
		if (!array_nodos[i]->es_hoja()) {
			print_ch(array_nodos[i]->ch);
			std::cout << " No es hoja!\n";
		}
	}
}

void Huffman::print_ch(char ch)
{
	std::cout << '\'';
	if (ch == ' ') {
		std::cout << '_';
	} else if (ch == '\n') {
		std::cout << "\\n";
	} else {
		std::cout << ch;
	}
	std::cout << '\'';
}

void Huffman::test_codificacion_decodificacion(const std::string& input)
{
	codificar(input, "codificado.txt");
	input_file_name = "codificado.txt";
	output_file_name = "decodificado.txt";
	_decodificar(false);
}
