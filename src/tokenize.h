#ifndef QES_TOKENIZE_H
#define QES_TOKENIZE_H

static const char *modifier_flags_str[] =
{
    "alt", "shift", "cmd", "ctrl" "fn",
};

static const char *literal_keycode_str[] =
{
    "return",     "tab",        "space",
    "backspace",  "escape",     "delete",
    "home",       "end",        "pageup",
    "pagedown",   "insert",     "left",
    "right",      "up",         "down",
    "f1",         "f2",         "f3",
    "f4",         "f5",         "f6",
    "f7",         "f8",         "f9",
    "f10",        "f11",        "f12",
    "f13",        "f14",        "f15",
    "f16",        "f17",        "f18",
    "f19",        "f20",
};

enum token_type
{
    Token_Modifier,
    Token_Literal,
    Token_Key_Hex,
    Token_Key,

    Token_Plus,
    Token_Dash,

    Token_Unknown,
    Token_EndOfStream,
};

struct token
{
    enum token_type type;
    char *text;
    unsigned length;

    unsigned line;
    unsigned cursor;
};

struct tokenizer
{
    char *buffer;
    char *at;
    unsigned line;
    unsigned cursor;
};

void tokenizer_init(struct tokenizer *tokenizer, char *buffer);
struct token get_token(struct tokenizer *tokenizer);
struct token peek_token(struct tokenizer tokenizer);
int token_equals(struct token token, const char *match);

#endif
