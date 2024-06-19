#ifndef LZ77COMPRESSION_H
#define LZ77COMPRESSION_H

#include <bits/stdc++.h>
using namespace std;

vector<int> encode(const string& s1) {
    unordered_map<string, int> table;
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        table[ch] = i;
    }

    string p = "", c = "";
    p += s1[0];
    int code = 256; ///////////////EDICION 34ncht2oiunchiowercoiwcmgoierwnhgwoi
    vector<int> output_code;
    for (int i = 0; i < s1.length(); i++) {
        if (i != s1.length() - 1)
            c += s1[i + 1];
        if (table.find(p + c) != table.end()) {
            p = p + c;
        } else {
            output_code.push_back(table[p]);
            table[p + c] = code;
            code++;
            p = c;
        }
        c = "";
    }
    output_code.push_back(table[p]);
    return output_code;
}

string decode(const vector<int>& op) {
    unordered_map<int, string> table;
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
    int count = 256;
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
        table[count] = table[old] + c;
        count++;
        old = n;
    }
    return result;
}

vector<int> compress(const string& input) {
    return encode(input);
}

string decompress(const vector<int>& input) {
    return decode(input);
}

#endif // LZ77COMPRESSION_H

