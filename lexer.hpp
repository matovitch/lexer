#ifndef __LEXER_H__
#define __LEXER_H__

#include <vector>
#include <string>

class Location;
class Reader;
class Token;
class Index;
class File;

namespace lexer
{

bool matchKeyword(Reader& reader, const std::string& keyword);

bool matchComment(Reader& reader, const File& file);

bool matchWhiteSpace(Reader& reader, const File& file);

bool matchIdentifier(Reader& reader);

bool matchFloatting(Reader& reader);

bool matchInteger(Reader& reader);

Token craftAndSaveToken(Index&             index,
                        uint8_t            tokenValue, 
                        std::size_t        tokenOffset, 
                        const std::string& lexeme);

std::vector<Token> craftTokensAndIndex(const File& file, Index& index);

const std::string& searchLexeme(const Token& token, 
                                const Index& index);

Location searchLocation(const Token& token, 
                        const File& file);

} // end lexer namespace

#endif // end __LEXER_H__