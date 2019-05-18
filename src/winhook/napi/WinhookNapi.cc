#include "WinhookNapi.h"

namespace winhook {
    namespace napi {
        Napi::FunctionReference WinhookNapi::constructor;

        Napi::Value WinhookNapi::FindWindow(const Napi::CallbackInfo& args) {
            Napi::Env env = args.Env();
            Napi::HandleScope scope(env);
            if (!args[0].IsString()) {
                Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
            }
            Napi::String windowName = args[0].As<Napi::String>();
            return Napi::Boolean::New(env, this->_instance->FindWindow(windowName.Utf8Value().c_str()));
        }
        
        Napi::Value WinhookNapi::SendInput(const Napi::CallbackInfo& args) {
            Napi::Env env  = args.Env();
            Napi::HandleScope scope(env);
            return Napi::Value::From<int>(env, 1);
        }

        Napi::Value WinhookNapi::GetInputState(const Napi::CallbackInfo& args) {
            Napi::Env env  = args.Env();
            Napi::HandleScope scope(env);
            return Napi::Value::From<int>(env, 1);
        }

    }
}