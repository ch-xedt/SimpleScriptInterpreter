#ifndef PARSER_H_v1
#define PARSER_H_v1

#include "Lexer.h"
#include "AstNodes.h"
#include <memory>
#include <vector>

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
                
                case TokenArt::Print:
                    return parsePrint();
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
            shared_ptr<Expression> left = parseMemberExpression();
            while(notTheEnd() && (thisToken().value=="*" || thisToken().value=="/" || thisToken().value=="%")){
                string operatorValue = thisEat().value;
                shared_ptr<Expression> right = parseMemberExpression();
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
            shared_ptr<Expression> left = parseObjectExpression();

            if(notTheEnd() && thisToken().art== TokenArt::Equal){
                thisEat();
                shared_ptr<Expression> assignValue = parseAdditivBinary();
                expect(TokenArt::Semicolon, ";");
                return make_shared<VariableAssignmentNode>(left, assignValue);
            }
            return left;
        }

        shared_ptr<Statement> parsePrint(){
            thisEat();
            expect(TokenArt::OpenParen, "(");
            shared_ptr<Expression> printValue = parseExpressions();
            expect(TokenArt::CloseParen, ")");
            expect(TokenArt::Semicolon, ";");
            return make_shared<PrintNode>(printValue);
        }

        shared_ptr<Expression> parseObjectExpression(){
            vector<shared_ptr<PropertyNode>> properties;
            if(thisToken().art != TokenArt::OpenBrace){
                return parseAdditivBinary();
            }
            thisEat();

            while(notTheEnd() && thisToken().art != TokenArt::CloseBrace){
                string key = expect(TokenArt::Identifier, "Identifier").value;
                if (thisToken().art == TokenArt::Semicolon){
                    thisEat();
                    properties.push_back(make_shared<PropertyNode>(key, nullptr));
                    continue;
                }else if (thisToken().art == TokenArt::CloseBrace){
                    properties.push_back(make_shared<PropertyNode>(key, nullptr));
                    continue;
                }

                expect(TokenArt::Equal, "=");
                shared_ptr<Expression> value = parseExpressions();
                properties.push_back(make_shared<PropertyNode>(key, value));
                
                if(thisToken().art != TokenArt::CloseBrace){
                    expect(TokenArt::Semicolon, ";");
                }
            }

            expect(TokenArt::CloseBrace, "}");
            return make_shared<ObjectNode>(properties);
        }

        shared_ptr<Expression> parseMemberExpression(){
            shared_ptr<Expression> object = parsePrimitives();
            while(thisToken().art == TokenArt::Dot || thisToken().art == TokenArt::OpenParen){
                Token operatorToken = thisEat();
                shared_ptr<Expression> property;
                bool isComputedProperty = false;
                if(operatorToken.art == TokenArt::Dot){
                    isComputedProperty = false;
                    property = parsePrimitives();
                    if (property->node != NodeType::IdentifierNode) {
                        cerr << "\n[[Stage]]: Parsing     [[ERROR]] : Invalid property access. Expected an identifier.";
                        exit(1);
                    }
                }else {
                    isComputedProperty = true;
                    property = parseExpressions();
                    expect(TokenArt::CloseParen, ")");
                }
                object = make_shared<MemberNode>(object, property, isComputedProperty);
            }
            return object;
        }

        shared_ptr<Expression> parseMemberCallExpression(){
            shared_ptr<Expression> member = parseMemberExpression();
            if(thisToken().art == TokenArt::OpenParen){
                return parseCallExpression(member);
            }
            return member;
        }

        shared_ptr<Expression> parseCallExpression(shared_ptr<Expression> caller){
            shared_ptr<Expression> callExpression = make_shared<CallNode>(caller, parseCallArguments());
            if(thisToken().art == TokenArt::OpenParen){
                callExpression = parseCallExpression(callExpression);
            }
            return callExpression;
        }

        vector<shared_ptr<Expression>> parseCallArguments(){
            vector<shared_ptr<Expression>> args;
            expect(TokenArt::OpenParen, "(");
            if(thisToken().art == TokenArt::CloseParen){
                return {};
            }
            args = parseFunctionArgumentsList();
            expect(TokenArt::CloseParen, ")");
            return args;
        }

        vector<shared_ptr<Expression>> parseFunctionArgumentsList(){
            vector<shared_ptr<Expression>> args;
            args.push_back(parseVariableAssignment());
            while(thisToken().art == TokenArt::Comma){
                thisEat();
                args.push_back(parseVariableAssignment());
            }
            return args;
        }



    public:
        Program produceAST(string source){
            lexer.setSource(source);
            tokens = lexer.tokenize();
            lexer.print();
            while (notTheEnd()){
                program.statements.push_back(parseStatements());
            }
            return program;
        }
};

#endif