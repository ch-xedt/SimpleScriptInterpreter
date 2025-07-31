#ifndef Interpreter_h_v1
#define Interpreter_h_v1

#include "Values.h"
#include "AstNodes.h"
#include "Environment.h"
#include "Parser.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include "filesystem"

using namespace std;

class Interpreter{
    private:
        struct ReturnException{
            shared_ptr<R_Value> returnValue;
            explicit ReturnException( shared_ptr<R_Value> returnValue) : returnValue(std::move(returnValue)){}
        };
        struct BreakException{};
        struct ContinueException{};
    public:
        shared_ptr<R_Value> evaluate(shared_ptr<Statement> astNode, shared_ptr<Environment> environment){
           switch(astNode->node){
                case NodeType::ProgramNode:                
                    {
                        shared_ptr<Program> programNode = dynamic_pointer_cast<Program>(astNode);
                        return evaluateProgramNode(programNode,environment);
                    }
                case NodeType::NumberNode:
                    {
                        shared_ptr<NumberNode> numberNode = dynamic_pointer_cast<NumberNode>(astNode);
                        return evaluateNumberNode(numberNode,environment);
                    }
                case NodeType::StringNode:
                    {
                        shared_ptr<StringNode> stringNode = dynamic_pointer_cast<StringNode>(astNode);
                        return evaluateStringNode(stringNode,environment);
                    }
                case NodeType::IdentifierNode:
                    {
                        shared_ptr<IdentifierNode> identifierNode = dynamic_pointer_cast<IdentifierNode>(astNode);
                        return evaluateIdentifierNode(identifierNode,environment);
                    }
                case NodeType::BinaryNode:
                    {
                        shared_ptr<BinaryNode> binaryNode = dynamic_pointer_cast<BinaryNode>(astNode);
                        return evaluateBinaryNode(binaryNode,environment);
                    }
                case NodeType::VariableDeclarationNode:
                    {
                        shared_ptr<VariableDeclarationNode> variableDeclarationNode = dynamic_pointer_cast<VariableDeclarationNode>(astNode);
                        return evaluateVariableDeclarationNode(variableDeclarationNode,environment);
                    }
                case NodeType::VariableAssignmentNode:
                    {
                        shared_ptr<VariableAssignmentNode> variableAssignmentNode = dynamic_pointer_cast<VariableAssignmentNode>(astNode);
                        return evaluateVariableAssignmentNode(variableAssignmentNode,environment);
                    }
                case NodeType::PrintNode:
                    {
                        shared_ptr<PrintNode> printNode = dynamic_pointer_cast<PrintNode>(astNode);
                        return evaluatePrintNode(printNode,environment);
                    }
                case NodeType::IfNode:
                    {
                        shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(astNode);
                        return evaluateIfNode(ifNode,environment);
                    }
                case NodeType::ConditionalNode:
                    {
                        shared_ptr<ConditionalNode> conditionalNode = dynamic_pointer_cast<ConditionalNode>(astNode);
                        return evaluateConditionalNode(conditionalNode,environment);
                    }
                case NodeType::ForNode:
                    {
                        shared_ptr<ForNode> forNode = dynamic_pointer_cast<ForNode>(astNode);
                        return evaluateForNode(forNode,environment);
                    }
                case NodeType::InputNode:
                    {
                        shared_ptr<InputNode> inputNode = dynamic_pointer_cast<InputNode>(astNode);
                        return evaluateInputNode(inputNode,environment);
                    }
                case NodeType::FunctionDeclarationNode:
                    {
                        shared_ptr<FunctionDeclarationNode> inputNode = dynamic_pointer_cast<FunctionDeclarationNode>(astNode);
                        return evaluateFunctionDeclarationNode(inputNode,environment);
                    }
                case NodeType::CallNode:
                    {
                        shared_ptr<CallNode> callNode = dynamic_pointer_cast<CallNode>(astNode);
                        return evaluateCallNode(callNode,environment);
                    }
                case NodeType::ReturnNode:
                    {
                        shared_ptr<ReturnNode> returnNode = dynamic_pointer_cast<ReturnNode>(astNode);
                        return evaluateReturnNode(returnNode,environment);
                    }
                case NodeType::WhileNode:
                    {
                        shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(astNode);
                        return evaluateWhileNode(whileNode,environment);
                    }
                case NodeType::ArrayNode:
                    {
                        shared_ptr<ArrayNode> arrayNode = dynamic_pointer_cast<ArrayNode>(astNode);
                        return evaluateArrayNode(arrayNode,environment);
                    }
                case NodeType::ArrayCallNode:
                    {
                        shared_ptr<ArrayCallNode> arrayCallNode = dynamic_pointer_cast<ArrayCallNode>(astNode);
                        return evaluateArrayCallNode(arrayCallNode, environment);
                    }
                case NodeType::DoWhileNode:
                    {
                        shared_ptr<DoWhileNode> doWhileNode = dynamic_pointer_cast<DoWhileNode>(astNode);
                        return evaluateDoWhileNode(doWhileNode,environment);
                    }
                case NodeType::BreakNode:
                    {
                        shared_ptr<BreakNode> breakNode = dynamic_pointer_cast<BreakNode>(astNode);
                        return evaluateBreak(breakNode, environment);
                    }
                case NodeType::ContinueNode:
                    {
                        shared_ptr<ContinueNode> continueNode = dynamic_pointer_cast<ContinueNode>(astNode);
                        return evaluateContinue(continueNode, environment);
                    }
                case NodeType::ImportNode:
                    {
                        shared_ptr<ImportNode> importNode = dynamic_pointer_cast<ImportNode>(astNode);
                        return evaluateImportNode(importNode, environment);
                    }
                case NodeType::SystemNode:
                    {
                        shared_ptr<SystemNode> systemNode = dynamic_pointer_cast<SystemNode>(astNode);
                        return evaluateSystemNode(systemNode, environment);
                    }
                case NodeType::NotNode:
                    {
                        shared_ptr<NotNode> notNode = dynamic_pointer_cast<NotNode>(astNode);
                        return evaluateNotNode(notNode, environment);
                    }
                default:
                    cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid node type\n";
                    astNode->print();
                    exit(1);
           } 
        }

