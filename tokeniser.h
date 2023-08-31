//
// Created by harsh on 8/29/23.
//

#ifndef TOKENISER_PPLASS1_TOKENISER_H
#define TOKENISER_PPLASS1_TOKENISER_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "def.h"

using namespace std;


bool isWhiteSpace(char c){
    return     c==' '
               or c=='\n'
               or c=='\t'
               or (9<=c and c<=13);
}

bool isPunctuator(char c){
    return c == '(' or c == ')' or c == '[' or c == ']' or c == '{' or c == '}' or
           c == ';' or c == ',' or c == '?' or c == ':' or c == '.';
}

bool isOperator(char c){
    return c == '+' or c == '-' or c == '/' or c == '*' or c == '%' or c == '=' or
           c == '<' or c == '>';
}

bool isSpecial(char c) {
    return isPunctuator(c) or isOperator(c);
}

bool isAlphaNumeric(char c){ return isalnum(c) or c=='_'; }

class LexicalAnalyser{

    map<string, int> keywords = {
            {"if", IF},
            {"while", WHILE},
            {"short", SHORT},
            {"int", INT},
            {"float", FLOAT},
            {"double", DOUBLE},
            {"bool", BOOL},
            {"char", CHAR},
            {"signed", SIGNED},
            {"unsigned", UNSIGNED},
            {"for", FOR},
            {"do", DO},
            {"return", RETURN},
            {"struct",STRUCT},
            {"const",CONST},
            {"void", VOID},
            {"switch", SWITCH},
            {"break", BREAK},
            {"case", CASE},
            {"continue", CONTINUE},
            {"goto", GOTO},
            {"long", LONG},
            {"static", STATIC},
            {"union", UNION},
            {"default", DEFAULT},
            {"else", ELSE}
    };

    map<char,int> punctuators = {
            {'[',L_SQUARE},
            {']',R_SQUARE},
            {'(',L_PARAEN},
            {')',R_PARAEN},
            {'{',L_CBRACE},
            {'}',R_CBRACE},
            {',',COMMA},
            {';',SEMICOL}
    };

    map<char,int> singlecharOperators = {
            {'+',PLUS},
            {'-',MINUS},
            {'*',MULT},
            {'/',DIV},
            {'%',PERCENT},
            {'=',EQ},
            {'<',LESSER},
            {'>',GREATER}
    };

    map<string,int> doublecharOperators = {
            {"++",PLUS_PLUS},
            {"--",MINUS_MINUS},
            {"+=",PLUS_EQ},
            {"-=",MINUS_EQ},
            {"*=",MULT_EQ},
            {"/=",DIV_EQ},
            {"==",EQ_EQ},
            {"<=",LESS_EQ},
            {">=",GREAT_EQ}
    };

    int i = 0;
    int latestToken;

public:

    string code;

    LexicalAnalyser(string feededCode){
        code = feededCode;
    }

    set<pair<string,short int>> symbols;
    vector<pair<int,string>> tokens;

    bool detectKW(string kw){
        int j;
        for(j=0;j<kw.length();j++){
            if(kw[j]!=code[i+j]) return false;
        }
        return isWhiteSpace(code[i+j]) or isSpecial(code[i+j]);
    }


    int detectID(){
        if(code[i]=='_' and code[i+1]=='_') {cerr<<"Identifiers with 2 leading underscores disallowed"; return -1;}
        if(isdigit(code[i])) return -1;
        int j = 0;
        while (i+j<code.length() and isAlphaNumeric(code[i + j])) j++;
        if(isWhiteSpace(code[i+j]) or isSpecial(code[i+j])) return j;
        return -1;
    }

    int detectNum() {
        int j = 0;
        bool f = false;
        if (code[i+j]=='+' or code[i+j]=='-') j++;
        while (i+j<code.length() and isdigit(code[i+j])) j++;

        if (i+j<code.length() and code[i+j] == '.') {
            f = true;
            j++;
            while (i+j<code.length() and isdigit(code[i+j])) j++;
        }
        if (j>0){
            latestToken = f ? FLO_CONST : INT_CONST;
            return j;
        }
        return -1;
    }





    void scan(){

        while(i<code.length()){

            while(i<code.length() and isWhiteSpace(code[i])) i++;
            bool detected = false;



            //detecting comments
            if(code[i]=='/' and code[i+1]=='*'){
                i+=2;
                while(!(code[i]=='*' and code[i+1]=='/')) {
                    i++;
                    if (i >= code.length()) { cerr << "parse Error: EOF while scanning comment"; return; }
                }
                i+=2;
                detected = true;
            }
            if(detected) continue;



            //detecting punctuators: ; , [ ] { etc...
            if(isPunctuator(code[i])) {
                detected = true;
                latestToken = punctuators[code[i]];
                tokens.push_back({punctuators[code[i]],""});
                i++;
            }
            if(detected) continue;



            //detecting keywords
            for(const auto& pair : keywords){           //check keyword
                string kw = pair.first;
                int id = pair.second;
                if(detectKW(kw)){
                    if(symbols.find({kw,0})==symbols.end())
                        symbols.insert({kw,0});
                    tokens.push_back({id,""});
                    i += kw.length();
                    detected = true;
                    latestToken = id;
                    break;
                }
            }
            if(detected) continue;





            //encountering + or -
            if(code[i]=='+' or code[i]=='-'){
                if(code[i+1]==code[i]  or code[i+1]=='='){
                    string w;
                    w += code[i]; w+= code[i+1];
                    latestToken = doublecharOperators[w];
                    tokens.push_back({latestToken,""});
                    i+=2;
                    detected = true;
                }
                else if(isdigit(code[i+1])){
                    if(latestToken==IDNTIFIER or latestToken==INT_CONST or latestToken==FLO_CONST){
                        tokens.push_back({singlecharOperators[code[i]],""});
                        i++;
                        detected = true;
                    }
                }
            }
            if(detected) continue;




            int k = detectNum();
            if(k!=-1){
                string num = code.substr(i, k);
                tokens.push_back({latestToken,num});
                detected = true;
                i+=k;
            }
            if(detected) continue;




            if(isOperator(code[i])){
                if(isOperator(code[i+1])){
                    string op;
                    op += code[i];
                    op += code[i+1];
                    if(doublecharOperators.find(op)!=doublecharOperators.end()){

                        tokens.push_back({doublecharOperators[op],""});
                        latestToken = doublecharOperators[op];
                        i+=2;
                        detected = true;
                    }
                    else {
                        if(op=="+-" or op=="-+") {
                            detected = true;
                            tokens.push_back({singlecharOperators[code[i]],""});
                            i++;
                        }
                        else {cerr << "illegal operator "<<op<<" at position "<<i<<", terminating now"; return;}
                    }

                }
                else{
                    char op = code[i];
                    tokens.push_back({singlecharOperators[op],""});
                    latestToken = singlecharOperators[op];
                    detected = true;
                    i+=1;
                }
            }
            if(detected) continue;




            k = detectID();
            if (k>0) {
                detected = true;
                latestToken = IDNTIFIER;
                string w;
                int j;
                for (j=0;j<k;j++) w += code[i+j];

                tokens.push_back({IDNTIFIER,w});
                if(symbols.find({w,1})==symbols.end())
                    symbols.insert({w,1});
                i += k;
            }
            if(detected) continue;



            if(i<code.length() and !detected) { cerr << "Invalid token detected, terminating" << endl; return; }

        }
    }



};


#endif
