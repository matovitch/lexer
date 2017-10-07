#ifndef __LEXER_H__
#define __LEXER_H__

#include "location.hpp"
#include "reader.hpp"
#include "index.hpp"
#include "file.hpp"

#include <string>

namespace token
{

class Vector;

} // end token namespace

class Lexer
{

public:
    
    void lex(const File& file);

private:

    Index _index;

public:

    std::vector<token::Vector> tokenVectors;
};

#endif // end __LEXER_H__