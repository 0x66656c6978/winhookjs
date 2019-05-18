#ifndef WINHOOK_NAPI_HWND_H
#define WINHOOK_NAPI_HWND_H

#include <napi.h>

#include "../Hwnd.h"

namespace winhook {
    namespace napi {
        class HwndNapi : public Napi::ObjectWrap<HwndNapi> {
            public:
                static Napi::Object Init(Napi::Env env, Napi::Object exports) {
                    Napi::HandleScope scope(env);
                    
                    Napi::Function func = DefineClass(env, "Hwnd", {});
                    constructor = Napi::Persistent(func);
                    constructor.SuppressDestruct();

                    exports.Set("Hwnd", func);

                    return exports;
                }
                
                HwndNapi(const Napi::CallbackInfo& args):
                    Napi::ObjectWrap<HwndNapi>(args), _instance(new winhook::Hwnd()) {
                    }
            private:
                static Napi::FunctionReference constructor;
                winhook::Hwnd* _instance;
        };
    }
}
#endif