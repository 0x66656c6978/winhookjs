#include "WinhookNapi.h"

namespace winhook {
    namespace napi {

        using winhook::input::Input;

        Napi::FunctionReference WinhookNapi::constructor;

        Napi::Value WinhookNapi::CreateProcess(const Napi::CallbackInfo& args) {
            Napi::Env env = args.Env();
            Napi::HandleScope scope(env);

            STARTUPINFOA si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            if(!::CreateProcessA(
                args[0].ToString().Utf8Value().c_str(),
                NULL,
                NULL,
                NULL,
                false,
                0,
                NULL,
                NULL,
                &si,
                &pi
            )) {
                return Napi::Number::New(env, -1);
            }

            Napi::Object ret = Napi::Object::New(env);
            ret["hProcess"] = Napi::Value(Napi::Number::New(env, (unsigned long long)pi.hProcess));
            ret["hThread"] = Napi::Value(Napi::Number::New(env, (unsigned long long)pi.hThread));
            return ret;
        }

        Napi::Value WinhookNapi::FindWindow(const Napi::CallbackInfo& args) {
            Napi::Env env = args.Env();
            Napi::HandleScope scope(env);
            if (!args[0].IsString()) {
                return Napi::Number::New(env, -1);
            }
            Napi::String windowName = args[0].As<Napi::String>();
            HWND hwnd;
            int retries = 0;
            while((hwnd = ::FindWindowA(NULL, windowName.Utf8Value().c_str())) == NULL) {
                if (retries > MAX_RETRIES) {
                    return Napi::Number::New(env, -1);
                }
                Sleep(30);
                retries++;
            }
            return Napi::Number::New(env, (unsigned long long)hwnd);
        }
        
        Napi::Value WinhookNapi::SendInput(const Napi::CallbackInfo& args) {
            Napi::Env env  = args.Env();
            Napi::HandleScope scope(env);

            INPUT *nativeInputs = (INPUT*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(INPUT) * args.Length());

            for (int i = 0; i < args.Length(); i++) {
                INPUT current;
                if (!args[i].IsObject()) {
                    printf("inputs[%d] is not an object.\n", i);
                    return Napi::Number::New(env, -1);
                }
                Napi::Object objArg = args[i].ToObject();
                unsigned int type;

                if (!objArg.Has("type")) {
                    printf("inputs[%d].type is undefined.\n", i);
                    return Napi::Number::New(env, -1);
                }
               
                type = Napi::Value(objArg["type"]).ToNumber().Uint32Value();

                const char* subProperty = NULL;
                switch (type) {
                    case INPUT_HARDWARE: subProperty = "hi"; break;
                    case INPUT_MOUSE: subProperty = "mi"; break;
                    case INPUT_KEYBOARD: subProperty = "ki"; break;
                    default:
                        printf("inputs[%d].type is invalid.\n", i);
                        return Napi::Number::New(env, -1);
                }

                if (!objArg.Has(subProperty)) {
                    printf("inputs[%d].%s is not defined.\n", i, subProperty);
                    return Napi::Number::New(env, -1);
                }

                Napi::Object subObjVal = Napi::Value(objArg[subProperty]).ToObject();

                current.type = type;
                auto properties = std::vector<const char*>();

                switch (type) {
                    case INPUT_HARDWARE: 
                        properties.push_back("uMsg");
                        properties.push_back("wParamL");
                        properties.push_back("wParamH");
                    break;
                    case INPUT_MOUSE:
                        properties.push_back("dx");
                        properties.push_back("dy");
                        properties.push_back("mouseData");
                        properties.push_back("dwFlags");
                        properties.push_back("time");
                    break;
                    case INPUT_KEYBOARD:
                        properties.push_back("wVk");
                        properties.push_back("wScan");
                        properties.push_back("dwFlags");
                        properties.push_back("time");
                    break;
                }

                for (auto j = properties.begin(); j != properties.end(); j++) {
                    const char* propertyName = *j;
                    if (!subObjVal.Has(propertyName)) {
                        printf("inputs[%d].%s.%s is undefined.\n", i, subProperty, propertyName);
                        return Napi::Number::New(env, -1);
                    }
                }

                HARDWAREINPUT hi;
                MOUSEINPUT mi;
                KEYBDINPUT ki;

                switch (type) {
                    case INPUT_HARDWARE: {
                        Napi::Value uMsg(subObjVal["uMsg"]);
                        Napi::Value wParamL(subObjVal["wParamL"]);
                        Napi::Value wParamH(subObjVal["wParamH"]);
                        if (!uMsg.IsNumber()) {
                            printf("inputs[%d].hi.uMsg should be a number.\n", i);
                            return Napi::Number::New(env, -1);
                        }
                        if (!wParamL.IsNumber()) {
                            printf("inputs[%d].hi.wParamL should be a number.\n", i);
                            return Napi::Number::New(env, -1);
                        }
                        if (!wParamH.IsNumber()) {
                            printf("inputs[%d].hi.wParamH should be a number.\n", i);
                            return Napi::Number::New(env, -1);
                        }
                        hi.uMsg = uMsg.ToNumber().Uint32Value();
                        hi.wParamH = uMsg.ToNumber().Uint32Value();
                        hi.wParamL = uMsg.ToNumber().Uint32Value();
                        current.hi = hi;
                    }
                    case INPUT_MOUSE: {
                        Napi::Value dx(subObjVal["dx"]);
                        Napi::Value dy(subObjVal["dy"]);
                        Napi::Value mouseData(subObjVal["mouseData"]);
                        Napi::Value dwFlags(subObjVal["dwFlags"]);
                        Napi::Value time(subObjVal["time"]);
                        if (!dx.IsNumber()) {
                            printf("inputs[%d].mi.dx should be a number.\n", i);
                            return Napi::Number::New(env, -1);
                        }
                        if (!dy.IsNumber()) {
                            printf("inputs[%d].mi.dy should be a number.\n", i);
                            return Napi::Number::New(env, -1);
                        }
                        if (!mouseData.IsNumber()) {
                            printf("inputs[%d].mi.mouseData should be a number.\n", i);
                            return Napi::Number::New(env, -1);
                        }
                        if (!dwFlags.IsNumber()) {
                            printf("inputs[%d].mi.dwFlags should be a number.\n", i);
                            return Napi::Number::New(env, -1);
                        }
                        if (!time.IsNumber()) {
                            printf("inputs[%d].mi.time should be a number.\n", i);
                            return Napi::Number::New(env, -1);
                        }
                        
                        mi.dx = dx.ToNumber().Int32Value();
                        mi.dy = dy.ToNumber().Int32Value();
                        mi.mouseData = mouseData.ToNumber().Uint32Value();
                        mi.dwFlags = dwFlags.ToNumber().Uint32Value();
                        mi.time = time.ToNumber().Uint32Value();
                        current.mi = mi;
                    }
                    case INPUT_KEYBOARD: {
                        Napi::Value wVk(subObjVal["wVk"]);
                        Napi::Value wScan(subObjVal["wScan"]);
                        Napi::Value dwFlags(subObjVal["dwFlags"]);
                        Napi::Value time(subObjVal["time"]);
                        if (!wVk.IsNumber()) {
                            printf("inputs[%d].ki.wVk should be a number.\n", i);
                            return Napi::Number::New(env, -1);
                        }
                        if (!wScan.IsNumber()) {
                            printf("inputs[%d].ki.wScan should be a number.\n", i);
                            return Napi::Number::New(env, -1);
                        }
                        if (!dwFlags.IsNumber()) {
                            printf("inputs[%d].ki.dwFlags should be a number.\n", i);
                            return Napi::Number::New(env, -1);
                        }
                        if (!time.IsNumber()) {
                            printf("inputs[%d].ki.time should be a number.\n", i);
                            return Napi::Number::New(env, -1);
                        }
                        ki.wVk = wVk.ToNumber().Int32Value();
                        ki.wScan = wScan.ToNumber().Int32Value();
                        ki.dwFlags = dwFlags.ToNumber().Uint32Value();
                        ki.time = time.ToNumber().Uint32Value();
                        current.ki = ki;
                    }
                }
                nativeInputs[i] = current;
            }

            int n = ::SendInput(args.Length(), nativeInputs, sizeof(INPUT));

            return Napi::Number::New(env, n);
        }

