#ifndef AST_H_v1
#define AST_H_v1

#include "iostream"
#include "vector"
#include "string"
#include "memory"

using namespace std;

enum class NodeType{
    ProgramNode,
    NumberNode,
    StringNode,
    IdentifierNode,
    BinaryNode,
    VariableDeclarationNode,
    VariableAssignmentNode
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
    string op;
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
    string name;
    shared_ptr<Expression> value;
    VariableDeclarationNode(string name, shared_ptr<Expression> value, bool cons) : Statement(NodeType::VariableDeclarationNode), name(name),value(value),IsConstant(cons){}
    void print(int depth) const override{
        string indent (3*depth,' ');
        cout<<"\n"<<indent<<"VariableDecleration( ' "<<name<<" '";
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


#endif