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

namespace lexer
{

token::Vector makeTokenVector(const File& file);

} // end lexer namespace

#endif // end __LEXER_H__