        shared_ptr<R_Value> evaluateProgramNode(shared_ptr<Program> programNode,shared_ptr<Environment> environment){
           shared_ptr<R_Value> result;
            for (auto& statement : programNode->statements){
               result = evaluate(statement, environment);
            } 
            return result;
        }

        shared_ptr<R_Value> evaluateNumberNode(shared_ptr<NumberNode> numberNode,shared_ptr<Environment> environment){
            shared_ptr<NumberValue> numValue = make_shared<NumberValue>();
            numValue->value = numberNode->value;
            return numValue;
        }

        shared_ptr<R_Value> evaluateStringNode(shared_ptr<StringNode> stringNode,shared_ptr<Environment> environment){
            shared_ptr<StringValue> stringValue = make_shared<StringValue>();
            stringValue->value = stringNode->value;
            return stringValue;
        }

        shared_ptr<R_Value> evaluateIdentifierNode(shared_ptr<IdentifierNode> identifierNode,shared_ptr<Environment> environment){
            return environment->lookupVariable(identifierNode->value);
        }

        shared_ptr<R_Value> evaluateBinaryNode(shared_ptr<BinaryNode> binaryNode,shared_ptr<Environment> environment){
            shared_ptr<R_Value> left = evaluate(binaryNode->left,environment);
            shared_ptr<R_Value> right = evaluate(binaryNode->right,environment);

            if(left->type == ValueType::NumberValue && right->type == ValueType::NumberValue){
                return evaluateCaseNumericBinaryNode(binaryNode, dynamic_pointer_cast<NumberValue>(left), dynamic_pointer_cast<NumberValue>(right));
            }else if (left->type == ValueType::StringValue && right ->type == ValueType::StringValue){
                return evaluateCaseStringBinaryNode(binaryNode, dynamic_pointer_cast<StringValue>(left), dynamic_pointer_cast<StringValue>(right)); 
            }else if(left->type == ValueType::NumberValue && right->type == ValueType::StringValue){
                return evaluateCaseNumericStringBinaryNode(binaryNode, dynamic_pointer_cast<NumberValue>(left), dynamic_pointer_cast<StringValue>(right));
            }else if(left->type == ValueType::StringValue && right->type == ValueType::NumberValue){
                return evaluateCaseNumericStringBinaryNode(binaryNode, dynamic_pointer_cast<StringValue>(left), dynamic_pointer_cast<NumberValue>(right));
            }else if(left->type == ValueType::BoolValue && right -> type == ValueType::StringValue){
                return evaluateCaseStringBooleanBinaryNode(binaryNode, dynamic_pointer_cast<BoolValue>(left), dynamic_pointer_cast<StringValue>(right));
            }else if(left->type == ValueType::StringValue && right -> type == ValueType::BoolValue){
                return evaluateCaseStringBooleanBinaryNode(binaryNode, dynamic_pointer_cast<StringValue>(left), dynamic_pointer_cast<BoolValue>(right));
            }else if(left->type == ValueType::BoolValue && right -> type == ValueType::BoolValue){
                return evaluateCaseBooleanBinaryNode(binaryNode, dynamic_pointer_cast<BoolValue>(left), dynamic_pointer_cast<BoolValue>(right));
            }else{
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid binary operator / Case not found " <<binaryNode->op<<" \n";
                exit(1);
            }
        }

