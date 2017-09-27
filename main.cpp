
#include "file_map.hpp"
#include "location.hpp"
#include "lexer.hpp"
#include "index.hpp"
#include "token.hpp"
#include "file.hpp"

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <string>


int main()
{
    File file{"data/test.txt"};

    Index index;

    const auto& fileMap = file.craftFileMap();

    auto&& tokens = lexer::craftTokensAndIndex(file, index);

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
