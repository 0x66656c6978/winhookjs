#include "InputNapi.h"

namespace winhook {
    namespace napi {
        namespace input {
            Napi::FunctionReference InputNapi::constructor;

            Napi::Value InputNapi::GetType(const Napi::CallbackInfo& args) {
                Napi::Env env = args.Env();
                Napi::HandleScope scope(env);

                return Napi::Number::From<unsigned long>(env, this->_instance->type);
            }

            void InputNapi::SetType(const Napi::CallbackInfo& args, const Napi::Value& value) {
                Napi::Env env = args.Env();
                Napi::HandleScope scope(env);

                if(!args[0].IsNumber()) {
                    Napi::TypeError::New(env, "Expected DWORD").ThrowAsJavaScriptException();;
                }

                this->_instance->type = args[0].As<Napi::Number>().Uint32Value();
            }

            Napi::Value InputNapi::GetHI(const Napi::CallbackInfo& args) {
                Napi::Env env = args.Env();
                Napi::HandleScope scope(env);

                winhook::input::HardwareInput* hi = this->_instance->hi;
                
                Napi::Object obj = Napi::Object::New(env);
                
                Napi::Value val = Napi::Value::From(env, hi->uMsg);

                obj["uMsg"] = val;
                obj["wParamL"] = Napi::Value::From(env, hi->wParamL);
                obj["wParamH"] = Napi::Value::From(env, hi->wParamH);

                return Napi::Value::Value(obj);
            }

            void InputNapi::SetHI(const Napi::CallbackInfo& args, const Napi::Value& value) {
                Napi::Env env = args.Env();
                Napi::HandleScope scope(env);
                
                if (!args[0].IsObject()) {
                    Napi::TypeError::New(env, "Expected Object").ThrowAsJavaScriptException();;
                }
                
                winhook::input::HardwareInput* hi = this->_instance->hi;
                Napi::Object obj = args[0].As<Napi::Object>();
                Napi::Value tmp;
                

                if (obj.Has("uMsg")) {
                    tmp = obj["uMsg"];
                    hi->uMsg = tmp.ToNumber().Uint32Value();
                }
                if (obj.Has("wParamH")) {
                    tmp = obj["wParamH"];
                    hi->wParamH = (unsigned short)tmp.ToNumber().Uint32Value();
                }
                if (obj.Has("wParamL")) {
                    tmp = obj["wParamL"];
                    hi->wParamL = (unsigned short)tmp.ToNumber().Uint32Value();
                }
            }

            Napi::Value InputNapi::GetKI(const Napi::CallbackInfo& args) {
                Napi::Env env = args.Env();
                Napi::HandleScope scope(env);
                winhook::input::KeyboardInput* ki = this->_instance->ki;
                Napi::Object obj = Napi::Object::New(env);
                obj["wVk"] = Napi::Value::From(env, ki->wVk);
                obj["wScan"] = Napi::Value::From(env, ki->wScan);
                obj["dwFlags"] = Napi::Value::From(env, ki->dwFlags);
                obj["time"] = Napi::Value::From(env, ki->time);
                // dwExtraInfo not supported for now...
                return Napi::Value::Value(obj);
            }

            void InputNapi::SetKI(const Napi::CallbackInfo& args, const Napi::Value& value) {
                Napi::Env env = args.Env();
                Napi::HandleScope scope(env);
                if (!args[0].IsObject()) {
                    Napi::TypeError::New(env, "Expected Object").ThrowAsJavaScriptException();;
                }
                winhook::input::KeyboardInput* ki = this->_instance->ki;
                Napi::Object obj = args[0].As<Napi::Object>();
                Napi::Value tmp;
                if (obj.Has("wVk")) {
                    tmp = obj["wVk"];
                    ki->wVk = (unsigned short)tmp.ToNumber().Uint32Value();
                }
                if (obj.Has("wScan")) {
                    tmp = obj["wScan"];
                    ki->wScan = (unsigned short)tmp.ToNumber().Uint32Value();
                }
                if (obj.Has("dwFlags")) {
                    tmp = obj["dwFlags"];
                    ki->dwFlags = tmp.ToNumber().Uint32Value();
                }
                if (obj.Has("time")) {
                    tmp = obj["time"];
                    ki->time = tmp.ToNumber().Uint32Value();
                }
            }


            Napi::Value InputNapi::GetMI(const Napi::CallbackInfo& args) {
                Napi::Env env = args.Env();
                Napi::HandleScope scope(env);
                winhook::input::MouseInput* mi = this->_instance->mi;
                Napi::Object obj = Napi::Object::New(env);
                obj["dx"] = Napi::Value::From(env, mi->dx);
                obj["dy"] = Napi::Value::From(env, mi->dy);
                obj["mouseData"] = Napi::Value::From(env, mi->mouseData);
                obj["dwFlags"] = Napi::Value::From(env, mi->dwFlags);
                obj["time"] = Napi::Value::From(env, mi->time);
                return Napi::Value::Value(obj);
            }

            void InputNapi::SetMI(const Napi::CallbackInfo& args, const Napi::Value& value) {
                Napi::Env env = args.Env();
                Napi::HandleScope scope(env);
                if (!args[0].IsObject()) {
                    Napi::TypeError::New(env, "Expected Object").ThrowAsJavaScriptException();;
                }
                winhook::input::MouseInput* mi = this->_instance->mi;
                Napi::Object obj = args[0].As<Napi::Object>();
                Napi::Value tmp;
                if (obj.Has("dx")) {
                    tmp = obj["dx"];
                    mi->dx = tmp.ToNumber().Int32Value();
                }
                if (obj.Has("dy")) {
                    tmp = obj["dy"];
                    mi->dy = tmp.ToNumber().Int32Value();
                }
                if (obj.Has("mouseData")) {
                    tmp = obj["mouseData"];
                    mi->mouseData = tmp.ToNumber().Uint32Value();
                }
                if (obj.Has("dwFlags")) {
                    tmp = obj["dwFlags"];
                    mi->dwFlags = tmp.ToNumber().Uint32Value();
                }
                if (obj.Has("time")) {
                    tmp = obj["time"];
                    mi->time = tmp.ToNumber().Uint32Value();
                }
            }


            winhook::input::Input* InputNapi::GetInputInstance() {
                return this->_instance;    
            }

        }
    }
}