        shared_ptr<R_Value> evaluateCaseNumericBinaryNode(shared_ptr<BinaryNode> binaryNode, shared_ptr<NumberValue> left, shared_ptr<NumberValue> right){
            shared_ptr<NumberValue> result = make_shared<NumberValue>();
            
            if (binaryNode->op == "+"){
                result->value = left->value + right->value;
            }else if (binaryNode->op == "-"){
                result->value = left->value - right->value;
            }else if (binaryNode->op == "*"){
                result->value = left->value * right->value; 
            }else if (binaryNode->op == "/"){
                result->value = left->value / right->value; 
            }else if (binaryNode->op == "%"){
                result->value = (int)left->value % (int)right->value;
            }else if (binaryNode->op == "^"){
                result->value = pow(left->value,right->value);
            }else{
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid binary operator " <<binaryNode->op<<" \n";
                exit(1);
            }

            return result;
        }

        shared_ptr<R_Value> evaluateCaseStringBinaryNode(shared_ptr<BinaryNode> binaryNode, shared_ptr<StringValue> left, shared_ptr<StringValue> right){
            shared_ptr<StringValue> result = make_shared<StringValue>(); 

            if (binaryNode->op == "+"){
               result->value = left->value + right->value;
            }else{
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid String binary operator " <<binaryNode->op<<" \n";
                exit(1);
            }

            return result;
        }

        shared_ptr<R_Value> evaluateCaseNumericStringBinaryNode(shared_ptr<BinaryNode> binaryNode, shared_ptr<NumberValue> left, shared_ptr<StringValue> right){
            shared_ptr<StringValue> result = make_shared<StringValue>();

            if (binaryNode->op == "+"){
                if (floor(left->value) == left->value ){
                    result->value = to_string(int(left->value)) + right->value;
                }else{
                    result->value = to_string(left->value) + right->value;   
                }
            }else if (binaryNode->op == "*"){
                string temp = "";
                int limit = (int)left->value;
                for(int i = 0; i<limit; i++){
                    temp += right->value;
                }
                result->value = temp;
            }else{
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid String binary operator " <<binaryNode->op<<" \n";
                exit(1);
            }
            
            return result;
        }
        
        shared_ptr<R_Value> evaluateCaseNumericStringBinaryNode(shared_ptr<BinaryNode> binaryNode, shared_ptr<StringValue> left, shared_ptr<NumberValue> right){
            shared_ptr<StringValue> result = make_shared<StringValue>();

            if (binaryNode->op == "+"){
                if(floor(right->value) == right->value){
                    result->value = left->value + to_string(int(right->value));
                }else{
                    result->value = left->value + to_string(right->value) ;   
                }
            }else if(binaryNode->op == "*"){
                string temp = "";
                int limit = (int)right->value;
                for(int i = 0; i<limit; i++){
                    temp += left->value;
                }
                result->value = temp;
            }else{
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid String binary operator " <<binaryNode->op<<" \n";
                exit(1);
            }

            return result;
        }

