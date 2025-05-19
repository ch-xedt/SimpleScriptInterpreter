#ifndef Lexer_h_V1
#define Lexer_h_V1

#include "iostream"
#include "vector"
#include "string"
#include "unordered_map"

using namespace std;

enum class TokenArt {
    Let,
    Const,
    Print,
    Number,
    Bool,
    Identifier,
    Equal,//=
    BinaryOperator,//+,-,*,/,%,
    OpenParen, //(
    CloseParen,//)
    OpenBrace,//{
    CloseBrace,//}
    Quote,//"
    String,
    Semicolon,//;
    EndOfFile
};

inline unordered_map<string, TokenArt> KEYWORDS = { //reserved Keywords
    {"let", TokenArt::Let},
    {"const", TokenArt::Const},
    {"print", TokenArt::Print}
};

struct Token {
    string value;
    TokenArt art;
};

class Lexer{
    private:
        vector<Token> tokens;
        string source;
        bool isAlpha(char c) {
            return isalpha(c);
        }
        bool isNum(char c) {
            return isdigit(c);
        }
        bool isSpace(char c) {
            return isspace(c);
        }
        static string to_string(TokenArt type) {
            switch (type) {
                case TokenArt::Let: return "LetToken";
                case TokenArt::Const: return "ConstToken";
                case TokenArt::Print: return "PrintToken";
                case TokenArt::Number: return "NumberToken";
                case TokenArt::Identifier: return "IdentifierToken";
                case TokenArt::BinaryOperator: return "BinaryOperatorToken";
                case TokenArt::Equal: return "EqualToken"; 
                case TokenArt::OpenParen: return "OpenParenToken";
                case TokenArt::CloseParen: return "CloseParenToken";
                case TokenArt::Quote: return "QuoteToken";
                case TokenArt::String: return "StringToken";
                case TokenArt::Semicolon: return "SemicolonToken";
                case TokenArt::EndOfFile: return "EndOfFileToken";
                default: return "UnknownToken";
            } 
        }

    public:
        Lexer(){}; 
        Lexer(string sourceCode):source(sourceCode){};
        void setSource(string sourceCode){
            source=sourceCode;
        }
        vector<Token> tokenize(){
            
            while(!source.empty()){
                if(source[0] == '('){
                    tokens.push_back({"(",TokenArt::OpenParen});
                    source.erase(0, 1);
                }else if (source[0] == ')') {
                    tokens.push_back({ ")", TokenArt::CloseParen });
                    source.erase(0, 1);
                }else if (source[0] == '{') {
                    tokens.push_back({ "{", TokenArt::OpenBrace  });
                    source.erase(0, 1);
                }else if (source[0] == '}') {
                    tokens.push_back({ "}", TokenArt::CloseBrace });
                    source.erase(0, 1);
                }else if (source[0] == '"'){
                    source.erase(0, 1); 
                    string strVal = "";
                    while(!source.empty() && source[0] != '"'){
                        strVal += source[0];
                        source.erase(0, 1); 
                    }
                    tokens.push_back({ strVal, TokenArt::String});

                    if (!source.empty() && source[0] == '"') {
                        source.erase(0, 1);
                    }else{
                        cerr<<"\n[[Stage]] : Lexing  [[ERROR]] : Unclosed String Literal.\n";
                        exit(1);
                    }
                }else if (source[0] == '+' || source[0] == '-' || source[0] == '*' || source[0] == '/' || source[0] == '%') {
                    tokens.push_back({ string(1, source[0]), TokenArt::BinaryOperator });
                    source.erase(0, 1); 
                }else if (source[0] == '=') {
                    tokens.push_back({ "=", TokenArt::Equal });
                    source.erase(0, 1);
                }else if (source[0] == ';') {
                    tokens.push_back({ ";", TokenArt::Semicolon });
                    source.erase(0, 1); 
                }else if (isAlpha(source[0])) {
                    string identifier = "";
                    while (!source.empty() && isAlpha(source[0])) {
                        identifier += source[0];
                        source.erase(0, 1);
                    }
                    if (KEYWORDS.find(identifier) != KEYWORDS.end()) {
                        tokens.push_back({ identifier, KEYWORDS[identifier] }); 
                    }
                    else {
                        tokens.push_back({ identifier, TokenArt::Identifier });
                    }
                }else if (isNum(source[0])) {
                    string number = "";
                    while (!source.empty() && isNum(source[0])) {
                        number += source[0];
                        source.erase(0, 1);
                    }
                    tokens.push_back({ number, TokenArt::Number });
                }else if (isSpace(source[0])) {
                    source.erase(0, 1);
                }else{
                    cerr << "\n[[WARNING]] : Invalid character: ' " << source[0] << " ' in sorce code.";
                }
            }
            tokens.push_back({ "EOF", TokenArt::EndOfFile });
            showTokens();
            return tokens;
        }

        void showTokens(){
            cout<<"\n------------------------ Tokens ------------------------\n ";
            for(int i = 0; i< tokens.size(); i++){
                cout<<"\nType: "<<to_string(tokens[i].art)<<"  ------------  Value: "<<tokens[i].value;
            }
            cout<<"\n\n---------------------------------------------------------\n";
        }
};


#endif