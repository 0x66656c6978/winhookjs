#ifndef WINHOOK_NAPI_WINHOOK_H
#define WINHOOK_NAPI_WINHOOK_H

#include <napi.h>

#include "../Winhook.h"

namespace winhook {
    namespace napi {
        class WinhookNapi : public Napi::ObjectWrap<WinhookNapi> {
            public:
                static Napi::Object Init(Napi::Env env, Napi::Object exports) {
                    Napi::HandleScope scope(env);
                    
                    Napi::Function func = DefineClass(env, "Winhook", {
                        InstanceMethod("FindWindow", &WinhookNapi::FindWindow),
                        InstanceMethod("SendInput", &WinhookNapi::SendInput),
                        InstanceMethod("GetInputState", &WinhookNapi::GetInputState)
                    });
                    constructor = Napi::Persistent(func);
                    constructor.SuppressDestruct();

                    exports.Set("Winhook", func);

                    return exports;
                }
                WinhookNapi(const Napi::CallbackInfo& args): Napi::ObjectWrap<WinhookNapi>(args), _instance(new winhook::Winhook()) {
                }
                
            private:
                Napi::Value FindWindow(const Napi::CallbackInfo& args);
                Napi::Value SendInput(const Napi::CallbackInfo& args);
                Napi::Value GetInputState(const Napi::CallbackInfo& args);
                static Napi::FunctionReference constructor;
                winhook::Winhook* _instance;
        };
    }
}


#endif