        shared_ptr<R_Value> evaluateCaseStringBooleanBinaryNode(shared_ptr<BinaryNode> binaryNode ,shared_ptr<BoolValue> left, shared_ptr<StringValue> right){
            shared_ptr<StringValue> result = make_shared<StringValue>();

            if(binaryNode->op == "+"){
                result->value = left->value == 0 ? "false" : "true" + right->value;
            }else{
                result->value = left->value == 0 ? "false" : "true" + right->value;
            }

            return result;
        }

        shared_ptr<R_Value> evaluateCaseStringBooleanBinaryNode(shared_ptr<BinaryNode> binaryNode, shared_ptr<StringValue> left, shared_ptr<BoolValue> right){
            shared_ptr<StringValue> result = make_shared<StringValue>();

            if(binaryNode->op == "+"){
                result->value = left->value + (right->value == 0 ? "false" : "true");
            }else{
                result->value = left->value + (right->value == 0 ? "false" : "true");
            }

            return result;
        }

        shared_ptr<R_Value> evaluateCaseBooleanBinaryNode(shared_ptr<BinaryNode> binaryNode, shared_ptr<BoolValue> left, shared_ptr<BoolValue> right){
            shared_ptr<BoolValue> result = make_shared<BoolValue>();
            if(binaryNode->op == "&&"){
                result->value = left->value && right->value;
            }else if(binaryNode->op == "||"){
                result->value = left->value || right->value;
            }else{
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid binary operator " <<binaryNode->op<<" \n";
                exit(1);
            }
            return result;
        }

        shared_ptr<R_Value> evaluateVariableDeclarationNode(shared_ptr<VariableDeclarationNode> variableDeclarationNode,shared_ptr<Environment> environment){
            shared_ptr<R_Value> result = variableDeclarationNode->value ? evaluate(variableDeclarationNode->value, environment) : makeNullValue();
            return environment->declareVariable(variableDeclarationNode->name,result,variableDeclarationNode->IsConstant);
        }

        shared_ptr<R_Value> evaluateVariableAssignmentNode(shared_ptr<VariableAssignmentNode> variableAssignmentNode, shared_ptr<Environment> environment){
            if(variableAssignmentNode->assignmentVariable->node != NodeType::IdentifierNode && variableAssignmentNode->assignmentVariable->node != NodeType::ArrayCallNode){
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid assignment value type \n";
                exit(1);
            }
            if(variableAssignmentNode->assignmentVariable->node == NodeType::ArrayCallNode){
                shared_ptr<ArrayCallNode> arrayCallNode = dynamic_pointer_cast<ArrayCallNode>(variableAssignmentNode->assignmentVariable);
                shared_ptr<ArrayValue> arrayValue = dynamic_pointer_cast<ArrayValue>(environment->lookupVariable(arrayCallNode->arrayName));
                shared_ptr<NumberValue> indexValue = dynamic_pointer_cast<NumberValue>(evaluate(arrayCallNode->index, environment));
                if(indexValue->value < 0 || indexValue->value >= arrayValue->body.size()){
                    cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid array index assigning to array("<<arrayCallNode->arrayName<<"), Index out of bounds\n";
                    exit(1);
                }
                shared_ptr<R_Value> assignValue = evaluate(variableAssignmentNode->value,environment);
                if(assignValue->type == arrayValue->body[indexValue->value]->type){
                    arrayValue->body[indexValue->value] = assignValue;
                    return arrayValue->body[indexValue->value];
                }else {
                    cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid value type assigning to array("<<arrayCallNode->arrayName<<")\n";
                    exit(1);
                }
            }else if(variableAssignmentNode->assignmentVariable->node == NodeType::IdentifierNode){
                const string variableName = dynamic_pointer_cast<IdentifierNode>(variableAssignmentNode->assignmentVariable)->value;
                return environment->assignVariable(variableName,evaluate(variableAssignmentNode->value,environment));
            }else{
                return makeNullValue();
            }
        }

