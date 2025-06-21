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
    ConditionalNode,
    IfNode,
    ForNode,
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

#endif