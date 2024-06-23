#include "Huffman.hpp"

#ifdef ORIGINAL
HuffmanCode::HuffmanCode() : alphabetcount(0), totalcharacters(0) {};
#else
HuffmanCode::HuffmanCode() : alphabetcount(0), totalcharacters(0)
{
	for (size_t i = 0; i < 128 ; i++) {
		symbolarray[i] = new TreeNode(i, 0, false);
	}
}
#endif

bool HuffmanCode::codificar(std::ifstream& fin, std::ofstream& fout)
{
	bool result = false;
	if (MapSymbols(fin))
	{
		GrowHuffmanTree();
		WriteCompressedFileHeader(fout);
		WriteCompressedFile(fin, fout);
		result = true;
	}
	return result;
};

// Build a table of symbols in the file and the number of occurrances (weight)
uintmax_t HuffmanCode::MapSymbols(std::ifstream& fin)
{
	totalcharacters = 0;
	char ch;
	while (fin.get(ch))
	{
		MapSymbol(ch);       // build map of symbols and weight
	}
	// reset stream so that we can read it again
	fin.clear();
	fin.seekg(0);
	return totalcharacters;
};

// Build Huffman Tree
bool HuffmanCode::GrowHuffmanTree()
{
	//   struct TreeNode* leftpointer, * rightpointer;
	//   priority_queue <TreeNode*, vector<TreeNode*>, compare> treeheap;

	// For each character create a leaf node and insert each leaf node in the heap.
	// Traverse the map
#ifdef ORIGINAL
	for (const auto& it : symbolmap) {
		treeheap.push(new TreeNode(it.first, it.second, true));
	}
#else
	for (size_t i = 0; i < 128; i++) {
		if (symbolarray[i]->weight) {
			treeheap.push(symbolarray[i]);
		}
	}
#endif

	// Iterate while size of min heap doesn't become 1
	while (treeheap.size() != 1)
	{
		//Extract two nodes from the heap.
		leftpointer = treeheap.top();
		treeheap.pop();

		rightpointer = treeheap.top();
		treeheap.pop();

		// Create a new internal node with weight equal to the sum of two extracted nodes.
		// By default 'leaf' will be false marking it as an internal node.
		// Assign symbol '\0' to this node and make the two extracted nodes as the left and right children. 
		// Add this node to the heap. 
		TreeNode* tmp = new TreeNode('\0', leftpointer->weight + rightpointer->weight);
		tmp->leftpointer = leftpointer;
		tmp->rightpointer = rightpointer;
		treeheap.push(tmp);
	}
	MakeCodesFromTree();
	SortCodeTable();

	return true;
}

// Write file tag, active symbol count, total character count and contents of charactermap to file
void HuffmanCode::WriteCompressedFileHeader(std::ofstream& fout)
{
	// Write the header verification tag and the character counts
#ifdef ORIGINAL
	fout.write((const char*)&filetag, sizeof(filetag));
#endif
	fout.write((const char*)&alphabetcount, sizeof(alphabetcount));
	fout.write((const char*)&totalcharacters, sizeof(totalcharacters));

	// Write the symbol and weights map to the file
	// This data is used to rebuild the Huffman tree for the decode

#ifdef ORIGINAL
	// Iterate over the map until end.
	for (const auto& it : symbolmap) {
		// Accessing KEY (symbol) from element pointed by it.
		char symbol = it.first;
		fout.write((const char*)&symbol, sizeof(symbol));
		// Accessing VALUE (weight) from element pointed by it.
		int weight = it.second;
		fout.write((const char*)&weight, sizeof(weight));
	}
#else
	for (size_t i = 0; i < 128; i++) {
		if (symbolarray[i]->weight) {
			char symbol = symbolarray[i]->symbol;
			fout.write((const char*)&symbol, sizeof(symbol));

			int weight = symbolarray[i]->weight;
			fout.write((const char*)&weight, sizeof(weight));
		}
	}
#endif
};

