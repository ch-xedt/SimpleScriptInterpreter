#ifndef AST_H_v1
#define AST_H_v1

#include "iostream"
#include "vector"
#include "string"
#include "memory"
#include <memory>
#include <vector>

using namespace std;

enum class NodeType{
    ProgramNode,
    NumberNode,
    StringNode,
    IdentifierNode,
    BinaryNode,
    VariableDeclarationNode,
    VariableAssignmentNode,
    PrintNode,
    ConditionalNode,
    IfNode,
    ForNode,
    InputNode,
    FunctionDeclarationNode,
    CallNode,
    ReturnNode,
    WhileNode,
    ArrayNode,
    ArrayCallNode,
    DoWhileNode,
};

struct Statement{
    NodeType node;
    Statement(NodeType type):node(type){}
    virtual ~Statement() = default;
    virtual void print(int depth =1) const = 0;
};

struct Expression : public Statement{
    Expression(NodeType type):Statement(type){}
};

struct Program : public Statement{
    Program():Statement(NodeType::ProgramNode){}
    vector<shared_ptr<Statement>> statements;
    void print(int depth) const override{
        cout<<"\n--------------------------- Program ---------------------------";
        string indent (3*depth,' ');
        cout<<"\n"<<indent<<"ProgramNode( ";
        for(auto &statement : statements){
            statement->print(depth+1);
        }
        cout<<"\n"<<indent<<")";
        cout<<"\n---------------------------------------------------------------\n";
    }
};

struct NumberNode : public Expression{
    double value = 0;
    NumberNode(double value):Expression(NodeType::NumberNode),value(value){}
    void print(int depth) const override{
        string indent (3*depth,' ');
        cout<<"\n"<<indent<<"NumberNode( "<<value<<" )";
    }
};

struct StringNode : public Expression{
    string value = "";
    StringNode(string value):Expression(NodeType::StringNode),value(value){}
    void print(int depth) const override{
        string indent (3*depth,' ');
        cout<<"\n"<<indent<<"StringNode( "<<value<<" )";
    }
};

struct IdentifierNode : public Expression{
    string value = "";
    IdentifierNode(string value) : 
    Expression(NodeType::IdentifierNode),value(value){};
    void print(int depth) const override{
        string indent (3*depth,' ');
        cout<<"\n"<<indent<<"IdentifierNode( "<<value<<" )"; 
    }
};

struct BinaryNode : public Expression{
    shared_ptr<Expression> left;
    shared_ptr<Expression> right;
    string op = "";
    BinaryNode(shared_ptr<Expression> left, shared_ptr<Expression> right, string op) : Expression(NodeType::BinaryNode), left(left),right(right),op(op){}
    void print(int depth) const override{
        string indent (3*depth,' ');
        cout<<"\n"<<indent<<"BinaryNode( ";
        left->print(depth+1);
        cout<<" "<<op<<" ";
        right->print(depth+1);
        cout<<"\n"<<indent<<")";
    }
};

struct VariableDeclarationNode : public Statement{
    bool IsConstant = false;
    string name = "";
    shared_ptr<Expression> value;
    VariableDeclarationNode(string name, shared_ptr<Expression> value, bool cons) : Statement(NodeType::VariableDeclarationNode), name(name),value(value),IsConstant(cons){}
    void print(int depth) const override{
        string indent (3*depth,' ');
        cout<<"\n"<<indent<<"VariableDeclaration( ' "<<name<<" '";
        value->print(depth+1);
        cout<<"\n"<<indent<<")";
    }
};

struct VariableAssignmentNode : public Expression{
    shared_ptr<Expression> assignmentVariable;
    shared_ptr<Expression> value;
    VariableAssignmentNode(shared_ptr<Expression> assignmVar, shared_ptr<Expression> value) : Expression(NodeType::VariableAssignmentNode), assignmentVariable(assignmVar),value(value){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"VariableAssignmentNode( ";
        assignmentVariable->print(depth+1);
        value->print(depth+1);
        cout<<"\n"<<indent<<" )";
    }
};

