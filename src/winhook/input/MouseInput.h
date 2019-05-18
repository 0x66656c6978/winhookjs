#ifndef WINHOOK_INPUT_MOUSEINPUT_H
#define WINHOOK_INPUT_MOUSEINPUT_H

#include <wtypes.h>

namespace winhook {
    namespace input {
        class MouseInput {
        public:
            LONG      dx;
            LONG      dy;
            DWORD     mouseData;
            DWORD     dwFlags;
            DWORD     time;
            ULONG_PTR dwExtraInfo;
            MouseInput(): dx(0), dy(0), mouseData(0), dwFlags(0), time(0), dwExtraInfo(NULL) {

            }
            void raw(MOUSEINPUT* mi) {
                mi->dx = dx;
                mi->dy = dy;
                mi->mouseData = mouseData;
                mi->dwFlags = dwFlags;
                mi->time = time;
                mi->dwExtraInfo = dwExtraInfo;
            }

            static MouseInput* create(MOUSEINPUT mi) {
                MouseInput* inp = new MouseInput();
                inp->dx = mi.dx;
                inp->dy = mi.dy;
                inp->mouseData = mi.mouseData;
                inp->dwFlags = mi.dwFlags;
                inp->time = mi.time;
                inp->dwExtraInfo = mi.dwExtraInfo;
                return inp;
            }
        };
    }
}

#endif