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

const std::string& searchLexeme(const Token& token, 
                                const Index& index)
{
    const auto& forwardIndex = index._forward;

    const auto& fit = forwardIndex.find(token.lexeme);

    if (fit != forwardIndex.end() &&
        !((fit->second).empty()))
    {
        return fit->second;
    }

    return token::AS_LEXEME[token.value];
}

Location searchLocation(const Token& token, 
                        const File& file)
{
    const auto& lines = file._lines;

    const std::size_t offset = token.offset;

    const auto& fit = std::lower_bound(lines.begin(),
                                       lines.end(),
                                       offset);
    return { 
               static_cast<std::size_t>(fit - lines.begin()),
               static_cast<std::size_t>(offset - *(fit - 1))
           };
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

Token craftAndSaveToken(Index&             index,
                        uint8_t            tokenValue, 
                        std::size_t        tokenOffset, 
                        const std::string& lexeme)
{
    const auto& craftToken = 
        [
            tokenValue,
            tokenOffset
        ]
        ( 
            std::size_t tokenLexeme 
        )
        {
            return Token
                   {
                       tokenValue,
                       tokenOffset,
                       tokenLexeme
                   };
        };

    auto&& backwardIndex = index._backward ;
    auto&& forwardIndex  = index._forward  ;

    const auto& fit = backwardIndex.find(lexeme);

    std::size_t tokenLexeme;

    if (fit != backwardIndex.end())
    {
        return craftToken(fit->second);
    }

    backwardIndex [ lexeme      ] = index._size ;
    forwardIndex  [ index._size ] = lexeme      ;

    return craftToken(index._size++);
}

enum IndexStatus
{
    DROP_LEXEME,
    SAVE_LEXEME
};

std::vector<Token> craftTokensAndIndex(const File& file, Index& index)
{
    auto&& reader = file.craftReader();

    std::vector<Token> tokens;

    while (reader::isValid(reader, file))
    {
        const uint8_t* const base = reader();

        const auto& craftToken = 
            [
                &reader,
                &index,
                &file, 
                base
            ]
            (
                uint8_t token, 
                IndexStatus indexStatus = DROP_LEXEME
            )
            {
                const uint8_t* const ptr = (indexStatus == DROP_LEXEME) ? reader()
                                                                        : base;

                return craftAndSaveToken
                    (
                        index,
                        token,
                        reader::offset(reader, file), 
                        std::string
                        {
                            reinterpret_cast<const char*>(base),
                            static_cast<std::size_t>(reader() - ptr)
                        }
                    );
            };

        if (lexer::matchWhiteSpace(reader, file))
        {
            tokens.push_back(craftToken(Token::WHITE_SPACE));
            continue;
        }
        if (lexer::matchComment(reader, file))
        {
            tokens.push_back(craftToken(Token::COMMENT));
            continue;
        }
        if (lexer::matchKeyword(reader, "module"))
        {
            tokens.push_back(craftToken(Token::IMPORT));
            continue;
        }
        if (lexer::matchKeyword(reader, "import"))
        {
            tokens.push_back(craftToken(Token::IMPORT));
            continue;
        }
        if (lexer::matchKeyword(reader, "return"))
        {
            tokens.push_back(craftToken(Token::RETURN));
            continue;
        }
        if (lexer::matchKeyword(reader, "alias"))
        {
            tokens.push_back(craftToken(Token::ALIAS));
            continue;
        }
        if (lexer::matchKeyword(reader, "if"))
        {
            tokens.push_back(craftToken(Token::IF));
            continue;
        }
        if (lexer::matchKeyword(reader, "then"))
        {
            tokens.push_back(craftToken(Token::THEN));
            continue;
        }
        if (lexer::matchKeyword(reader, "else"))
        {
            tokens.push_back(craftToken(Token::ELSE));
            continue;
        }
        if (lexer::matchKeyword(reader, "while"))
        {
            tokens.push_back(craftToken(Token::WHILE));
            continue;
        }
        if (lexer::matchFloatting(reader))
        {
            tokens.push_back(craftToken(Token::FLOATTING, SAVE_LEXEME));
            continue;
        }
        if (lexer::matchInteger(reader))
        {
            tokens.push_back(craftToken(Token::INTEGER, SAVE_LEXEME));
            continue;
        }
        if (lexer::matchIdentifier(reader))
        {
            tokens.push_back(craftToken(Token::IDENTIFIER, SAVE_LEXEME));
            continue;
        }
        if (reader.match("=="))
        {
            tokens.push_back(craftToken(Token::C_EQL));
            continue;
        }
        if (reader.match("!="))
        {
            tokens.push_back(craftToken(Token::C_NEQ));
            continue;
        }
        if (reader.match("&&"))
        {
            tokens.push_back(craftToken(Token::C_AND));
            continue;
        }
        if (reader.match("||"))
        {
            tokens.push_back(craftToken(Token::C_OR));
            continue;
        }
        if (reader.match("<="))
        {
            tokens.push_back(craftToken(Token::LEQ));
            continue;
        }
        if (reader.match(">="))
        {
            tokens.push_back(craftToken(Token::GEQ));
            continue;
        }
        if (reader.match('('))
        {
            tokens.push_back(craftToken(Token::L_PARENT));
            continue;
        }
        if (reader.match(')'))
        {
            tokens.push_back(craftToken(Token::R_PARENT));
            continue;
        }
        if (reader.match('{'))
        {
            tokens.push_back(craftToken(Token::L_BRACE));
            continue;
        }
        if (reader.match('}'))
        {
            tokens.push_back(craftToken(Token::R_BRACE));
            continue;
        }
        if (reader.match(';'))
        {
            tokens.push_back(craftToken(Token::SEMI_COLON));
            continue;
        }
        if (reader.match('.'))
        {
            tokens.push_back(craftToken(Token::DOT));
            continue;
        }
        if (reader.match('+'))
        {
            tokens.push_back(craftToken(Token::ADD));
            continue;
        }
        if (reader.match('-'))
        {
            tokens.push_back(craftToken(Token::SUB));
            continue;
        }
        if (reader.match('*'))
        {
            tokens.push_back(craftToken(Token::MUL));
            continue;
        }
        if (reader.match('/'))
        {
            tokens.push_back(craftToken(Token::DIV));
            continue;
        }
        if (reader.match('%'))
        {
            tokens.push_back(craftToken(Token::MOD));
            continue;
        }
        if (reader.match('<'))
        {
            tokens.push_back(craftToken(Token::LST));
            continue;
        }
        if (reader.match('>'))
        {
            tokens.push_back(craftToken(Token::GST));
            continue;
        }
        if (reader.match('='))
        {
            tokens.push_back(craftToken(Token::EQL));
            continue;
        }
        if (reader.match('&'))
        {
            tokens.push_back(craftToken(Token::AND));
            continue;
        }
        if (reader.match('!'))
        {
            tokens.push_back(craftToken(Token::NOT));
            continue;
        }
        if (reader.match('^'))
        {
            tokens.push_back(craftToken(Token::XOR));
            continue;
        }
        if (reader.match('|'))
        {
            tokens.push_back(craftToken(Token::OR));
            continue;
        }

        reader += 1;

        tokens.push_back(craftToken(Token::UKNOWN, SAVE_LEXEME));
    }

    return tokens;
}

} // end lexer namespace