void HuffmanCode::WriteCompressedFile(std::ifstream& fin, std::ofstream& fout)
{
	string outbyte = "";
	char symbol;
	fin.clear();
	fin.seekg(0);
	while (fin)
	{
		// get character from infile
		fin.get(symbol);
#ifdef ORIGINAL
		// look for character match in tuple
		auto it = std::find_if(codetable.begin(), codetable.end(), [&symbol](const std::tuple<unsigned char, int, std::string>& e)
			{
				return std::get<0>(e) == symbol;
			});

		if (it != codetable.end())
		{
			outbyte += std::get<2>(*it);
			if (outbyte.length() >= 8)
			{
				std::bitset<8> b1(outbyte.substr(0, 8));
				outbyte.erase(0, 8);
				fout.write((const char*)&b1, 1);
			}
		}
#else
	//	if (symbolarray[symbol]->weight) {
			outbyte += symbolarray[symbol]->code;
			if (outbyte.length() >= 8)
			{
				std::bitset<8> b1(outbyte.substr(0, 8));
				outbyte.erase(0, 8);
				fout.write((const char*)&b1, 1);
			}

	//	}
#endif
	}
};

bool HuffmanCode::decodificar(std::ifstream& fin, std::ofstream& fout)
{
	if (!ReadCompressedFileHeader(fin))
	{
		return false;
	};
	GrowHuffmanTree();
	ReadCompressedFile(fin, fout);
	return true;
}

// Recover count numbers from front of file
bool HuffmanCode::ReadCompressedFileHeader(std::ifstream& fin)
{
	int count = 0;
	alphabetcount = 0;
	totalcharacters = 0;
#ifdef ORIGINAL
	char temptag[sizeof(filetag)]{};
	// Read verification tag
	fin.read(const_cast<char*>(temptag), sizeof(filetag));
	if (string(temptag) != string(filetag))
	{
		fin.close();
		return false;
	}
#endif
	// Read the counters from the file into variables
	fin.read(reinterpret_cast<char*>(&alphabetcount), sizeof(alphabetcount));
	fin.read(reinterpret_cast<char*>(&totalcharacters), sizeof(totalcharacters));

	// Read the symbol and weight data and construct symbolmap
	ClearSymbolMap();
	char tempsymbol = 0;
	int tempcount = 0;
	for (unsigned int count = 0; count < alphabetcount; count++)
	{
		fin.read(reinterpret_cast<char*>(&tempsymbol), sizeof(char));
		fin.read(reinterpret_cast<char*>(&tempcount), sizeof(int));
#ifdef ORIGINAL
		symbolmap.insert(make_pair(tempsymbol, tempcount));
#else
		symbolarray[tempsymbol]->weight = tempcount;
		symbolarray[tempsymbol]->isleaf = true;
#endif
	}
	return true;
};

// Process the compressed section of the file
void HuffmanCode::ReadCompressedFile(std::ifstream& fin, std::ofstream& fout)
{
	// Reposition start point past the header information
	size_t startpos = (
#ifdef ORIGINAL
			sizeof(filetag) + 
#endif
			sizeof(alphabetcount) + sizeof(totalcharacters) + alphabetcount * (sizeof(char) + sizeof(int)));
	fin.seekg(startpos);
	char inchar;
	unsigned char mask = 0x80;
	unsigned char code = 0;
	uintmax_t totalbytes = GetTotalCodedBits() / 8;		// number of complete bytes in the encoded file
	unsigned char extrabits = GetTotalCodedBits() % 8;  // odd bits at the end og the file
	TreeNode* node = treeheap.top();
	for (uintmax_t index = 0; index < totalbytes; index++)
	{
		fin.get(inchar);
		for (unsigned char offset = 0; offset < 8; offset++)
		{
			code = (inchar & mask) ? 1 : 0;
			// look up tree
			node = (code) ? node->rightpointer : node->leftpointer;
			if (node->isleaf)
			{
				fout.write(&node->symbol, 1);
				node = treeheap.top();
			}
			inchar = inchar << 1;
		}
	}
	if (extrabits)  // any spare bits that are not a whole byte
	{
		fin.get(inchar);
        for (unsigned char offset = 0; offset < extrabits; offset++)
		{
			code = (inchar & mask) ? 1 : 0;
			// look up tree
			node = (code) ? node->rightpointer : node->leftpointer;
			if (node->isleaf)
			{
				fout.write(&node->symbol, 1);
				node = treeheap.top();
			}
			inchar = inchar << 1;
		}
	}
}

