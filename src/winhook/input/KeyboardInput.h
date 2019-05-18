#ifndef WINHOOK_INPUT_KEYBOARDINPUT_H
#define WINHOOK_INPUT_KEYBOARDINPUT_H

#include <wtypes.h>

namespace winhook {
    namespace input {
        class KeyboardInput {
        public:
            WORD    wVk;
            WORD    wScan;
            DWORD   dwFlags;
            DWORD   time;
            ULONG_PTR dwExtraInfo;
            KeyboardInput(): wVk(0), wScan(0), dwFlags(0), time(0), dwExtraInfo(NULL) {
                
            }

            void raw(KEYBDINPUT* ki) {
                ki->wVk = wVk;
                ki->wScan = wScan;
                ki->dwFlags = dwFlags;
                ki->time = time;
                ki->dwExtraInfo = dwExtraInfo;
            }

            static KeyboardInput* create(KEYBDINPUT mi) {
                KeyboardInput* inp = new KeyboardInput();
                inp->wVk = mi.wVk;
                inp->wScan = mi.wScan;
                inp->dwFlags = mi.dwFlags;
                inp->time = mi.time;
                inp->dwExtraInfo = mi.dwExtraInfo;
                return inp;
            }
        };
    }
}

#endif
