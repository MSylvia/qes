#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define internal static

inline internal bool
same_string(const char *a, const char *b)
{
    return strcmp(a, b) == 0;
}

internal char *
get_token(int argc, char **argv, int *current_argument)
{
    if (++(*current_argument) >= argc) return NULL;
    return argv[*current_argument];
}

struct key_event
{
};

internal void
synthesize_key_event(struct key_event *event)
{
}

internal struct key_event
parse_key_event(int argc, char **argv)
{
    struct key_event event;
    char *token, *key, *mod, *action;
    int current_argument;

    current_argument = 0;
    token = get_token(argc, argv, &current_argument);
    assert(same_string(token, "-key"));

    // @incomplete: lookup keycode
    key = get_token(argc, argv, &current_argument);
    if (!key) {
        // @incomplete: print error and exit
    }

    // @incomplete: optional argument
    token = get_token(argc, argv, &current_argument);
    if (same_string(token, "-mod")) {
        mod = get_token(argc, argv, &current_argument);
        if (!mod) {
            // @incomplete: print error and exit
        }

        while (*mod) {
            int mod_length;
            char *next_mod;

            next_mod = strchr(mod, ' ');
            if (next_mod) {
                mod_length = next_mod - mod;
                while (*next_mod == ' ') ++next_mod;
            } else {
                mod_length = strlen(mod);
                next_mod = mod + mod_length;
            }

            // @incomplete: lookup flag for modifier
            printf("mod: '%.*s'\n", mod_length, mod);

            mod = next_mod;
        }
    }

    // @incomplete: validate action
    token = get_token(argc, argv, &current_argument);
    if (same_string(token, "-action")) {
        action = get_token(argc, argv, &current_argument);
        if (!action) {
            // @incomplete: print error and exit
        }
    }

    return event;
}

struct pointer_event
{
};

internal void
synthesize_pointer_event(struct pointer_event *event)
{
}

internal struct pointer_event
parse_pointer_event(int argc, char **argv)
{
    struct pointer_event event;
    return event;
}

struct text_event
{
};

internal void
synthesize_text_event(struct text_event *event)
{
}

internal struct text_event
parse_text_event(int argc, char **argv)
{
    struct text_event event;
    return event;
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
    switch (event->type) {
    case Event_Type_Key:     synthesize_key_event(&event->k);     break;
    case Event_Type_Pointer: synthesize_pointer_event(&event->p); break;
    case Event_Type_Text:    synthesize_text_event(&event->t);    break;
    }
}

internal void
print_usage_and_exit(char *name)
{
    // @incomplete
    fprintf(stdout, "usage: %s ..\n", name);
    exit(1);
}

/* syntax draft:
 *
 * ./qes -key "a" -mod "shift cmd" -action "press"
 * ./qes -key "a" -mod "shift cmd" -action "release"
 *
 * ./qes -pointer "left" -action "press"
 * ./qes -pointer "left" -action "release"
 *
 * ./qes -text "some text to output"
 *
 * absolute coordinates unless relative prefix: +, -
 * ./qes -pointer " 20  20" -action "move"
 * ./qes -pointer "+20 +20" -action "move"
 * ./qes -pointer "-20 +20" -action "move"
 *
 * */

internal struct event
parse_event(int argc, char **argv)
{
    struct event event;

    if (same_string(argv[1], "-key")) {
        event.type = Event_Type_Key;
        event.k = parse_key_event(argc, argv);
    } else if (same_string(argv[1], "-pointer")) {
        event.type = Event_Type_Pointer;
        event.p = parse_pointer_event(argc, argv);
    } else if (same_string(argv[1], "-text")) {
        event.type = Event_Type_Text;
        event.t = parse_text_event(argc, argv);
    } else {
        print_usage_and_exit(argv[0]);
    }

    return event;
}

int main(int argc, char **argv)
{
    if (argc < 2) print_usage_and_exit(argv[0]);
    struct event event = parse_event(argc, argv);
    synthesize_event(&event);
    return 0;
}
