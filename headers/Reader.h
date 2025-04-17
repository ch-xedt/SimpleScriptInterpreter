#ifndef READER_H_v1
#define READER_H_v1

#include "iostream"
#include "string"
#include "fstream"

using namespace std;

class Reader{
    private:
    public:
        string readFile(string filepath){
            ifstream file(filepath);
            string content = "";
            string line = "";
            while(getline(file,line)){
                content += line;
            }
            file.close();
            return content;
        }
};


#endif