        Napi::Value WinhookNapi::SetForegroundWindow(const Napi::CallbackInfo& args) {
            Napi::Env env  = args.Env();
            Napi::HandleScope scope(env);
            
            if (!args[0].IsNumber()) {
                return Napi::Number::New(env, -1);
            }

            
            Napi::Value arg = args[0];
            Napi::Number nArg = arg.ToNumber();

            HWND target = (HWND)nArg.Int64Value();
            bool result = ::SetForegroundWindow(target);

            return Napi::Boolean::New(env, result);
        }


        Napi::Value WinhookNapi::GetForegroundWindow(const Napi::CallbackInfo& args) {
            Napi::Env env  = args.Env();
            Napi::HandleScope scope(env);
            
            int retries = 0;
            HWND target;

            while((target = ::GetForegroundWindow()) == NULL) {
                if (retries > MAX_RETRIES) {
                    return Napi::Number::New(env, -1);
                }
                Sleep(30);
                retries++;
            }

            return Napi::Number::New(env, (unsigned long long)target);
        }

        Napi::Value WinhookNapi::GetLastError(const Napi::CallbackInfo& args) {
            Napi::Env env = args.Env();
            Napi::HandleScope scope(env);
            DWORD lastErr = ::GetLastError();
            return Napi::Number::New(env, lastErr);
        }

        Napi::Value WinhookNapi::CloseHandle(const Napi::CallbackInfo& args) {
            Napi::Env env = args.Env();
            Napi::HandleScope scope(env);
            HANDLE handle = (HANDLE)args[0].ToNumber().Int64Value();
            return Napi::Boolean::New(env, ::CloseHandle(handle));
        }

        Napi::Value WinhookNapi::CloseWindow(const Napi::CallbackInfo& args) {
            Napi::Env env = args.Env();
            Napi::HandleScope scope(env);
            HWND hwnd = (HWND)args[0].ToNumber().Int64Value();
            return Napi::Boolean::New(env, ::CloseWindow(hwnd));
        }

        Napi::Value WinhookNapi::ShowWindow(const Napi::CallbackInfo& args) {
            Napi::Env env = args.Env();
            Napi::HandleScope scope(env);
            HWND hwnd = (HWND)args[0].ToNumber().Int64Value();
            int nShowCmd = args[0].ToNumber().Int32Value();
            return Napi::Boolean::New(env, ::ShowWindow(hwnd, nShowCmd));
        }
    }
}
