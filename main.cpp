
#include "location.hpp"
#include "lexer.hpp"
#include "index.hpp"
#include "token.hpp"
#include "file.hpp"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>

int main()
{
    File file{"data/test.txt"};

    Index index;

    const auto& tokens = lexer::craftTokensAndIndex(file, index);

    for (const auto& token : tokens)
    {
        const auto& location = lexer::searchLocation(token, file);

        std::cout << std::left                          << std::setw(12)
                  << token.asString()     << std::right << std::setw (2)
                  << location.line << ':' << std::right << std::setw (2)
                  << location.col         << std::right << std::setw (4);

        const auto& lexeme = lexer::searchLexeme(token, index);

        if (!lexeme.empty())
        {
            std::cout << "(\"" << lexeme << "\")";
        }
    
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