        shared_ptr<R_Value> evaluatePrintNode(shared_ptr<PrintNode> printNode,shared_ptr<Environment> environment){
            shared_ptr<R_Value> value = evaluate(printNode->value,environment);
            if(value->type == ValueType::NumberValue){
                cout<<dynamic_pointer_cast<NumberValue>(value)->value;
            }else if(value->type == ValueType::StringValue){
                cout<<"\n"<<dynamic_pointer_cast<StringValue>(value)->value;
            }else if(value->type == ValueType::BoolValue){
                cout<<"\n"<<(dynamic_pointer_cast<BoolValue>(value)->value == 0? "false" : "true");                
            }
            return value;
        }

        shared_ptr<R_Value> evaluateIfNode(shared_ptr<IfNode> ifNode, shared_ptr<Environment> environment){
            shared_ptr<R_Value> condition = evaluate(ifNode->condition,environment);
            if(dynamic_pointer_cast<BoolValue>(condition)->value == true){
                shared_ptr<Environment> env = make_shared<Environment>(environment);
                env->initEnvironment();
                for (auto& statement : ifNode->ifBody){
                    evaluate(statement,env);
                }
                return makeNullValue();
            }else {
                shared_ptr<Environment> env = make_shared<Environment>(environment);
                env->initEnvironment();
                for (auto& statement : ifNode->elseBody){
                    evaluate(statement,env);
                }
                return makeNullValue();
            }
        }

        shared_ptr<R_Value> evaluateConditionalNode(shared_ptr<ConditionalNode> conditionalNode, shared_ptr<Environment> environment){
            shared_ptr<R_Value> left = evaluate(conditionalNode->left,environment);
            shared_ptr<R_Value> right = evaluate(conditionalNode->right,environment);
            shared_ptr<BoolValue> result = make_shared<BoolValue>();
            if(left->type == ValueType::NumberValue && right->type == ValueType::NumberValue){
                if(conditionalNode->conditionOperator == ">"){
                    result->value = dynamic_pointer_cast<NumberValue>(left)->value > dynamic_pointer_cast<NumberValue>(right)->value;
                }else if (conditionalNode->conditionOperator == "<") {
                    result->value = dynamic_pointer_cast<NumberValue>(left)->value < dynamic_pointer_cast<NumberValue>(right)->value;
                }else if (conditionalNode->conditionOperator == "==") {
                    result->value = dynamic_pointer_cast<NumberValue>(left)->value == dynamic_pointer_cast<NumberValue>(right)->value;
                }else if(conditionalNode->conditionOperator == ">=") {
                    result->value = dynamic_pointer_cast<NumberValue>(left)->value >= dynamic_pointer_cast<NumberValue>(right)->value;
                }else if (conditionalNode->conditionOperator == "<=") {
                    result->value = dynamic_pointer_cast<NumberValue>(left)->value <= dynamic_pointer_cast<NumberValue>(right)->value;
                }
            }else if (left->type == ValueType::StringValue && right->type == ValueType::StringValue) {
                if(conditionalNode->conditionOperator == "=="){
                    result->value = dynamic_pointer_cast<StringValue>(left)->value == dynamic_pointer_cast<StringValue>(right)->value;
                }else{
                    cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid conditional operator ("<<conditionalNode->conditionOperator<<") for String-Values\n";
                    exit(1);
                }
            }else if (left->type == ValueType::BoolValue && right->type == ValueType::BoolValue){
                if (conditionalNode->conditionOperator == "==") {
                    result->value = dynamic_pointer_cast<BoolValue>(left)->value == dynamic_pointer_cast<BoolValue>(right)->value;
                }else{
                    cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid conditional operator ("<<conditionalNode->conditionOperator<<") for Boolean-Values\n";
                    exit(1);
                }
            }else {
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid conditional operation between different values\n";
                exit(1);
            }
            return result;
        }

