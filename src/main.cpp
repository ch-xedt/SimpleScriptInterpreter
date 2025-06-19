#ifndef MAIN_CPP
#define MAIN_CPP

#include "../headers/Fundament.h"
#include "filesystem"


int main(int argc, char* argv[]){
    std::filesystem::path filename = argv[1];
    if(filename.extension() != ".cael"){
        cerr<<"\n\n[[ERROR]]: Invalid file, expected .cael file\n\n";
        exit(1);
    }
    Fundament f(argv[1]);
}

#endif

