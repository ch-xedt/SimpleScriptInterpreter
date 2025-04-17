#ifndef PARSER_H_v1
#define PARSER_H_v1

#include "Lexer.h"
#include "AstNodes.h"

class Parser{
    private:
        vector<Token> tokens;
        Lexer lexer;
        Program program;

        bool notTheEnd(){
            return !tokens.empty() && tokens[0].art != TokenArt::EndOfFile;
        }

        Token thisToken(){
            return tokens[0];
        }

        Token thisEat(){
            Token prevToken = tokens[0];
            tokens.erase(tokens.begin());
            return prevToken;
        }

        Token expect(TokenArt tokentype, string tokentypeName){
            if (thisToken().art == tokentype){
                return thisEat();
            }else{
                cerr<<"\n[[Stage]]: Parsing     [[ERROR]] : Expected "<<tokentypeName<<" got "<<thisToken().value;
                exit(1);
            }
        }

        shared_ptr<Statement> parseStatements(){

            switch(thisToken().art){
                case TokenArt::Let:
                    return parseVariableDeclaration(false);
                case TokenArt::Const:
                    return parseVariableDeclaration(true);
                default:
                    return parseExpressions();
            }
        }

        shared_ptr<Expression> parseExpressions(){
            return parseVariableAssignment();
        }

        shared_ptr<Expression> parsePrimitives(){
            switch (thisToken().art){
                case TokenArt::Number:
                    return make_shared<NumberNode>(stod(thisEat().value)); 
                case TokenArt::Identifier:
                    return make_shared<IdentifierNode>(thisEat().value);
                case TokenArt::OpenParen:{
                    thisEat();
                    shared_ptr<Expression> value = parseExpressions();
                    expect(TokenArt::CloseParen, "Closing Parenthesis");
                    return value;
                }
                case TokenArt::String:{
                    return make_shared<StringNode>(thisEat().value);
                }
                default:
                    cerr<<"\n[[Stage]]: Parsing     [[ERROR]] Unknown token : "<<thisToken().value;
                    exit(1);
            }
        }

        shared_ptr<Expression> parseAdditivBinary(){
            shared_ptr<Expression> left = parseMultiplicativeBinary();
            while(notTheEnd() && (thisToken().value=="+" || thisToken().value=="-")){
                string operatorValue = thisEat().value;
                shared_ptr<Expression> right = parseMultiplicativeBinary();
                left=make_shared<BinaryNode>(left,right,operatorValue);
            }
            return left;
        }

        shared_ptr<Expression> parseMultiplicativeBinary(){
            shared_ptr<Expression> left = parsePrimitives();
            while(notTheEnd() && (thisToken().value=="*" || thisToken().value=="/" || thisToken().value=="%")){
                string operatorValue = thisEat().value;
                shared_ptr<Expression> right = parsePrimitives();
                left=make_shared<BinaryNode>(left,right,operatorValue);
            } 
            return left;
        }

        shared_ptr<Statement> parseVariableDeclaration(bool isConst){
            isConst = thisEat().art == TokenArt::Const ? true : false;
            string variableName = expect(TokenArt::Identifier, "Identifier").value;
            if(thisToken().art == TokenArt::Semicolon){
                if(thisToken().art == TokenArt::Semicolon && isConst){
                    thisEat();
                    cerr<<"\n[[Stage]]: Parsing     [[ERROR]] : Constant variables must be assigned a value!";
                    exit(1);
                }
                thisEat();
                return make_shared<VariableDeclarationNode>(variableName, nullptr, isConst);
            }
            expect(TokenArt::Equal, "=");
            shared_ptr<Expression> value = parseExpressions();
            expect(TokenArt::Semicolon, ";");
            return make_shared<VariableDeclarationNode>(variableName, value, isConst);
        }

        shared_ptr<Expression> parseVariableAssignment(){
            shared_ptr<Expression> left = parseAdditivBinary();

            if(notTheEnd() && thisToken().art== TokenArt::Equal){
                thisEat();
                shared_ptr<Expression> assignValue = parseAdditivBinary();
                expect(TokenArt::Semicolon, ";");
                return make_shared<VariableAssignmentNode>(left, assignValue);
            }
            return left;
        }


    public:
        Program produceAST(string source){
            lexer.setSource(source);
            tokens = lexer.tokenize();
            while (notTheEnd()){
                program.statements.push_back(parseStatements());
            }
            return program;
        }
};

#endif