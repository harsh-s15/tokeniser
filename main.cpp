#include <iostream>
#include <fstream>
#include <string>

#include "tokeniser.h"

using namespace std;

int main(int argc, char* argv[]) {

    ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        cerr << "Unable to open input file: " << argv[1] << endl;
        return 1;
    }

    string code;
    string line;
    while (getline(inputFile, line)) {
        code += line +'\n';
    }
    inputFile.close();

    LexicalAnalyser LA(code);
    LA.scan();

    ofstream pa1File("pa_1.out");

    for (const auto& token : LA.tokens)
        pa1File << token.first << " " << token.second << endl;



    ofstream symbolFile("symbol_table_1.out");

    for (const auto& symbol : LA.symbols)
        symbolFile << symbol.first << " " << symbol.second << endl;


    return 0;
}
