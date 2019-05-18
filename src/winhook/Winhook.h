#ifndef WINHOOK_WINHOOK_H
#define WINHOOK_WINHOOK_H

#include <windows.h>
#include "input/Input.h"

namespace winhook {

    using winhook::input::Input;

    class Winhook {
    public:
        bool FindWindow(const char* windowName);
        void SendInput(Input* input);
        Input* GetInputState();
    };
}

#endif