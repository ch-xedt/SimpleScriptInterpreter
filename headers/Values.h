#ifndef VALUES_H_v1
#define VALUES_H_v1

#include "AstNodes.h"
#include "iostream"
#include "string"
#include "cmath"
#include "memory"
#include "unordered_map"
#include "functional"
#include "algorithm"

using namespace std;

class Environment;

enum class ValueType{
    NullValue,
    NumberValue,
    StringValue,
    BoolValue,
    FunctionValue,
    ArrayValue,
};

struct R_Value{
    ValueType type; 
    R_Value(ValueType valT):type(valT){}
    virtual ~R_Value(){};
    virtual void print() const = 0;
};

struct NullValue:R_Value{
    double value = 0.0;
    NullValue():R_Value(ValueType::NullValue){}
    void print() const override{
        cout<<"\n NullValue ( "<<value<<" )";
    }
};

struct NumberValue:R_Value{
    double value=0;
    NumberValue():R_Value(ValueType::NumberValue){};
    NumberValue(double val):R_Value(ValueType::NumberValue),value(val){}
    void print() const override{
        cout<<"\n NumberValue ( "<<value<<" )"; 
    }
};

struct StringValue:R_Value{
    string value="";
    StringValue():R_Value(ValueType::StringValue){}
    StringValue(string val):R_Value(ValueType::StringValue),value(val){}
    void print() const override{
        cout<<"\n StringValue ( "<<value<<" )"; 
    }
};

struct BoolValue:R_Value{
    bool value = false;
    BoolValue():R_Value(ValueType::BoolValue){}
    BoolValue(bool val):R_Value(ValueType::BoolValue),value(val){}
    void print() const override{
        cout<<"\n BoolValue ( "<<(value == 0 ? "false" : "true")<<" )";
    }
};

struct FunctionValue:R_Value{
    string functionName = "";
    vector<string> parameters;
    vector<shared_ptr<Statement>> body;
    shared_ptr<Environment> env;
    FunctionValue():R_Value(ValueType::FunctionValue){}
    FunctionValue(string name, vector<string> params, vector<shared_ptr<Statement>> bdy, shared_ptr<Environment> env):R_Value(ValueType::FunctionValue),functionName(name),parameters(params),body(bdy),env(env){}
    void print() const override{
        cout<<"\n FunctionValue ( "<<functionName<<" )";
    }
};

struct ArrayValue:R_Value{
    vector<shared_ptr<R_Value>> body;
    ArrayValue(vector<shared_ptr<R_Value>> body):R_Value(ValueType::ArrayValue), body(body){}
    void print() const override{
        cout<<"\n ArrayValue(";
        for(auto &index : body){
            index->print();
            cout<<" , ";
        }
        cout<<" )";
    }
};


inline shared_ptr<R_Value> makeNullValue(){
    return make_shared<NullValue>();
}
inline shared_ptr<R_Value> makeNumberValue(double val){
    return make_shared<NumberValue>(val);  
}
inline shared_ptr<R_Value> makeStringValue(string val){
    return make_shared<StringValue>(val); 
}
inline shared_ptr<R_Value> makeBoolValue(bool val){
    return make_shared<BoolValue>(val);
}
inline shared_ptr<R_Value> makeFunctionValue(string name, vector<string> params, vector<shared_ptr<Statement>> bdy, shared_ptr<Environment> env){
    return make_shared<FunctionValue>(name, params, bdy, env);
}


#endif