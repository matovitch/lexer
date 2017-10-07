
#include "file_map.hpp"
#include "location.hpp"
#include "lexer.hpp"
#include "index.hpp"
#include "token.hpp"
#include "error.hpp"
#include "file.hpp"

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <string>

int main(int argc, char** argv)
{
    std::vector<std::string> args;

    for (int i = 1; i < argc; i++)
    {
        args.emplace_back(argv[i]);
    }

    std::vector<token::Vector> tokenVectors;

    for (const auto& filePath : args)
    {
        auto&& [file, error] = file::make(filePath);

        if (!error)
        {
            tokenVectors.emplace_back
            (
                lexer::makeTokenVector(file)
            );

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

        auto&& tokenVector = tokenVectors.back();

        std::cout << tokenVector << std::endl;

        // Trim white spaces and comments before parsing 
        tokenVector.trim
        (
            []
            (const Token& token)
            { 
                return token == token::WHITE_SPACE ||
                       token == token::COMMENT;
            }
        );
    }

    return EXIT_SUCCESS;
}
