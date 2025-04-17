#ifndef ENVIRONMENT_H_v1
#define ENVIRONMENT_H_v1


#include "iostream"
#include "string"
#include "vector"
#include "memory"
#include "unordered_map"
#include "set"
#include "Values.h"

using namespace std;

class Environment;

class Environment;
void setupScope(shared_ptr<Environment> env) ;

class Environment:public std::enable_shared_from_this<Environment>{
    private:
        unordered_map<string, shared_ptr<R_Value>> variables;
        set<string> constantVariablesNames;
        shared_ptr<Environment> parentEnvironment;

        void initGlobalEnvironment(){
            setupScope(shared_from_this());
        }

    public:

        Environment(shared_ptr<Environment> parentEnv= nullptr ): parentEnvironment(parentEnv),variables(),constantVariablesNames(){}

        void initEnvironment(){
            initGlobalEnvironment();
        }

        shared_ptr<R_Value> declareVariable(const string& varName, shared_ptr<R_Value> value, bool isConst=false){
            if(variables.find(varName) != variables.end()){
                cerr<<"[[Stage]] : Environment  [[ERROR]] : Variable with name '" << varName << "' already declared.";
                exit(1);
            }
            variables[varName] = value;
            if(isConst){
                constantVariablesNames.insert(varName);
            }
            return value;
        }

        shared_ptr<R_Value> assignVariable(const string& varName, shared_ptr<R_Value> value){
            shared_ptr<Environment> env = resolve(varName);
            if(env->constantVariablesNames.find(varName)!= env->constantVariablesNames.end()){
                cerr<<"[[Stage]] : Environment  [[ERROR]] : Variable with name '" << varName << "' is constant and cannot be assigned.";
                exit(1);
            } 
            env->variables[varName] = value;
            return value;
        }

        shared_ptr<R_Value> lookupVariable(string varname) {
            shared_ptr<Environment> env = resolve(varname);
            return env->variables[varname];
        }

        shared_ptr<Environment> resolve(string varname) {
            if(variables.find(varname) != variables.end()) {
                return make_shared<Environment>(*this);
            }
            if(parentEnvironment == nullptr) {
                cerr << "\n[[Stage]] : Environment  [[ERROR]] : Variable not defined ---- Variable : " << varname << "\n";
                exit(1);
            }
            return parentEnvironment->resolve(varname);
        }

};

void setupScope(shared_ptr<Environment> enviroment){
    enviroment->declareVariable("null", makeNullValue(), true);
    enviroment->declareVariable("true", makeBoolValue(true), true);  
    enviroment->declareVariable("false", makeBoolValue(false), true);
}


#endif