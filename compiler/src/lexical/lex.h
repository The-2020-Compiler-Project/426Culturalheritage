#ifndef _SRC_LEXICAL_LEX
#define _SRC_LEXICAL_LEX
enum Token_type {
    TK_SEMICOLON = ';',
    TK_LEFTPAR = '(',
    TK_RIGHTPAR = ')',
    TK_COMMA = ',',
    TK_SHARP = '#',
    TK_TLIDE = '~',
    TK_LEFTBRKT = '[',
    TK_RIGHTBRKT = ']',
    TK_LEFTBRACE = '{',
    TK_RIGHTBRACE = '}',
    TK_DOT = '.',
    TK_QUESTIONMARK = '?',
    TK_COLON = ':',
    TK_LT = '<',
    TK_GT = '>',
    TK_ASSIGN = '=',
    TK_EX = '!',
    TK_AND = '&',
    TK_OR = '|',
    TK_ADD = '+',
    TK_SUB = '-',
    TK_MUL = '*',
    TK_DIV = '/',
    TK_MOD = '%',
    TK_XOR = '^',
    TK_NUM = 256,  // Number literal
    TK_CHAR,       // Character literal
    TK_STR,        // String literal
    TK_IDENT,      // Identifier
    TK_ARROW,      // ->
    TK_EXTERN,     // "extern"
    TK_TYPEDEF,    // "typedef"
    TK_NUMBERTYPE, // "number"
    TK_CHARTYPE,   // "char"
    TK_STRINGTYPE, // "char"
    TK_VOID,       // "void"
    TK_STRUCT,     // "struct"
    TK_BOOL,       // "Bool"
    TK_IF,         // "if"
    TK_ELSE,       // "else"
    TK_FOR,        // "for"
    TK_DO,         // "do"
    TK_WHILE,      // "while"
    TK_SWITCH,     // "switch"
    TK_CASE,       // "case"
    TK_DEFAULT,    // "case"
    TK_BREAK,      // "break"
    TK_CONTINUE,   // "continue"
    TK_NEW,        // new
    TK_DELETE,     // delete
    TK_MAIN,       // main
    TK_EQ,         // ==
    TK_NE,         // !=
    TK_LE,         // <=
    TK_GE,         // >=
    TK_LOGOR,      // ||
    TK_LOGAND,     // &&
    TK_SHL,        // <<
    TK_SHR,        // >>
    TK_INC,        // ++
    TK_DEC,        // --
    TK_MUL_EQ,     // *=
    TK_DIV_EQ,     // /=
    TK_MOD_EQ,     // %=
    TK_ADD_EQ,     // +=
    TK_SUB_EQ,     // -=
    TK_SHL_EQ,     // <<=
    TK_SHR_EQ,     // >>=
    TK_AND_EQ,     // &=
    TK_XOR_EQ,     // ^=
    TK_OR_EQ,      // |=
    TK_RETURN,     // "return"
    TK_SIZEOF,     // "sizeof"
    TK_ALIGNOF,    // "_Alignof"
    TK_TYPEOF,     // "typeof"
    TK_PARAM,      // Function-like macro parameter
    TK_EOF,        // End marker
};

typedef struct {
    char *Name;
    Token_type token_type;
    int line_num;
    int colomn_num;
} Token;

Token get_token();

#endif
