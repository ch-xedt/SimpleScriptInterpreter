#ifndef Interpreter_h_v1
#define Interpreter_h_v1

#include "Values.h"
#include "AstNodes.h"
#include "Environment.h"
#include <cstdlib>
#include <memory>

using namespace std;

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


        shared_ptr<R_Value> evaluateVariableDeclarationNode(shared_ptr<VariableDeclarationNode> variableDeclarationNode,shared_ptr<Environment> environment){
            shared_ptr<R_Value> result = variableDeclarationNode->value ? evaluate(variableDeclarationNode->value, environment) : makeNullValue();
            return environment->declareVariable(variableDeclarationNode->name,result,variableDeclarationNode->IsConstant);
        }

        shared_ptr<R_Value> evaluateVariableAssignmentNode(shared_ptr<VariableAssignmentNode> variableAssignmentNode, shared_ptr<Environment> environment){
            if(variableAssignmentNode->assignmentVariable->node != NodeType::IdentifierNode){
                cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid assignment variable type \n";
                exit(1);
            }
            const string variableName = dynamic_pointer_cast<IdentifierNode>(variableAssignmentNode->assignmentVariable)->value;
            return environment->assignVariable(variableName,evaluate(variableAssignmentNode->value,environment));
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
                }else if (conditionalNode->conditionOperator == "=") {
                    result->value = dynamic_pointer_cast<NumberValue>(left)->value == dynamic_pointer_cast<NumberValue>(right)->value;
                }
            }else if (left->type == ValueType::StringValue && right->type == ValueType::StringValue) {
                if(conditionalNode->conditionOperator == "="){
                    result->value = dynamic_pointer_cast<StringValue>(left)->value == dynamic_pointer_cast<StringValue>(right)->value;
                }else{
                    cerr<<"\n[[Stage]] : Interpreting  [[ERROR]] : Invalid conditional operator ("<<conditionalNode->conditionOperator<<") for String-Values\n";
                    exit(1);
                }
            }else if (left->type == ValueType::BoolValue && right->type == ValueType::BoolValue){
                if (conditionalNode->conditionOperator == "=") {
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
                while(dynamic_pointer_cast<BoolValue>(condition)->value == true){
                    for (auto& statement : forNode->forBody){
                        evaluate(statement,env);
                    }
                    shared_ptr<VariableAssignmentNode> incrementNode = dynamic_pointer_cast<VariableAssignmentNode>(forNode->increment);
                    evaluateVariableAssignmentNode(incrementNode, env);
                    condition = evaluateConditionalNode(conditionNode, env);
                }
                return makeNullValue();
        }

};



#endif 