        shared_ptr<R_Value>evaluateForNode(shared_ptr<ForNode> forNode, shared_ptr<Environment> environment){
                shared_ptr<Environment> env = make_shared<Environment>(environment);
                env->initEnvironment();
                shared_ptr<VariableDeclarationNode> variableDeclNode = dynamic_pointer_cast<VariableDeclarationNode>(forNode->initializer);
                shared_ptr<R_Value> initVar = evaluateVariableDeclarationNode(variableDeclNode,env);
                shared_ptr<ConditionalNode> conditionNode = dynamic_pointer_cast<ConditionalNode>(forNode->condition);
                shared_ptr<R_Value> condition = evaluateConditionalNode(conditionNode, env);
                shared_ptr<R_Value> lastValue = makeNullValue();
                while(dynamic_pointer_cast<BoolValue>(condition)->value == true){
                    shared_ptr<Environment> forEnv = make_shared<Environment>(env);
                    forEnv->initEnvironment();
                    try{
                        for (auto& statement : forNode->forBody){
                            lastValue = evaluate(statement,forEnv);
                        }
                    }catch(BreakException&){
                        break;
                    }catch(ContinueException){}
                    shared_ptr<VariableAssignmentNode> incrementNode = dynamic_pointer_cast<VariableAssignmentNode>(forNode->increment);
                    evaluateVariableAssignmentNode(incrementNode, env);
                    condition = evaluateConditionalNode(conditionNode, env);
                }
                return lastValue;
        }

        shared_ptr<R_Value> evaluateInputNode(shared_ptr<InputNode> inputNode, shared_ptr<Environment> environment){
            string input = "";
            if(inputNode->inputType == "n"){
                getline(cin,input);
                shared_ptr<R_Value> inputValue = makeNumberValue(stod(input));
                environment->assignVariable(inputNode->variableName, inputValue );
            }else if(inputNode->inputType == "s"){
                getline(cin,input);
                shared_ptr<R_Value> inputValue = makeStringValue(input);
                environment->assignVariable(inputNode->variableName, inputValue );
            }else if(inputNode->inputType == "b"){
                cin>>input;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if(input == "true" || input == "True" || input == "1" || input == "y" || input == "Y"){
                    shared_ptr<R_Value> inputValue = makeBoolValue(true);
                    environment->assignVariable(inputNode->variableName, inputValue );
                }else if(input == "false" || input == "False" || input == "0" || input == "n" || input == "N") {
                    shared_ptr<R_Value> inputValue = makeBoolValue(false);
                    environment->assignVariable(inputNode->variableName, inputValue );
                }else {
                    cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Input type does not match -- Boolean\n";
                    exit(1);
                }
            }else{
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid input type ("<<inputNode->inputType<<") for ("<< inputNode->variableName<<")\n";
                exit(1);
            }
                return makeNullValue();
        }

        shared_ptr<R_Value> evaluateFunctionDeclarationNode(shared_ptr<FunctionDeclarationNode> functionNode, shared_ptr<Environment> environment){
            shared_ptr<FunctionValue> functionValue = make_shared<FunctionValue>();
            functionValue->functionName = functionNode->functionName;
            functionValue->parameters = functionNode->parameters;
            functionValue->body = functionNode->functionBody;
            functionValue->env = environment;
            return environment->declareVariable(functionNode->functionName,functionValue,true);
        }

        shared_ptr<R_Value> evaluateCallNode(shared_ptr<CallNode> callNode, shared_ptr<Environment> environment){
            shared_ptr<FunctionValue> functionValue = dynamic_pointer_cast<FunctionValue>(environment->lookupVariable(callNode->functionName));
            if(functionValue->parameters.size() != callNode->arguments.size()){
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid number of arguments for function call\n";
                exit(1);
            }
            shared_ptr<Environment> functionEnvironment = make_shared<Environment>(functionValue->env);
            for(int i = 0; i < functionValue->parameters.size(); i++){
                functionEnvironment->declareVariable(functionValue->parameters[i],evaluate(callNode->arguments[i],environment),false);
            }

            shared_ptr<R_Value> returnValue = makeNullValue();

            try{
                for(auto& statement : functionValue->body){
                    evaluate(statement,functionEnvironment);
                }
            }catch (ReturnException &exception){
                returnValue = exception.returnValue;
            }
            return returnValue;
        }

