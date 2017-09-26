#ifndef __TOKEN_VECTOR_H__
#define __TOKEN_VECTOR_H__

#include "reader.hpp"
#include "index.hpp"
#include "file.hpp"

#include <cstddef>
#include <string>
#include <vector>

using Token = uint8_t;

namespace token
{

enum : uint8_t
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


const std::string& asString(uint8_t word);

class Vector
{

public:

    Vector(const File   & file,
           const Reader & reader,
           Index        & index);

    void pushBack(const uint8_t word, 
                  const index::Status indexStatus = index::Status::DROP_LEXEME);

    std::size_t size() const;

private:

    const File&    _file;
    const Reader&  _reader;
    Index&         _index;

    const uint8_t* _prevReader;
    std::size_t    _prevOffset;

public: // Second public for members memory layout

    std::vector<Token>       words;
    std::vector<std::size_t> offsets;
    std::vector<std::size_t> lexemeIDs;
};

} // end token namespace

#endif