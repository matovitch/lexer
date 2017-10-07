#include "index.hpp"

#include "token.hpp"

#include <cstdint>
#include <string>

Index::Index() : _size{1} {}

std::size_t Index::findOrSave(const std::string& lexeme)
{
    const auto& fit = _backward.find(lexeme);

    if (fit != _backward.end())
    {
        return fit->second;
    }

    _backward [ lexeme ] = _size  ;
    _forward  [ _size  ] = lexeme ;

    return _size++;
}

const std::string& Index::searchLexeme(const std::size_t lexemeID, 
                                       const uint8_t     word) const
{
    const auto& fit = _forward.find(lexemeID);

    if (fit != _forward.end())
    {
        return fit->second;
    }

    return token::AS_LEXEME[word];
}