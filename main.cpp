
#include "file_map.hpp"
#include "location.hpp"
#include "lexer.hpp"
#include "index.hpp"
#include "token.hpp"
#include "error.hpp"
#include "file.hpp"

#include <system_error>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <string>


int main()
{
    const auto& [file, error] = file::make("data/test.txt");

    if (!error)
    {
        Index index;

        const auto& fileMap = file.makeFileMap();

        auto&& tokens = lexer::makeTokensAndIndex(file, index);

        for (std::size_t i = 0; i < tokens.size(); i++)
        {
            const auto& word     = tokens.words[i];
            const auto& location = fileMap .searchLocation (tokens.offsets[i]);
            const auto& lexeme   = index   .searchLexeme   (tokens.lexemeIDs[i], word);

            std::cout                          << std::left  << std::setw(12)
                    << token::asString(word) << std::right << std::setw (2)
                    << location.line << ':'  << std::right << std::setw (2)
                    << location.col          << std::right << std::setw (4);

            if (!lexeme.empty())
            {
                std::cout << "(\"" << lexeme << "\")";
            }
        
            std::cout << std::endl;
        }

        // Trim white spaces and comments before parsing.
        tokens.trim
        (
            []
            (const Token& token)
            { 
                return token == token::WHITE_SPACE ||
                       token == token::COMMENT;
            }
        );

        //start parsing here...

        return EXIT_SUCCESS;
    }
    else if (error == Error::FILE_NOT_FOUND)
    {
        std::cerr << "Could not find the file." << std::endl;
    }
    else if (error == Error::FILE_NOT_REGULAR)
    {
        std::cerr << "The file provided, is not a regular file." << std::endl;
    }
    else
    {
        std::cerr << "Unkown error." << std::endl;
    }

    return EXIT_FAILURE;
}
