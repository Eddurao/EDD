#ifndef LZ77COMPRESSION_H
#define LZ77COMPRESSION_H

#include <bits/stdc++.h>
using namespace std;

vector<int> encode(const string& s1) { 
    unordered_map<string, int> table; //Diccionario que almacena las secuencias vistas
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        table[ch] = i;  //Inicialización
    }

    string p = "", c = "";  //"p" prefijo actual y "c" sufijo actual
    p += s1[0];
    int code = 256; 
    vector<int> output_code; //Vector para almacenar los códigos de salida comprimidos
    for (int i = 0; i < s1.length(); i++) {
        if (i != s1.length() - 1)
            c += s1[i + 1];
        if (table.find(p + c) != table.end()) {
            p = p + c;
        } else {
            output_code.push_back(table[p]); //Se añade al vector el código de p
            table[p + c] = code;
            code++;
            p = c; //Se actualiza el prefijo p
        }
        c = "";
    }
    output_code.push_back(table[p]);
    return output_code; //Devuelve el vector de códigos comprimidos
}

string decode(const vector<int>& op) {
    unordered_map<int, string> table; //Diccionario para almacenar las secuencias recuperadas
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        table[i] = ch;
    }
    int old = op[0], n;
    string s = table[old];
    string c = "";
    c += s[0];
    string result = s;
    int count = 256; //Contador para los próximos códigos disponibles en el diccionario
    for (int i = 0; i < op.size() - 1; i++) {
        n = op[i + 1];
        if (table.find(n) == table.end()) {
            s = table[old];
            s = s + c;
        } else {
            s = table[n];
        }
        result += s;
        c = "";
        c += s[0];
        table[count] = table[old] + c; //Se añade al diccionario la entrada correspondiente
        count++;
        old = n;
    }
    return result;
}

vector<int> compress(const string& input) {
    return encode(input); //Utiliza la función para realizar la compresión
}

string decompress(const vector<int>& input) {
    return decode(input); //Utiliza la función para realizar la descompresión
}

#endif // LZ77COMPRESSION_H

