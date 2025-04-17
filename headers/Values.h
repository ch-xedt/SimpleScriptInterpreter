#ifndef VALUES_H_v1
#define VALUES_H_v1

#include "iostream"
#include "string"
#include "cmath"

using namespace std;

enum class ValueType{
    NullValue,
    NumberValue,
    StringValue,
    BoolValue
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

shared_ptr<R_Value> makeNullValue(){
    return make_shared<NullValue>();
}
shared_ptr<R_Value> makeNumberValue(double val){
    return make_shared<NumberValue>(val);  
}
shared_ptr<R_Value> makeStringValue(string val){
    return make_shared<StringValue>(val); 
}
shared_ptr<R_Value> makeBoolValue(bool val){
    return make_shared<BoolValue>(val);
}



#endif