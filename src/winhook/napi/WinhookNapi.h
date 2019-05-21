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
                        InstanceMethod("FindWindow", &WinhookNapi::FindWindow),
                        InstanceMethod("SetForegroundWindow", &WinhookNapi::SetForegroundWindow),
                        InstanceMethod("GetForegroundWindow", &WinhookNapi::GetForegroundWindow),
                        InstanceMethod("CreateProcess", &WinhookNapi::CreateProcess),
                        InstanceMethod("SendInput", &WinhookNapi::SendInput),
                        StaticMethod("GetLastError", &WinhookNapi::GetLastError),
                        StaticValue("INPUT_MOUSE", Napi::Number::New(env, 0)),
                        StaticValue("INPUT_KEYBOARD", Napi::Number::New(env, 1)),
                        StaticValue("INPUT_HARDWARE", Napi::Number::New(env, 2)),
                    });
                    constructor = Napi::Persistent(func);
                    constructor.SuppressDestruct();

                    exports.Set("Winhook", func);

                    HMODULE dllProc = ::LoadLibraryA("Kernelbase.dll");

                    if(!dllProc) {
                        Napi::TypeError::New(env, "Could not load Kernelbase.dll").ThrowAsJavaScriptException();
                    }

                    compareFn = (MyCompareObjectHandles*)::GetProcAddress(dllProc, "CompareObjectHandles");
                    if(!compareFn) {
                        Napi::TypeError::New(env, "Failed toload MyCompareObjectHandles").ThrowAsJavaScriptException();
                    }

                    return exports;
                }
                WinhookNapi(const Napi::CallbackInfo& args): 
                    Napi::ObjectWrap<WinhookNapi>(args), handles(std::vector<HANDLE>()) {
                }
                
            private:
                std::vector<HANDLE> handles;
                static MyCompareObjectHandles* compareFn;

                Napi::Value CreateProcess(const Napi::CallbackInfo& args);
                Napi::Value FindWindow(const Napi::CallbackInfo& args);
                Napi::Value SendInput(const Napi::CallbackInfo& args);
                Napi::Value SetForegroundWindow(const Napi::CallbackInfo& args);
                Napi::Value GetForegroundWindow(const Napi::CallbackInfo& args);
                static Napi::Value GetLastError(const Napi::CallbackInfo& args);

                int _GetHandleIndex(HANDLE target);

                static Napi::FunctionReference constructor;
        };
    }
}


#endif
