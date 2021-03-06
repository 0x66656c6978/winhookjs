#ifndef WINHOOK_INPUT_INPUT_H
#define WINHOOK_INPUT_INPUT_H

#include "HardwareInput.h"
#include "MouseInput.h"
#include "KeyboardInput.h"

namespace winhook {
    namespace input {
        class Input {
        public:
            DWORD type;
            HardwareInput* hi;
            MouseInput* mi;
            KeyboardInput* ki;

            Input():
                type(0), hi(new HardwareInput()), mi(new MouseInput()), ki(new KeyboardInput()) {

            }

            void raw(INPUT* inp) {
                inp->type = type;
                switch (inp->type) {
                    case INPUT_HARDWARE:
                        hi->raw(&(inp->hi));
                        break;
                    case INPUT_MOUSE:
                        mi->raw(&(inp->mi));                        
                        break;
                    case INPUT_KEYBOARD:
                        ki->raw(&(inp->ki));
                        break;
                }
            }

            static Input* create(INPUT inp) {
                Input* input = new Input();
                input->type = inp.type;
                input->hi = HardwareInput::create(inp.hi);
                input->mi = MouseInput::create(inp.mi);
                input->ki = KeyboardInput::create(inp.ki);
                return input;
            }
        };
    }
}

#endif