        shared_ptr<R_Value> evaluateReturnNode(shared_ptr<ReturnNode> returnNode, shared_ptr<Environment> environment){
            shared_ptr<R_Value> returnValue;
            if(returnNode->returnValueExpression != nullptr){
                returnValue = evaluate(returnNode->returnValueExpression,environment);
            }else{
                returnValue = makeNullValue();
            }
            throw ReturnException(returnValue);
        }

        shared_ptr<R_Value> evaluateWhileNode(shared_ptr<WhileNode> whileNode, shared_ptr<Environment> environment){
            shared_ptr<Environment> env = make_shared<Environment>(environment);
            env->initEnvironment();
            shared_ptr<ConditionalNode> conditionNode = dynamic_pointer_cast<ConditionalNode>(whileNode->condition);
            shared_ptr<R_Value> condition = evaluateConditionalNode(conditionNode, env);
            while(dynamic_pointer_cast<BoolValue>(condition)->value == true){
                try{
                    for (auto& statement : whileNode->whileBody){
                        evaluate(statement,env);
                    }
                }catch(BreakException&){
                    break;
                }catch(ContinueException&){
                    condition = evaluateConditionalNode(conditionNode, env);
                    continue;
                }
                condition = evaluateConditionalNode(conditionNode, env);
            }
            return makeNullValue();
        }

        shared_ptr<R_Value> evaluateArrayNode(shared_ptr<ArrayNode> arrayNode, shared_ptr<Environment> environment){
            vector<shared_ptr<R_Value>> array;
            shared_ptr<R_Value> sizeValue  = evaluate(arrayNode->size, environment);
            int size = 0;

            if(sizeValue->type == ValueType::NumberValue){
                size = dynamic_pointer_cast<NumberValue>(sizeValue)->value;
            }else{
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid array size, expected NumberValue\n";
                exit(1);
            }
            if (arrayNode->arrayBody.empty()) {
                cerr << "\n[[Stage]] : Interpreting  [[ERROR]] : Array cannot be empty, type cannot be inferred\n";
                exit(1);
            }
            if(size != arrayNode->arrayBody.size()){
                if(size < arrayNode->arrayBody.size()){
                    cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid array size\n";
                    exit(1);
                }
                int pushedSize = 0;
                for(auto &expression : arrayNode->arrayBody){
                    array.push_back(evaluate(expression, environment));
                    pushedSize++;
                }
                switch (array[0]->type) {
                    case ValueType::NumberValue:
                        {
                            for(int i = 0; i < size - pushedSize; i++){
                                array.push_back(makeNumberValue(0));
                            }
                            break;
                        }
                    case ValueType::StringValue:
                        {
                            for(int i = 0; i < size - pushedSize; i++){
                                array.push_back(makeStringValue(""));
                            }
                            break;
                        }
                    case ValueType::BoolValue:
                        {
                            for(int i = 0; i < size - pushedSize; i++){
                                array.push_back(makeBoolValue (false));
                            }
                            break;
                        }
                    default:
                        {
                            cout<<"\n[[Stage]] : Interpreting  [[ERROR]] : Cannot fill array with given ValueType";
                            exit(1);
                        }
                }
            }else{
                for(auto &expression : arrayNode->arrayBody){
                    array.push_back(evaluate(expression, environment));
                }
            }
            if (!array.empty()) {
                for(auto &value : array){
                    if(array[0]->type != value->type){
                        cout << "\n[[Stage]] : Interpreting  [[ERROR]] : Invalid Array";
                        exit(1);
                    }
                }
            }
            shared_ptr<ArrayValue> result = make_shared<ArrayValue>(array);
            environment->declareVariable(arrayNode->arrayName, result);
            return makeNullValue();
        }

