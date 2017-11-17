#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define internal static

struct tokenizer
{
    char *buffer;
    char *at;
    int cursor;
};

struct token
{
    char *text;
    int length;
    int cursor;
};

struct token get_token()
{
    struct token token;
    return token;
}

struct key_event
{
};

internal void
synthesize_key_event(struct key_event *event)
{
}

internal struct key_event
parse_key_event()
{
}

struct pointer_event
{
};

internal void
synthesize_pointer_event(struct pointer_event *event)
{
}

internal struct pointer_event
parse_pointer_event()
{
}

struct text_event
{
};

internal void
synthesize_text_event(struct text_event *event)
{
}

internal struct text_event
parse_text_event()
{
}

enum event_type
{
    Event_Type_Key,
    Event_Type_Pointer,
    Event_Type_Text,
};

struct event
{
    enum event_type type;
    union {
        struct pointer_event p;
        struct key_event k;
        struct text_event t;
    };
};

internal void
synthesize_event(struct event *event)
{
    switch (event.type) {
    case Event_Type_Key:     synthesize_key_event(event->k);     break;
    case Event_Type_Pointer: synthesize_pointer_event(event->p); break;
    case Event_Type_Text:    synthesize_text_event(event->t);    break;
    }
}

/* syntax draft:
 *
 * ./qes -key "a" -mod "shift, cmd" -action "press"
 * ./qes -key "a" -mod "shift, cmd" -action "release"
 *
 * ./qes -pointer "left" -action "press"
 * ./qes -pointer "left" -action "release"
 *
 * ./qes -text "some text to output"

 * absolute coordinates unless relative prefix: +, -
 * ./qes -pointer " 20, 20" -action "move"
 * ./qes -pointer "+20,+20" -action "move"
 * ./qes -pointer "-20,+20" -action "move"
 * */
internal struct event
parse_event(int argc, char **argv)
{
}

int main(int argc, char **argv)
{
    struct event event = parse_event(argc, argv);
    synthesize_event(&event);
    return 0;
}
