#ifndef QES_PARSE_H
#define QES_PARSE_H

#include "tokenize.h"
#include <stdbool.h>

struct parser
{
    struct token previous_token;
    struct token current_token;
    struct tokenizer tokenizer;
    bool error;
};

struct hotkey;
struct hotkey *parse_keypress(struct parser *parser);

struct token parser_peek(struct parser *parser);
struct token parser_previous(struct parser *parser);
bool parser_eof(struct parser *parser);
struct token parser_advance(struct parser *parser);
bool parser_check(struct parser *parser, enum token_type type);
bool parser_match(struct parser *parser, enum token_type type);
void parser_init(struct parser *parser, char *buffer);
void parser_report_error(struct parser *parser, const char *format, ...);

#endif
