#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>

#include <Carbon/Carbon.h>

#include "locale.h"
#include "tokenize.h"
#include "parse.h"
#include "hotkey.h"

#include "locale.c"
#include "tokenize.c"
#include "parse.c"

internal unsigned major_version = 0;
internal unsigned minor_version = 0;
internal unsigned patch_version = 2;

internal inline void
create_and_post_keyevent(uint16_t key, bool pressed)
{
    CGPostKeyboardEvent((CGCharCode)0, (CGKeyCode)key, pressed);
}

internal inline void
synthesize_modifiers(struct hotkey *hotkey, bool pressed)
{
    if (has_flags(hotkey, Hotkey_Flag_Alt)) {
        create_and_post_keyevent(Modifier_Keycode_Alt, pressed);
    }
    if (has_flags(hotkey, Hotkey_Flag_Shift)) {
        create_and_post_keyevent(Modifier_Keycode_Shift, pressed);
    }
    if (has_flags(hotkey, Hotkey_Flag_Cmd)) {
        create_and_post_keyevent(Modifier_Keycode_Cmd, pressed);
    }
    if (has_flags(hotkey, Hotkey_Flag_Control)) {
        create_and_post_keyevent(Modifier_Keycode_Ctrl, pressed);
    }
    if (has_flags(hotkey, Hotkey_Flag_Fn)) {
        create_and_post_keyevent(Modifier_Keycode_Fn, pressed);
    }
}

internal void
synthesize_key(char *key_string)
{
    struct parser parser;
    parser_init(&parser, key_string);
    struct hotkey *hotkey = parse_keypress(&parser);
    if (hotkey) {
        CGSetLocalEventsSuppressionInterval(0.0f);
        CGEnableEventStateCombining(false);

        synthesize_modifiers(hotkey, true);
        create_and_post_keyevent(hotkey->key, true);

        create_and_post_keyevent(hotkey->key, false);
        synthesize_modifiers(hotkey, false);
    }
}

internal void
synthesize_text(char *text)
{
    CFStringRef text_ref = CFStringCreateWithCString(NULL, text, kCFStringEncodingUTF8);
    CFIndex text_length = CFStringGetLength(text_ref);

    CGEventRef de = CGEventCreateKeyboardEvent(NULL, 0, true);
    CGEventRef ue = CGEventCreateKeyboardEvent(NULL, 0, false);

    CGEventSetFlags(de, 0);
    CGEventSetFlags(ue, 0);

    UniChar c;
    for(CFIndex i = 0; i < text_length; ++i)
    {
        c = CFStringGetCharacterAtIndex(text_ref, i);
        CGEventKeyboardSetUnicodeString(de, 1, &c);
        CGEventPost(kCGAnnotatedSessionEventTap, de);
        usleep(1000);
        CGEventKeyboardSetUnicodeString(ue, 1, &c);
        CGEventPost(kCGAnnotatedSessionEventTap, ue);
    }

    CFRelease(ue);
    CFRelease(de);
    CFRelease(text_ref);
}

int main(int argc, char **argv)
{
    int option;
    const char *short_option = "vk:t:";
    struct option long_option[] = {
        { "version", no_argument, NULL, 'v' },
        { "key", required_argument, NULL, 'k' },
        { "text", required_argument, NULL, 't' },
        { NULL, 0, NULL, 0 }
    };

    while ((option = getopt_long(argc, argv, short_option, long_option, NULL)) != -1) {
        switch (option) {
        case 'v': {
            printf("qes version %d.%d.%d\n", major_version, minor_version, patch_version);
            return true;
        } break;
        case 'k': {
            synthesize_key(optarg);
        } break;
        case 't': {
            synthesize_text(optarg);
        } break;
        }
    }

    return 0;
}
