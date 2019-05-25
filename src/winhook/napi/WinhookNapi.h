#ifndef WINHOOK_NAPI_WINHOOK_H
#define WINHOOK_NAPI_WINHOOK_H

#include <napi.h>

#include <windows.h>
#include <string>
#include <vector>
#include <iostream>

#include "input/InputNapi.h"

typedef BOOL (__stdcall MyCompareObjectHandles)(
  HANDLE hFirstObjectHandle,
  HANDLE hSecondObjectHandle
);

namespace winhook {
    namespace napi {
        const int MAX_RETRIES = 3;

        class WinhookNapi : public Napi::ObjectWrap<WinhookNapi> {
            public:
                static Napi::Object Init(Napi::Env env, Napi::Object exports) {
                    Napi::HandleScope scope(env);

                    Napi::Function func = DefineClass(env, "Winhook", {
                        StaticMethod("FindWindow", &WinhookNapi::FindWindow),
                        StaticMethod("ShowWindow", &WinhookNapi::ShowWindow),
                        StaticMethod("SetForegroundWindow", &WinhookNapi::SetForegroundWindow),
                        StaticMethod("GetForegroundWindow", &WinhookNapi::GetForegroundWindow),
                        StaticMethod("CreateProcess", &WinhookNapi::CreateProcess),
                        StaticMethod("SendInput", &WinhookNapi::SendInput),
                        StaticMethod("CloseHandle", &WinhookNapi::CloseHandle),
                        StaticMethod("CloseWindow", &WinhookNapi::CloseWindow),
                        StaticMethod("GetLastError", &WinhookNapi::GetLastError),
                        StaticValue("INPUT_MOUSE", Napi::Number::New(env, 0)),
                        StaticValue("INPUT_KEYBOARD", Napi::Number::New(env, 1)),
                        StaticValue("INPUT_HARDWARE", Napi::Number::New(env, 2)),
                    });
                    constructor = Napi::Persistent(func);
                    constructor.SuppressDestruct();

                    exports.Set("Winhook", func);

                    return exports;
                }
                WinhookNapi(const Napi::CallbackInfo& args): 
                    Napi::ObjectWrap<WinhookNapi>(args) {
                }
                
            private:
                static Napi::Value CreateProcess(const Napi::CallbackInfo& args);
                static Napi::Value FindWindow(const Napi::CallbackInfo& args);
                static Napi::Value ShowWindow(const Napi::CallbackInfo& args);
                static Napi::Value SendInput(const Napi::CallbackInfo& args);
                static Napi::Value SetForegroundWindow(const Napi::CallbackInfo& args);
                static Napi::Value GetForegroundWindow(const Napi::CallbackInfo& args);
                static Napi::Value CloseHandle(const Napi::CallbackInfo& args);
                static Napi::Value CloseWindow(const Napi::CallbackInfo& args);
                static Napi::Value GetLastError(const Napi::CallbackInfo& args);

                static Napi::FunctionReference constructor;
        };
    }
}


#endif
