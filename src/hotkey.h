#ifndef QES_HOTKEY_H
#define QES_HOTKEY_H

#include <Carbon/Carbon.h>
#include <stdint.h>
#include <stdbool.h>

#define Modifier_Keycode_Alt     0x3A
#define Modifier_Keycode_Shift   0x38
#define Modifier_Keycode_Cmd     0x37
#define Modifier_Keycode_Ctrl    0x3B
#define Modifier_Keycode_Fn      0x3F

enum hotkey_flag
{
    Hotkey_Flag_Alt         = (1 <<  0),
    Hotkey_Flag_Shift       = (1 <<  1),
    Hotkey_Flag_Cmd         = (1 <<  2),
    Hotkey_Flag_Control     = (1 <<  3),
    Hotkey_Flag_Fn          = (1 <<  4),
};

struct hotkey
{
    uint32_t flags;
    uint32_t key;
};

static inline void
add_flags(struct hotkey *hotkey, uint32_t flag)
{
    hotkey->flags |= flag;
}

static inline bool
has_flags(struct hotkey *hotkey, uint32_t flag)
{
    bool result = hotkey->flags & flag;
    return result;
}

static inline void
clear_flags(struct hotkey *hotkey, uint32_t flag)
{
    hotkey->flags &= ~flag;
}

#endif