struct PrintNode : public Statement{
    shared_ptr<Expression> value;
    PrintNode(shared_ptr<Expression> value) : Statement(NodeType::PrintNode), value(value){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"PrintNode( ";
        value->print(depth+1);
        cout<<"\n"<<indent<<" )";
    }
};

struct ConditionalNode : public Expression{
    shared_ptr<Expression> left;
    shared_ptr<Expression> right;
    string conditionOperator = "";
    ConditionalNode(shared_ptr<Expression> left, shared_ptr<Expression> right, string conditionOperator) : Expression(NodeType::ConditionalNode), left(left),right(right),conditionOperator(conditionOperator){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"ConditionalNode( ";
        left->print(depth+1);
        cout<<" "<<conditionOperator<<" ";
        right->print(depth+1);
        cout<<"\n"<<indent<<")";
    }
};

struct IfNode : public Statement{
    shared_ptr<Expression> condition;
    vector<shared_ptr<Statement>> ifBody;
    vector<shared_ptr<Statement>> elseBody;
    IfNode(shared_ptr<Expression> condition, vector<shared_ptr<Statement>> ifBody) : Statement(NodeType::IfNode), condition(condition),ifBody(ifBody){}
    IfNode(shared_ptr<Expression> condition, vector<shared_ptr<Statement>> ifBody, vector<shared_ptr<Statement>> elseBody) : Statement(NodeType::IfNode), condition(condition),ifBody(ifBody),elseBody(elseBody){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"IfNode( ";
        condition->print(depth+1);
        cout<<"\n"<<indent<<")";
    }
};

struct ForNode : public Statement{
    shared_ptr<Statement> initializer;
    shared_ptr<Expression> condition;
    shared_ptr<Statement> increment;
    vector<shared_ptr<Statement>> forBody;
    ForNode(shared_ptr<Statement> Initializer, shared_ptr<Expression> Condition, shared_ptr<Statement> Increment, vector<shared_ptr<Statement>> ForBody) : Statement(NodeType::ForNode), initializer(Initializer), condition(Condition), increment(Increment), forBody(ForBody){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"ForNode( ";
        initializer->print(depth+1);
        condition->print(depth+1);
        increment->print(depth+1);
        cout<<"\n"<<indent<<indent<<"ForBody:";
        for(auto &statement : forBody){
            statement->print(depth+3);
        }
        cout<<"\n"<<indent<<")";
    }
};

struct InputNode : public Statement{
    string variableName = "";
    string inputType = "";
    InputNode(string variableName, string inputType) : Statement(NodeType::InputNode), variableName(variableName), inputType(inputType){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"InputNode( "<<variableName<<" , "<<inputType<<" )";
    }
};

struct FunctionDeclarationNode : public Statement{
    string functionName = "";
    vector<string> parameters;
    vector<shared_ptr<Statement>> functionBody;
    FunctionDeclarationNode(string functionName, vector<string> parameters, vector<shared_ptr<Statement>> functionBody) : Statement(NodeType::FunctionDeclarationNode), functionName(functionName), parameters(parameters), functionBody(functionBody){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"FunctionNode(";
        cout<<"\n"<<indent<<indent<<"(Name:"<<functionName<<")";
        cout<<"\n"<<indent<<indent<<"(Parameters: ";
        for(auto &parameter : parameters){
            cout<<parameter<<", ";
        }
        cout<<")";
        cout<<"\n"<<indent<<indent<<"(Body: ";
        for(auto &statement : functionBody){
            statement->print(depth+3);
        }
        cout<<"\n"<<indent<<indent<<")";
        cout<<"\n"<<indent<<")";
    }
};

struct CallNode : public Expression{
    string functionName = "";
    vector<shared_ptr<Expression>> arguments;

    CallNode(string functionName, vector<shared_ptr<Expression>> arguments) : Expression(NodeType::CallNode), functionName(functionName), arguments(arguments){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"CallNode( (Name:"<<functionName<<") (Arguments: ";
        for(auto &argument : arguments){
            argument->print(depth+1);
        }
        cout<<"\n"<<indent<<")";
    }
};

