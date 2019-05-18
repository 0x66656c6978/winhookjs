#ifndef WINHOOK_INPUT_HARDWAREINPUT_H
#define WINHOOK_INPUT_HARDWAREINPUT_H

#include <wtypes.h>

namespace winhook {
    namespace input {
        class HardwareInput {
        public:
            DWORD   uMsg;
            WORD    wParamH;
            WORD    wParamL;
            HardwareInput(): uMsg(0), wParamH(0), wParamL(0) {
                
            }

            void raw(HARDWAREINPUT* hi) {
                hi->uMsg = uMsg;
                hi->wParamH = wParamH;
                hi->wParamL = wParamL;
            }

            static HardwareInput* create(HARDWAREINPUT hi) {
                HardwareInput *inp = new HardwareInput();
                inp->uMsg = hi.uMsg;
                inp->wParamH = hi.wParamH;
                inp->wParamL = hi.wParamL;
            }
        };
    }
}

#endif