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

bool matchKeyword(Reader& reader, const std::string& keyword);

bool matchComment(Reader& reader, const File& file);

bool matchWhiteSpace(Reader& reader, const File& file);

bool matchIdentifier(Reader& reader);

bool matchFloatting(Reader& reader);

bool matchInteger(Reader& reader);

token::Vector craftTokensAndIndex(const File& file, Index& index);

} // end lexer namespace

#endif // end __LEXER_H__