// Clear character map
void HuffmanCode::ClearSymbolMap()
{
#ifdef ORIGINAL
	symbolmap = {};
#else
	for (size_t i = 0; i < 128; i++) {
		delete symbolarray[i];
		symbolarray[i] = new TreeNode(i, 0, false);
	}
#endif
};

// Clear the tree
void HuffmanCode::ClearHuffmanTree()
{
	treeheap = {};
};

// Clear all tuples
void HuffmanCode::ClearCodeTable()
{
	codetable = {};
};

// Make the Huffman tree
void HuffmanCode::MakeCodesFromTree()
{
	MakePrefixCodes(treeheap.top(), "");
}

// Generate Huffman Prefix Codes as strings
void HuffmanCode::MakePrefixCodes(TreeNode* node, string str)
{
	if (!node)  // If node is Null then return.
	{
		return;
	}
	// If the node's leaf is true, that means it's not an internal node and save the string.
	if (node->isleaf)
	{
#ifdef ORIGINAL
		codetable.push_back(make_tuple(node->symbol, node->weight, str));
#else
		node->code = str;
#endif
	}
	MakePrefixCodes(node->leftpointer, str + "0");
	MakePrefixCodes(node->rightpointer, str + "1");
}

// Add a character to the symbolmap.
// Add entry for new symbols or update count for existing symbols
void HuffmanCode::MapSymbol(char character)
{
#ifdef ORIGINAL
	// Traverse the map looking for existing entry
	auto it = symbolmap.find(character);
	if (it != symbolmap.end())  // existing symbol entry, update count
	{
		it->second++;
	}
	else
	{
		// new symbol found, add entry
		symbolmap.insert(make_pair(character, 1));
		alphabetcount++;      // keep count of different symbols
	}
#else
	if (! symbolarray[character]->weight++ ) {
		symbolarray[character]->isleaf = true;
		alphabetcount++;
	}
#endif
	totalcharacters++;      // keep track of total characters read
	return;
}

// Sort tuple by value of second element
bool sortbysec(const tuple<char, int, string>& a, const tuple<char, int, string>& b)
{
	return (get<1>(b) < get<1>(a));
}

// Sort tuple based on symbol weight
void HuffmanCode::SortCodeTable()
{
	sort(codetable.begin(), codetable.end(), sortbysec);
}

// Print Huffman Codes
void HuffmanCode::PrintCodeTable()
{
	for (const auto& it : codetable)
	{
		cout << std::get<0>(it) << "\t: " << std::get<1>(it) << "\t: " << std::get<2>(it) << endl;
	}
}

#ifdef ORIGINAL
// Give a copy of CharacterMap when requested, if it has been built
bool HuffmanCode::GetSymbolMap(map<char, int>& tempmap)
{
	bool result = false;

	if (symbolmap.size())
	{
		tempmap = symbolmap;
		result = true;
	}
	return result;
};
#endif

// Return number of symbols in the CharacterMap
uint16_t HuffmanCode::GetGetAlphabetCount()
{
	return alphabetcount;
};

// Return total number of characters in the file
uintmax_t HuffmanCode::GetTotalCharacters()
{
	return totalcharacters;
};



// Return total number of bits in the coded file
uintmax_t HuffmanCode::GetTotalCodedBits()
{
	size_t totalbits = 0;
#ifdef ORIGINAL
	for (const auto& it : codetable)
	{
		// multiply number of times symbol appears by the number of bits in the prefix code
		totalbits += (std::get<1>(it) * (std::get<2>(it)).length());
	}
#else
	for (size_t i = 0; i < 128; i++) {
		totalbits += symbolarray[i]->weight * symbolarray[i]->code.length();
	}
#endif
	return totalbits;
};
