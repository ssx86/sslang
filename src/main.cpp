#include <iostream>
#include <fstream>

//test

#include "token.h"


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

    std::istream& input = *p_input_stream;
}

int main(int argc, char* argv[])
{
    std::istream* input = GetInput(argc, argv);
    Token token(1);
    Token token1(Token::L_PAREN);
    Token token2(Token::STRING, std::string("hehe"));

    return 0;
}





