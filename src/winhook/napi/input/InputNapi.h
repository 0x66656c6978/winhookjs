#ifndef WINHOOK_NAPI_INPUT_INPUT_H
#define WINHOOK_NAPI_INPUT_INPUT_H

#include <napi.h>
#include "../../input/Input.h"
#include <iostream>

namespace winhook {
    namespace napi {
        namespace input {
            class InputNapi : public Napi::ObjectWrap<InputNapi> {
            public:
                static Napi::Object Init(Napi::Env env, Napi::Object exports) {
                    Napi::HandleScope scope(env);
                    
                    Napi::Function func = DefineClass(env, "Input", {
                        InstanceAccessor("type", &InputNapi::GetType, &InputNapi::SetType),
                        InstanceAccessor("hi", &InputNapi::GetHI, &InputNapi::SetHI),
                        InstanceAccessor("ki", &InputNapi::GetKI, &InputNapi::SetKI),
                        InstanceAccessor("mi", &InputNapi::GetMI, &InputNapi::SetMI),
                    });
                    constructor = Napi::Persistent(func);
                    constructor.SuppressDestruct();

                    exports.Set("Input", func);

                    return exports;
                }
                InputNapi(const Napi::CallbackInfo& args):
                    Napi::ObjectWrap<InputNapi>(args),
                    _instance(new winhook::input::Input()) {
                        
                }

            private:
                static Napi::FunctionReference constructor;

                Napi::Value GetType(const Napi::CallbackInfo& args);
                void SetType(const Napi::CallbackInfo& args, const Napi::Value& value);
                
                Napi::Value GetHI(const Napi::CallbackInfo& args);
                void SetHI(const Napi::CallbackInfo& args, const Napi::Value& value);

                Napi::Value GetKI(const Napi::CallbackInfo& args);
                void SetKI(const Napi::CallbackInfo& args, const Napi::Value& value);

                Napi::Value GetMI(const Napi::CallbackInfo& args);
                void SetMI(const Napi::CallbackInfo& args, const Napi::Value& value);

                winhook::input::Input* _instance;
            };
        }
    }
}


#endif
