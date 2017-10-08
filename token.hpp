#ifndef __TOKEN_VECTOR_H__
#define __TOKEN_VECTOR_H__

#include "file_map.hpp"
#include "reader.hpp"
#include "index.hpp"
#include "file.hpp"

#include <cstdint>
#include <string>
#include <vector>

using Token = uint8_t;

namespace token
{

enum : Token
{
    COMMENT,
    WHITE_SPACE,
    IDENTIFIER,
    FLOATTING,
    INTEGER,
    IMPORT,
    MODULE,
    RETURN,
    ALIAS,
    IF,
    THEN,
    ELSE,
    WHILE,
    L_PARENT,
    R_PARENT,
    L_BRACE,
    R_BRACE,
    SEMI_COLON,
    DOT,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    LST,
    LEQ,
    GST,
    GEQ,
    EQL,
    AND,
    OR,
    XOR,
    NOT,
    C_EQL,
    C_NEQ,
    C_AND,
    C_OR,
    UKNOWN,
    END_OF_FILE
};

static const std::string AS_STRING[] =
    {
        "COMMENT",
        "WHITE_SPACE",
        "IDENTIFIER",
        "FLOATTING",
        "INTEGER",
        "IMPORT",
        "MODULE",
        "RETURN",
        "ALIAS",
        "IF",
        "THEN",
        "ELSE",
        "WHILE",
        "L_PARENT",
        "R_PARENT",
        "L_BRACE",
        "R_BRACE",
        "SEMI_COLON",
        "DOT",
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "MOD",
        "LST",
        "LEQ",
        "GST",
        "GEQ",
        "EQL",
        "AND",
        "OR",
        "XOR",
        "NOT",
        "C_EQL",
        "C_NEQ",
        "C_AND",
        "C_OR",
        "UKNOWN",
        "END_OF_FILE"
    };

static const std::string AS_LEXEME[] =
    {
        "",
        "",
        "",
        "",
        "",
        "import",
        "module",
        "return",
        "alias",
        "if",
        "then",
        "else",
        "while",
        "(",
        ")",
        "{",
        "}",
        ";",
        ".",
        "+",
        "-",
        "*",
        "/",
        "%",
        "<",
        "<=",
        ">",
        ">=",
        "=",
        "&",
        "|",
        "^",
        "!",
        "==",
        "!=",
        "&&",
        "||",
        "",
        ""
    };

class Pretty
{

    friend std::ostream& operator<< (std::ostream& outputStream, const Pretty& token);

public:

    Pretty(const Token token,
           const Location& location,
           const std::string& lexeme,
           const std::string& filepath);

    const std::string& asString() const;

private:

    const Token _token;

public:
    
    const Location      location;
    const std::string & lexeme;
    const std::string & filePath;
};

class Vector
{

    friend std::ostream& operator<< (std::ostream& outputStream, const Vector& tokenVector);

public:

    Vector(const File   & file,
           const Reader & reader,
           Index& index);

    std::size_t size() const;

    const std::string& filePath() const;

    Pretty operator[](const std::size_t i) const;

    void pushBack(const Token token, 
                  const index::Status indexStatus = index::Status::DROP_LEXEME);

    template <class UnaryPred>
    void trim(const UnaryPred& isTrimable)
    {
        std::vector<Token>       trimedTokens;
        std::vector<std::size_t> trimedOffsets;
        std::vector<std::size_t> trimedLexemeIDs;

        for (std::size_t i = 0; i < size(); i++)
        {
            const auto& token = _tokens[i];

            if (!isTrimable(token))
            {
                trimedTokens    .push_back( token       );
                trimedOffsets   .push_back(_offsets[i]  );
                trimedLexemeIDs .push_back(_lexemeIDs[i]);
            }
        }

        std::swap(_tokens   , trimedTokens   );
        std::swap(_offsets  , trimedOffsets  );
        std::swap(_lexemeIDs, trimedLexemeIDs);
    }

private:

    const File&    _file;
    const Reader&  _reader;
    Index&         _index;
 
    const file::Map _fileMap;

    const uint8_t* _prevReader;
    std::size_t    _prevOffset;

    std::vector<Token>       _tokens;
    std::vector<std::size_t> _offsets;
    std::vector<std::size_t> _lexemeIDs;
};

} // end token namespace

#endif