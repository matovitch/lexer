
#include "lexer.hpp"
#include "token.hpp"
#include "error.hpp"
#include "file.hpp"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

std::vector<std::string> makeArgs(int argc, char** argv)
{
    std::vector<std::string> args;
    
    for (int i = 1; i < argc; i++)
    {
        args.emplace_back(argv[i]);
    }

    return args;
}

int main(int argc, char** argv)
{
    const auto& args = makeArgs(argc, argv);

    Lexer lexer;

    for (const auto& filePath : args)
    {
        auto&& [file, error] = file::make(filePath);

        if (!error)
        {
            lexer.lex(file);
            file.freeBuffer(); // save some memory
        }
        else if (error == Error::FILE_NOT_FOUND)
        {
            std::cerr << "Error: Could not find the file '" << filePath << "'." << std::endl;
            return EXIT_FAILURE;
        }
        else if (error == Error::FILE_NOT_REGULAR)
        {
            std::cerr << "Error: '" << filePath << "' is not a regular file." << std::endl;
            return EXIT_FAILURE;
        }
        else
        {
            std::cerr << "Unkown error." << std::endl;
            return EXIT_FAILURE;            
        }
    }

    for (auto&& tokenVector : lexer.tokenVectors)
    {
        std::cout << tokenVector << std::endl;

        // Trim white spaces and comments before parsing 
        tokenVector.trim
        (
            []
            (const Token token)
            { 
                return token == token::WHITE_SPACE ||
                       token == token::COMMENT;
            }
        );
    }

    return EXIT_SUCCESS;
}
