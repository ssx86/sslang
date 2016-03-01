#include <iostream>
#include <fstream>

//test

#include "token.h"
#include "lexer.h"


//test

std::istream* GetInput(int argc, char* argv[])
{
    std::istream* p_input_stream = NULL;
    //parse shell arguments
    if(argc > 1)
    {
        //read from a file
        std::ifstream* file_input = new std::ifstream();
        file_input->open(argv[1]);
        p_input_stream = file_input;
    }
    else
    {
        //read from cin
        p_input_stream = &std::cin;
    }

    return p_input_stream;
}

int main(int argc, char* argv[])
{
    std::istream* input = GetInput(argc, argv);
    std::string s;

    Lexer lexer(input);
    bool ret = lexer.Init();
    if(!ret)
        return 1;
    while(Token* token = lexer.GetToken())
        std::cout << token->tostring();
    std::cout << std::endl;

    return 0;
}





