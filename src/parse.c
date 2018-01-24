#include "parse.h"
#include "tokenize.h"
#include "locale.h"
#include "hotkey.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#define internal static

internal char *
copy_string_count(char *s, int length)
{
    char *result = malloc(length + 1);
    memcpy(result, s, length);
    result[length] = '\0';
    return result;
}

internal uint32_t
keycode_from_hex(char *hex)
{
    uint32_t result;
    sscanf(hex, "%x", &result);
    return result;
}

internal uint32_t
parse_key_hex(struct parser *parser)
{
    struct token key = parser_previous(parser);
    char *hex = copy_string_count(key.text, key.length);
    uint32_t keycode = keycode_from_hex(hex);
    free(hex);
    return keycode;
}

internal uint32_t
parse_key(struct parser *parser)
{
    uint32_t keycode;
    struct token key = parser_previous(parser);
    keycode = keycode_from_char(*key.text);
    return keycode;
}

#define KEY_HAS_IMPLICIT_FN_MOD 4
internal uint32_t literal_keycode_value[] =
{
    kVK_Return,     kVK_Tab,           kVK_Space,
    kVK_Delete,     kVK_Escape,        kVK_ForwardDelete,
    kVK_Home,       kVK_End,           kVK_PageUp,
    kVK_PageDown,   kVK_Help,          kVK_LeftArrow,
    kVK_RightArrow, kVK_UpArrow,       kVK_DownArrow,
    kVK_F1,         kVK_F2,            kVK_F3,
    kVK_F4,         kVK_F5,            kVK_F6,
    kVK_F7,         kVK_F8,            kVK_F9,
    kVK_F10,        kVK_F11,           kVK_F12,
    kVK_F13,        kVK_F14,           kVK_F15,
    kVK_F16,        kVK_F17,           kVK_F18,
    kVK_F19,        kVK_F20,
};

internal void
parse_key_literal(struct parser *parser, struct hotkey *hotkey)
{
    struct token key = parser_previous(parser);
    for (int i = 0; i < array_count(literal_keycode_str); ++i) {
        if (token_equals(key, literal_keycode_str[i])) {
            if (i > KEY_HAS_IMPLICIT_FN_MOD) hotkey->flags |= Hotkey_Flag_Fn;
            hotkey->key = literal_keycode_value[i];
            break;
        }
    }
}

internal enum hotkey_flag modifier_flags_value[] =
{
    Hotkey_Flag_Alt, Hotkey_Flag_Shift,
    Hotkey_Flag_Cmd, Hotkey_Flag_Control,
    Hotkey_Flag_Fn,
};

internal uint32_t
parse_modifier(struct parser *parser)
{
    struct token modifier = parser_previous(parser);
    uint32_t flags = 0;

    for (int i = 0; i < array_count(modifier_flags_str); ++i) {
        if (token_equals(modifier, modifier_flags_str[i])) {
            flags |= modifier_flags_value[i];
            break;
        }
    }

    if (parser_match(parser, Token_Plus)) {
        if (parser_match(parser, Token_Modifier)) {
            flags |= parse_modifier(parser);
        } else {
            parser_report_error(parser, "expected modifier");
        }
    }

    return flags;
}

internal struct hotkey *
parse_hotkey(struct parser *parser)
{
    struct hotkey *hotkey = malloc(sizeof(struct hotkey));
    memset(hotkey, 0, sizeof(struct hotkey));
    bool found_modifier;

    if ((found_modifier = parser_match(parser, Token_Modifier))) {
        hotkey->flags = parse_modifier(parser);
        if (parser->error) {
            goto err;
        }
    }

    if (found_modifier) {
        if (!parser_match(parser, Token_Dash)) {
            parser_report_error(parser, "expected '-'");
            goto err;
        }
    }

    if (parser_match(parser, Token_Key)) {
        hotkey->key = parse_key(parser);
    } else if (parser_match(parser, Token_Key_Hex)) {
        hotkey->key = parse_key_hex(parser);
    } else if (parser_match(parser, Token_Literal)) {
        parse_key_literal(parser, hotkey);
    } else {
        parser_report_error(parser, "expected key-literal");
        goto err;
    }

    return hotkey;

err:
    free(hotkey);
    return NULL;
}

struct hotkey *
parse_keypress(struct parser *parser)
{
    if ((parser_check(parser, Token_Modifier)) ||
        (parser_check(parser, Token_Literal)) ||
        (parser_check(parser, Token_Key_Hex)) ||
        (parser_check(parser, Token_Key))) {
        return parse_hotkey(parser);
    } else {
        parser_report_error(parser, "expected modifier or key-literal");
        return NULL;
    }
}

struct token
parser_peek(struct parser *parser)
{
    return parser->current_token;
}

struct token
parser_previous(struct parser *parser)
{
    return parser->previous_token;
}

bool parser_eof(struct parser *parser)
{
    struct token token = parser_peek(parser);
    return token.type == Token_EndOfStream;
}

struct token
parser_advance(struct parser *parser)
{
    if (!parser_eof(parser)) {
        parser->previous_token = parser->current_token;
        parser->current_token = get_token(&parser->tokenizer);
    }
    return parser_previous(parser);
}

bool parser_check(struct parser *parser, enum token_type type)
{
    if (parser_eof(parser)) return false;
    struct token token = parser_peek(parser);
    return token.type == type;
}

bool parser_match(struct parser *parser, enum token_type type)
{
    if (parser_check(parser, type)) {
        parser_advance(parser);
        return true;
    }
    return false;
}

void parser_report_error(struct parser *parser, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    fprintf(stderr, "#%d:%d ", parser->current_token.line, parser->current_token.cursor);
    vfprintf(stderr, format, args);
    fprintf(stderr, ", but got '%.*s'\n", parser->current_token.length, parser->current_token.text);

    va_end(args);
    parser->error = true;
}

void parser_init(struct parser *parser, char *buffer)
{
    memset(parser, 0, sizeof(struct parser));
    tokenizer_init(&parser->tokenizer, buffer);
    parser_advance(parser);
}
