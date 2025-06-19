#ifndef AST_H_v1
#define AST_H_v1

#include "iostream"
#include "vector"
#include "string"
#include "memory"
#include <memory>

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
    PropertyNode,
    ObjectNode,
    MemberNode,
    CallNode,
    FunctionDeclarationNode
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

struct PropertyNode : public Expression{
    string key = "";
    shared_ptr<Expression> value;
    PropertyNode(string key, shared_ptr<Expression> value) : Expression(NodeType::PropertyNode), key(key),value(value){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"PropertyNode( Key= '"<<key<<"' ";
        value->print(depth+1);
        cout<<"\n"<<indent<<" )";
    }
};

struct ObjectNode : public Expression{
    vector<shared_ptr<PropertyNode>> properties;
    ObjectNode(vector<shared_ptr<PropertyNode>> properties) : Expression(NodeType::ObjectNode), properties(properties){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"ObjectNode( ";
        for(auto &property : properties){
            property->print(depth+1);
        }
        cout<<"\n"<<indent<<" )";
    }
};

struct MemberNode : public Expression{
    shared_ptr<Expression> object;
    shared_ptr<Expression> property;
    bool computed = false;
    MemberNode(shared_ptr<Expression> object, shared_ptr<Expression> property, bool computed) : Expression(NodeType::MemberNode), object(object),property(property),computed(computed){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"MemberNode( computed = "<<computed;
        object->print(depth+1);
        property->print(depth+1);
        cout<<"\n"<<indent<<" )";
    }
};

struct CallNode : public Expression{
    shared_ptr<Expression> callee;
    vector<shared_ptr<Expression>> arguments;
    CallNode(shared_ptr<Expression> callee, vector<shared_ptr<Expression>> arguments) : Expression(NodeType::CallNode), callee(callee),arguments(arguments){}
    void print(int depth) const override{
        string indent(3*depth,' ');
        cout<<"\n"<<indent<<"FunctionCallNode( ";
        callee->print(depth+1);
        for(auto &argument : arguments){
            argument->print(depth+1);
        }
        cout<<"\n"<<indent<<" )";
    }
};

struct FunctionDeclarationNode : public Statement{
    string name = "";
    vector<string> parameters;
    shared_ptr<Statement> body;
};

#endif