#ifndef __INDEX_H__
#define __INDEX_H__

#include <unordered_map>
#include <string>

static const std::string EMPTY_STRING = "";

class Index
{
    friend Token lexer::craftAndSaveToken(Index&             index,
                                          uint8_t            tokenValue, 
                                          std::size_t        tokenOffset, 
                                          const std::string& lexeme);

    friend const std::string& lexer::searchLexeme(const Token& token, 
                                                  const Index& index);
public:

    Index() : _size{0} {}

private:

    std::size_t _size;

    std::unordered_map<std::size_t, std::string> _forward;
    std::unordered_map<std::string, std::size_t> _backward;
};


#endif