        shared_ptr<R_Value> evaluateArrayCallNode(shared_ptr<ArrayCallNode> arrayCallNode, shared_ptr<Environment> environment){
            shared_ptr<ArrayValue> arrayValue = dynamic_pointer_cast<ArrayValue>(environment->lookupVariable(arrayCallNode->arrayName));
            shared_ptr<NumberValue> indexValue = dynamic_pointer_cast<NumberValue>(evaluate(arrayCallNode->index, environment));
            if(indexValue->value >= arrayValue->body.size()){
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid array index Array("<<arrayCallNode->arrayName<<")["<<arrayCallNode->index<<"], Index out of bounds\n";
                exit(1);
            }
            return arrayValue->body[indexValue->value];
        }

        shared_ptr<R_Value> evaluateDoWhileNode(shared_ptr<DoWhileNode> doWhileNode, shared_ptr<Environment> environment){
            shared_ptr<Environment> env = make_shared<Environment>(environment);
            env->initEnvironment();
            shared_ptr<ConditionalNode> conditionNode = dynamic_pointer_cast<ConditionalNode>(doWhileNode->condition);
            while (true) {
                try {
                    for (auto& statement : doWhileNode->doBody){
                        evaluate(statement, env);
                    }
                } catch (BreakException&) {
                    break;
                } catch (ContinueException&) {
                    // continue springt direkt zur Bedingungsprüfung
                    shared_ptr<R_Value> condition = evaluateConditionalNode(conditionNode, env);
                    if (!dynamic_pointer_cast<BoolValue>(condition)->value) {
                        break;
                    } else {
                        continue;
                    }
                }
                shared_ptr<R_Value> condition = evaluateConditionalNode(conditionNode, env);
                if (!dynamic_pointer_cast<BoolValue>(condition)->value) {
                    break;
                }
            }
            return makeNullValue();
        }

        shared_ptr<R_Value> evaluateBreak(shared_ptr<BreakNode> breakNode, shared_ptr<Environment> environment){
            throw BreakException();
        }

        shared_ptr<R_Value> evaluateContinue(shared_ptr<ContinueNode> continueNode, shared_ptr<Environment> environment){
            throw ContinueException();
        }

        shared_ptr<R_Value> evaluateImportNode(shared_ptr<ImportNode> importNode, shared_ptr<Environment> environment){
            namespace fs = std::filesystem;
            fs::path currentRootFile(environment->currentRootPath);
            fs::path currentDir = currentRootFile.parent_path();
            fs::path importPath = currentDir / importNode->importPath;
            if(importPath.extension() != ".aer"){
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid import file, expected .aer file\n";
                exit(1);
            }
            std::ifstream file(importPath.string());
            if(!file.is_open()){
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid import path, file ("<<importNode->importPath<<") not found\n";
                exit(1);
            }
            string content = "";
            string line = "";
            while(getline(file,line)){
                content += line;
            }
            file.close();            
            Parser parser;
            Program program;
            program = parser.produceAST(content);
            for(auto &statement : program.statements){
                evaluate(statement, environment);
            }
            return makeNullValue();
        }

        shared_ptr<R_Value> evaluateSystemNode(shared_ptr<SystemNode> systemNode, shared_ptr<Environment> environment){
            int status = system(systemNode->systemCommand.c_str());
            if (status < 0) {
                cerr << "\n[[Stage]] : Interpreting  [[ERROR]] : System command failed, command: "<<systemNode->systemCommand<<"\n";
                exit(1);
            }else if(status > 0){
                cerr << "\n[[Stage]] : Interpreting  [[ERROR]] : System prozess failed, command: "<<systemNode->systemCommand<<"\n";
                exit(1);
            }else{
                return makeNullValue();
            }
        }

        shared_ptr<R_Value> evaluateNotNode(shared_ptr<NotNode> notNode, shared_ptr<Environment> environment){
            shared_ptr<R_Value> operand = evaluate(notNode->operand, environment);
            if(operand->type == ValueType::BoolValue){
                shared_ptr<BoolValue> result = make_shared<BoolValue>(!dynamic_pointer_cast<BoolValue>(operand)->value);
                return result;
            }else{
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid operand type for NotNode, expected BoolValue, got "<<ValueTypeToString(operand->type)<<"\n";
                exit(1);
            }
        }
};

#endif 