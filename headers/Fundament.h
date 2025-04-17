#ifndef FUNDAMENT_H_v1
#define FUNDAMENT_H_v1

#include "Reader.h"
#include "Parser.h"
#include "Interpreter.h"

class Fundament{
    private:
    public:
        Fundament(){
            shared_ptr<Environment> environment = make_shared<Environment>();
            Reader reader;
            Parser parser;
            Program program;
            Interpreter interpreter;

            environment->initEnvironment();

            program=parser.produceAST(reader.readFile("../test.txt"));
            program.print(1); 

            shared_ptr<R_Value> lastResult = make_shared<NullValue>(); 
            lastResult = interpreter.evaluate(make_shared<Program>(program),environment);

            cout<<"\n--------------------------- Values ---------------------------\n";
            lastResult->print();
            cout<<"\n\n--------------------------------------------------------------\n";  
        }
};






#endif