struct ReturnNode : public Expression{
    shared_ptr<Expression> returnValueExpression;
    ReturnNode(shared_ptr<Expression> returnValueExpression) : Expression(NodeType::ReturnNode), returnValueExpression(returnValueExpression){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"ReturnNode( ";
        returnValueExpression->print(depth+1);
        cout<<"\n"<<indent<<")";
    }
};

struct WhileNode : public Statement{
    shared_ptr<Expression> condition;
    vector<shared_ptr<Statement>> whileBody;
    WhileNode(shared_ptr<Expression> condition, vector<shared_ptr<Statement>> whileBody) : Statement(NodeType::WhileNode), condition(condition), whileBody(whileBody){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"WhileNode( ";
        condition->print(depth+1);
        cout<<"\n"<<indent<<indent<<"WhileBody:";
        for(auto &statement : whileBody){
            statement->print(depth+3);
        }
        cout<<"\n"<<indent<<")";
    }
};

struct ArrayNode : public Expression{
    string arrayName = "";
    vector<shared_ptr<Expression>> arrayBody;
    shared_ptr<Expression> size;
    ArrayNode(string arrayName, vector<shared_ptr<Expression>> arrayBody) : Expression(NodeType::ArrayNode), arrayName(arrayName) ,arrayBody(arrayBody){
        size = make_shared<NumberNode>(arrayBody.size());
    }
    ArrayNode(string arrayName, vector<shared_ptr<Expression>> arrayBody, shared_ptr<Expression> size) : Expression(NodeType::ArrayNode), arrayName(arrayName) ,arrayBody(arrayBody), size(size){}
    void print(int depth) const override{
        string indent(3*depth, ' ');
        cout<<"\n"<<indent<<"ArrayLiteral( ("<< arrayName<<")";
        if(size->node == NodeType::NumberNode){
            cout<<"\n"<<indent<<indent<<"Size:"<<dynamic_pointer_cast<NumberNode>(size)->value;
        }else if(size->node == NodeType::IdentifierNode){
            cout<<"\n"<<indent<<indent<<"Size:"<<dynamic_pointer_cast<IdentifierNode>(size)->value;
        }else if(size->node == NodeType::BinaryNode){
            cout<<"\n"<<indent<<indent<<"Size:";
            dynamic_pointer_cast<BinaryNode>(size)->print(depth+3);
        }else{
            cout<<"\n"<<indent<<"ArrayLiteral( ("<< arrayName<<")";
            cout<<"\n"<<indent<<indent<<"Size: Unknown";
        }
        cout<<"\n"<<indent<<indent<<"ArrayBody:";
        for(auto &index : arrayBody){
            index->print(depth+3);
        }
        cout<<"\n"<<indent<<")";
    }
};

struct ArrayCallNode: public Expression{
    string arrayName = "";
    shared_ptr<Expression> index;
    ArrayCallNode(string arrayName, shared_ptr<Expression>  index) : Expression(NodeType::ArrayCallNode), arrayName(arrayName), index(index){}
    void print(int depth) const override{
        string indent(3*depth, ' ');
        cout<<"\n"<<indent<<"ArrayCall( ("<< arrayName<< ")";
        cout<<"\n   "<<indent<<"Index(";
        index->print(depth+2);
        cout<<"\n   "<<indent<<")";
        cout<<"\n"<<indent<<")";
    }
};

struct DoWhileNode : public Statement{
    vector<shared_ptr<Statement>> doBody;
    shared_ptr<Expression> condition;
    DoWhileNode(vector<shared_ptr<Statement>> doBody, shared_ptr<Expression> condition) : Statement(NodeType::DoWhileNode), doBody(doBody), condition(condition){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"DoWhileNode( ";
        cout<<"\n"<<indent<<indent<<"DoBody:";
        for(auto &statement : doBody){
            statement->print(depth+3);
        }
        cout<<"\n"<<indent<<indent<<"Condition:";
        condition->print(depth+3);
        cout<<"\n"<<indent<<")";
    }
};

#endif