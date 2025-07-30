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

        Token peekToken(int i = 1){
            return tokens[i];
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
                case TokenArt::If:
                    return parseIf();
                case TokenArt::For:
                    return parseFor();
                case TokenArt::Input:
                    return parseInput();
                case TokenArt::Function:
                    return parseFunction();
                case TokenArt::Call:
                    return parseFunctionCall();
                case TokenArt::Return:
                    return parseReturn();
                case TokenArt::While:
                    return parseWhile();
                case TokenArt::Array:
                    return parseArray();
                case TokenArt::Do:
                    return parseDoWhile();
                case TokenArt::Break:
                    return parseBreak();
                case TokenArt::Continue:
                    return parseContinue();
                case TokenArt::Import:
                    return parseImport();
                case TokenArt::System:
                    return parseSystem();
                default:
                    return parseExpressions();
            }
        }

        shared_ptr<Expression> parseExpressions(){
            return parseVariableAssignment();
        }

        shared_ptr<Expression> parsePrimitives(){
            if (thisToken().art == TokenArt::BinaryOperator && thisToken().value == "-") {
                thisEat();
                if (thisToken().art == TokenArt::Number) {
                    double value = -stod(thisEat().value);
                    return make_shared<NumberNode>(value);
                }else if(thisToken().art == TokenArt::Identifier){
                    return make_shared<BinaryNode>(make_shared<NumberNode>(0), make_shared<IdentifierNode>(thisToken().value), "-");
                }else {
                    cerr<<"\n[[Stage]]: Parsing     [[ERROR]] : Expected Number or Identifier after '-', got "<<thisToken().value;
                    exit(1);
                }
            }
            switch (thisToken().art){
                case TokenArt::Number:
                    if(peekToken(1).art == TokenArt::Dot){
                        if (peekToken(2).art == TokenArt::Number) {
                            string doubelNumber = thisEat().value;
                            doubelNumber += thisEat().value;
                            doubelNumber += thisEat().value;
                            return make_shared<NumberNode>(stod(doubelNumber));
                        }else{
                            cerr<<"\n[[Stage]]: Parsing     [[ERROR]] : Expected Number after '.', got "<<thisToken().value;
                            exit(1);
                        }
                    }else{
                        return make_shared<NumberNode>(stod(thisEat().value)); 
                    }
                case TokenArt::Identifier:
                    if(peekToken(1).art == TokenArt::OpenBracket) {
                        return parseArrayCall();
                    }
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
            while(notTheEnd() && (thisToken().value=="*" || thisToken().value=="/" || thisToken().value=="%" || thisToken().value=="^")){
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

            if(thisToken().art == TokenArt::Call){
                shared_ptr<Expression> assignValue = parseFunctionCall();
                return make_shared<VariableDeclarationNode>(variableName, assignValue, isConst);
            }else{
                shared_ptr<Expression> assignValue = parseExpressions();
                expect(TokenArt::Semicolon, ";");
                return make_shared<VariableDeclarationNode>(variableName, assignValue, isConst);
            }
        }

        shared_ptr<Expression> parseVariableAssignment(){
            shared_ptr<Expression> left = parseAdditivBinary();
            if(notTheEnd() && thisToken().art== TokenArt::Equal){
                thisEat();
                if(thisToken().art == TokenArt::Call){
                    shared_ptr<Expression> assignValue = parseFunctionCall();
                    return make_shared<VariableAssignmentNode>(left, assignValue);	
                }else{
                    shared_ptr<Expression> assignValue = parseAdditivBinary();
                    expect(TokenArt::Semicolon, ";");
                    return make_shared<VariableAssignmentNode>(left, assignValue);
                }
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

        shared_ptr<Expression> parseConditional(){
            shared_ptr<Expression> left = parseAdditivBinary();
            if(notTheEnd() && thisToken().value == "<" || thisToken().value == ">" || thisToken().value == "==" || thisToken().value == "<=" || thisToken().value == ">="){
                string conditionOperator = thisEat().value;
                shared_ptr<Expression> right = parseAdditivBinary();
                return make_shared<ConditionalNode>(left, right, conditionOperator);
            }else{
                cerr<<"\n[[Stage]]: Parsing     [[ERROR]] : Expected conditional operator got "<<thisToken().value;
                exit(1);
            }
        }

        shared_ptr<Statement> parseIf(){
            thisEat();
            expect(TokenArt::OpenParen, "(");
            shared_ptr<Expression> condition = parseConditional();
            expect(TokenArt::CloseParen, ")");
            expect(TokenArt::OpenBrace, "{");
            vector<shared_ptr<Statement>> ifBody;
            while(notTheEnd() && thisToken().art != TokenArt::CloseBrace){
                ifBody.push_back(parseStatements());
            }
            expect(TokenArt::CloseBrace, "}");
            if(notTheEnd() && thisToken().art == TokenArt::Else){
                thisEat();
                expect(TokenArt::OpenBrace, "{");
                vector<shared_ptr<Statement>> elseBody;
                while(notTheEnd() && thisToken().art!= TokenArt::CloseBrace){
                    elseBody.push_back(parseStatements());
                }
                expect(TokenArt::CloseBrace, "}");
                return make_shared<IfNode>(condition, ifBody, elseBody);
            }
            return make_shared<IfNode>(condition, ifBody);
        }

        shared_ptr<Statement> parseFor(){
            thisEat();
            expect(TokenArt::OpenParen, "(");
            shared_ptr<Statement> initializer = parseVariableDeclaration(false);
            shared_ptr<Expression> condition = parseConditional();
            expect(TokenArt::Semicolon, ";");
            shared_ptr<Statement> increment = parseVariableAssignment();
            expect(TokenArt::CloseParen, ")");
            expect(TokenArt::OpenBrace, "{");
            vector<shared_ptr<Statement>> forBody;
            while(notTheEnd() && thisToken().art!= TokenArt::CloseBrace){
                forBody.push_back(parseStatements());
            }
            expect(TokenArt::CloseBrace, "}");
            return make_shared<ForNode>(initializer, condition, increment, forBody);
        }

        shared_ptr<Statement> parseInput(){
            thisEat();
            expect(TokenArt::OpenParen, "(");
            string variableName = expect(TokenArt::Identifier, "Identifier VariableName").value;
            expect(TokenArt::Comma, ",");
            string inputType = expect(TokenArt::Identifier, "Identifier InputType s/n/b").value;
            expect(TokenArt::CloseParen, ")");
            expect(TokenArt::Semicolon, ";");
            return make_shared<InputNode>(variableName, inputType);
        }
        
        shared_ptr<Statement> parseFunction(){
            thisEat();
            string functionName = expect(TokenArt::Identifier, "Identifier FunctionName").value;
            expect(TokenArt::OpenParen, "(");
            vector<string> parameters;
            while(notTheEnd() && thisToken().art != TokenArt::CloseParen){
                parameters.push_back(expect(TokenArt::Identifier, "Identifier ParameterName").value);
                if(notTheEnd() && thisToken().art != TokenArt::CloseParen){
                    expect(TokenArt::Comma, ",");
                }
            }
            expect(TokenArt::CloseParen, ")");
            expect(TokenArt::OpenBrace, "{");
            vector<shared_ptr<Statement>> functionBody;
            while(notTheEnd() && thisToken().art!= TokenArt::CloseBrace){
                functionBody.push_back(parseStatements());
            }
            expect(TokenArt::CloseBrace, "}");
            return make_shared<FunctionDeclarationNode>(functionName, parameters, functionBody);
        }

        shared_ptr<Expression> parseFunctionCall(){
            thisEat();
            string functionName = expect(TokenArt::Identifier, "Identifier FunctionName").value;
            expect(TokenArt::OpenParen, "(");
            vector<shared_ptr<Expression>> arguments;
            while(notTheEnd() && thisToken().art!= TokenArt::CloseParen){
                arguments.push_back(parseExpressions());
                if(notTheEnd() && thisToken().art!= TokenArt::CloseParen){
                    expect(TokenArt::Comma, ",");
                }
            }
            expect(TokenArt::CloseParen, ")");
            expect(TokenArt::Semicolon, ";");
            return make_shared<CallNode>(functionName, arguments);
        }

        shared_ptr<Expression> parseReturn(){
            thisEat();
            shared_ptr<Expression> returnValue = parseExpressions();
            expect(TokenArt::Semicolon, ";");
            return make_shared<ReturnNode>(returnValue);
        }

        shared_ptr<Statement> parseWhile(){
            thisEat();
            expect(TokenArt::OpenParen, "(");
            shared_ptr<Expression> condition = parseConditional();
            expect(TokenArt::CloseParen, ")");
            expect(TokenArt::OpenBrace, "{");
            vector<shared_ptr<Statement>> whileBody;
            while(notTheEnd() && thisToken().art!= TokenArt::CloseBrace){
                whileBody.push_back(parseStatements());
            }
            expect(TokenArt::CloseBrace, "}");
            return make_shared<WhileNode>(condition, whileBody);
        }

        shared_ptr<Expression> parseArray(){
            thisEat();
            expect(TokenArt::Lesser, "<");
            shared_ptr<Expression> arraySize = nullptr;
            if(thisToken().art != TokenArt::Greater){
                arraySize = parseExpressions();
            }
            expect(TokenArt::Greater, ">");
            string variableName = expect(TokenArt::Identifier, "Identifier").value;
            expect(TokenArt::Equal, "=");
            expect(TokenArt::OpenBracket, "[");
            vector<shared_ptr<Expression>> body;
            while (notTheEnd() && thisToken().art != TokenArt::CloseBracket){
                body.push_back(parseExpressions());
                if(notTheEnd() && thisToken().art != TokenArt::CloseBracket){
                    expect(TokenArt::Comma, ",");
                }
            }
            expect(TokenArt::CloseBracket, "]");
            expect(TokenArt::Semicolon, ";");
            if(arraySize == nullptr){
                return make_shared<ArrayNode>(variableName, body);
            }
            return make_shared<ArrayNode>(variableName, body, arraySize);
        }

        shared_ptr<Expression> parseArrayCall(){
            string arrayName = thisEat().value;
            shared_ptr<Expression> arrayIndex = nullptr;
            expect(TokenArt::OpenBracket, "[");
            if(thisToken().art!= TokenArt::CloseBracket){
                arrayIndex = parseExpressions();
            }
            expect(TokenArt::CloseBracket, "]");
            if(arrayIndex == nullptr){
                cerr<<"\n[[Stage]]: Parsing     [[ERROR]] : Expected Array Index got "<<thisToken().value;
                exit(1);
            }
            return make_shared<ArrayCallNode>(arrayName, arrayIndex);
        }

        shared_ptr<Statement> parseDoWhile(){
            thisEat();
            expect(TokenArt::OpenBrace, "{");
            vector<shared_ptr<Statement>> doWhileBody;
            while(notTheEnd() && thisToken().art!= TokenArt::CloseBrace){
                doWhileBody.push_back(parseStatements());
            }
            expect(TokenArt::CloseBrace, "}");
            expect(TokenArt::While, "while");
            expect(TokenArt::OpenParen, "(");
            shared_ptr<Expression> condition = parseConditional();
            expect(TokenArt::CloseParen, ")");
            expect(TokenArt::Semicolon, ";");
            return make_shared<DoWhileNode>(doWhileBody, condition);
        }

        shared_ptr<Expression> parseBreak(){
            thisEat();
            expect(TokenArt::Semicolon, ";");
            return make_shared<BreakNode>();
        }

        shared_ptr<ContinueNode> parseContinue(){
            thisEat();
            expect(TokenArt::Semicolon, ";");
            return make_shared<ContinueNode>();
        }

        shared_ptr<ImportNode> parseImport(){
            thisEat();
            string importPath = expect(TokenArt::String, "String ImportPath").value;
            expect(TokenArt::Semicolon, ";");
            return make_shared<ImportNode>(importPath);
        }

        shared_ptr<SystemNode> parseSystem(){
            thisEat();
            expect(TokenArt::OpenParen, "(");
            string systemCommand = expect(TokenArt::String, "String SystemCommand").value;
            expect(TokenArt::CloseParen, ")");
            expect(TokenArt::Semicolon, ";");
            return make_shared<SystemNode>(systemCommand);
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