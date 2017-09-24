#include "token.hpp"

#include <string>

const std::string& Token::asString() const
{
    return token::AS_STRING[value];
}

Token::operator uint8_t() const 
{ 
    return value; 
}
