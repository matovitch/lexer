#include "lexer.hpp"

#include "reader.hpp"
#include "token.hpp"
#include "index.hpp"
#include "file.hpp"

#include <vector>
#include <string>

namespace lexer
{

bool isWhiteSpace(char c)
{
    return (c == ' '  ||
            c == '\t' ||
            c == '\r' ||
            c == '\n');
}

bool isNum(char c)
{
    return (c >= '0' && 
            c <= '9');
}

bool isExponant(char c)
{
    return (c == 'e' || 
            c == 'E');
}

bool isPlusOrMinus(char c)
{
    return (c == '+' ||
            c == '-');
}

bool isDot(char c)
{
    return c == '.';
}

bool isNewLine(char c)
{
    return c == '\n';
}

bool isAlphaOrUnderscore(char c)
{
    return ((c >= 'a'   && 
             c <= 'z')  ||
            (c >= 'A'   &&
             c <= 'Z')) ||
             c == '_';
}

bool isAlphaNumOrUnderscore(char c)
{
    return (isAlphaOrUnderscore(c) ||
            isNum              (c));
}

bool matchKeyword(Reader& reader, const std::string& keyword)
{
    reader.capture();

    if (!reader.match(keyword))
    {
        return false;
    }

    if (isAlphaNumOrUnderscore(reader.asChar()))
    {
        reader.restore();
        return false;
    }

    return true;
}

bool matchComment(Reader& reader, const File& file)
{
    if (reader.match("//"))
    {
        do
        {
            reader += 1;
        }
        while (!isNewLine(reader.asChar()) && 
                reader::isValid(reader, file));

        return true;
    }

    if (reader.match("/*"))
    {
        do
        {
            reader += 1;
        }
        while (!reader.match("*/") && 
                reader::isValid(reader, file));
            
            return true;
    }

    return false;
}

bool matchWhiteSpace(Reader& reader, const File& file)
{
    if (!isWhiteSpace(reader.asChar()))
    {
        return false;
    }

    do
    {
       reader += 1;
    }
    while (isWhiteSpace(reader.asChar()) &&
           reader::isValid(reader, file));

    return true;
}

bool matchIdentifier(Reader& reader)
{
    if (!isAlphaOrUnderscore(reader.asChar()))
    {
        return false;
    }

    do
    {
       reader += 1;
    }
    while (isAlphaNumOrUnderscore(reader.asChar()));

    return true;
}

bool matchInteger(Reader& reader)
{
    if (!isNum(reader.asChar()))
    {
        return false;
    }

    reader.capture();

    do
    {
       reader += 1;
    }
    while (isNum(reader.asChar()));

    if (isDot(reader.asChar()))
    {
        reader.restore();
        return false;
    }

    return true;
}

bool matchFloatting(Reader& reader)
{
    char currentChar = reader.asChar();

    if (isNum(currentChar) || isDot(currentChar))
    {
        if (isDot(currentChar))
        {
            reader.capture();

            reader += 1;

            if (!isNum(reader.asChar()))
            {
                reader.restore();
                return false;
            }

            do
            {
               reader += 1;
            }
            while (isNum(reader.asChar()));
        }

        if (isNum(currentChar))
        {
            reader.capture();

            do
            {
               reader += 1;
            }
            while (isNum(reader.asChar()));

            if (!isDot(reader.asChar()))
            {
                reader.restore();
                return false;
            }

            do
            {
                reader += 1;
            }
            while (isNum(reader.asChar()));
        }

        reader.capture();

        if (isExponant(reader.asChar()))
        {
            reader += 1;
        }

        currentChar = reader.asChar();

        if (!isPlusOrMinus(currentChar) &&
            !isNum        (currentChar))
        {
            reader.restore();
            return true;
        }

        if (isPlusOrMinus(currentChar))
        {
            reader += 1;

            if (!isNum(reader.asChar()))
            {
                reader.restore();
                return true;
            }
        }

        while (isNum(reader.asChar()))
        {
            reader += 1;
        }

        return true;
    }

    return false;
}

token::Vector makeTokensAndIndex(const File& file, Index& index)
{
    auto&& reader = file.makeReader();

    token::Vector tokens{file, reader, index};

    while (reader::isValid(reader, file))
    {
        if (lexer::matchWhiteSpace(reader, file))
        {
            tokens.pushBack(token::WHITE_SPACE);
            continue;
        }
        if (lexer::matchComment(reader, file))
        {
            tokens.pushBack(token::COMMENT);
            continue;
        }
        if (lexer::matchKeyword(reader, "module"))
        {
            tokens.pushBack(token::MODULE);
            continue;
        }
        if (lexer::matchKeyword(reader, "import"))
        {
            tokens.pushBack(token::IMPORT);
            continue;
        }
        if (lexer::matchKeyword(reader, "return"))
        {
            tokens.pushBack(token::RETURN);
            continue;
        }
        if (lexer::matchKeyword(reader, "alias"))
        {
            tokens.pushBack(token::ALIAS);
            continue;
        }
        if (lexer::matchKeyword(reader, "if"))
        {
            tokens.pushBack(token::IF);
            continue;
        }
        if (lexer::matchKeyword(reader, "then"))
        {
            tokens.pushBack(token::THEN);
            continue;
        }
        if (lexer::matchKeyword(reader, "else"))
        {
            tokens.pushBack(token::ELSE);
            continue;
        }
        if (lexer::matchKeyword(reader, "while"))
        {
            tokens.pushBack(token::WHILE);
            continue;
        }
        if (lexer::matchFloatting(reader))
        {
            tokens.pushBack(token::FLOATTING, index::Status::SAVE_LEXEME);
            continue;
        }
        if (lexer::matchInteger(reader))
        {
            tokens.pushBack(token::INTEGER, index::Status::SAVE_LEXEME);
            continue;
        }
        if (lexer::matchIdentifier(reader))
        {
            tokens.pushBack(token::IDENTIFIER, index::Status::SAVE_LEXEME);
            continue;
        }
        if (reader.match("=="))
        {
            tokens.pushBack(token::C_EQL);
            continue;
        }
        if (reader.match("!="))
        {
            tokens.pushBack(token::C_NEQ);
            continue;
        }
        if (reader.match("&&"))
        {
            tokens.pushBack(token::C_AND);
            continue;
        }
        if (reader.match("||"))
        {
            tokens.pushBack(token::C_OR);
            continue;
        }
        if (reader.match("<="))
        {
            tokens.pushBack(token::LEQ);
            continue;
        }
        if (reader.match(">="))
        {
            tokens.pushBack(token::GEQ);
            continue;
        }
        if (reader.match('('))
        {
            tokens.pushBack(token::L_PARENT);
            continue;
        }
        if (reader.match(')'))
        {
            tokens.pushBack(token::R_PARENT);
            continue;
        }
        if (reader.match('{'))
        {
            tokens.pushBack(token::L_BRACE);
            continue;
        }
        if (reader.match('}'))
        {
            tokens.pushBack(token::R_BRACE);
            continue;
        }
        if (reader.match(';'))
        {
            tokens.pushBack(token::SEMI_COLON);
            continue;
        }
        if (reader.match('.'))
        {
            tokens.pushBack(token::DOT);
            continue;
        }
        if (reader.match('+'))
        {
            tokens.pushBack(token::ADD);
            continue;
        }
        if (reader.match('-'))
        {
            tokens.pushBack(token::SUB);
            continue;
        }
        if (reader.match('*'))
        {
            tokens.pushBack(token::MUL);
            continue;
        }
        if (reader.match('/'))
        {
            tokens.pushBack(token::DIV);
            continue;
        }
        if (reader.match('%'))
        {
            tokens.pushBack(token::MOD);
            continue;
        }
        if (reader.match('<'))
        {
            tokens.pushBack(token::LST);
            continue;
        }
        if (reader.match('>'))
        {
            tokens.pushBack(token::GST);
            continue;
        }
        if (reader.match('='))
        {
            tokens.pushBack(token::EQL);
            continue;
        }
        if (reader.match('&'))
        {
            tokens.pushBack(token::AND);
            continue;
        }
        if (reader.match('!'))
        {
            tokens.pushBack(token::NOT);
            continue;
        }
        if (reader.match('^'))
        {
            tokens.pushBack(token::XOR);
            continue;
        }
        if (reader.match('|'))
        {
            tokens.pushBack(token::OR);
            continue;
        }

        reader += 1;

        tokens.pushBack(token::UKNOWN, index::Status::SAVE_LEXEME);
    }

    return tokens;
}

} // end lexer namespace