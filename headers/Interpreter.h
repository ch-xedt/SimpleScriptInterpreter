#ifndef Interpreter_h_v1
#define Interpreter_h_v1

#include "Values.h"
#include "AstNodes.h"
#include "Environment.h"

class Interpreter{
    private:
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
            }

            return result;
        }

        shared_ptr<R_Value> evaluateVariableDeclarationNode(shared_ptr<VariableDeclarationNode> variableDeclarationNode,shared_ptr<Environment> environment){
            shared_ptr<R_Value> result = variableDeclarationNode->value ? evaluate(variableDeclarationNode->value, environment) : makeNullValue();
            return environment->declareVariable(variableDeclarationNode->name,result,variableDeclarationNode->IsConstant);
        }

        shared_ptr<R_Value> evaluateVariableAssignmentNode(shared_ptr<VariableAssignmentNode> variableAssignmentNode, shared_ptr<Environment> environment){
            if(variableAssignmentNode->assignmentVariable->node != NodeType::IdentifierNode){
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid assignment variable type \n";
                exit(1);
            }

            shared_ptr<IdentifierNode> assignmentVariable = dynamic_pointer_cast<IdentifierNode>(variableAssignmentNode->assignmentVariable);
            shared_ptr<R_Value> value = evaluate(variableAssignmentNode->value, environment);
            return environment->assignVariable(assignmentVariable->value,value);
        }

};



#endif