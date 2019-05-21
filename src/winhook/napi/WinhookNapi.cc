#include "WinhookNapi.h"

namespace winhook {
    namespace napi {
        Napi::FunctionReference WinhookNapi::constructor;
        MyCompareObjectHandles* WinhookNapi::compareFn;

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
                DWORD err = ::GetLastError();
                // use FormatMessage
                Napi::String sErr = Napi::Number::New(env, err).ToString();
                Napi::String errMsg = Napi::String::New(env, "Creating Process failed. Error Code: ");
                std::string tmp = std::string(errMsg) + std::string(sErr);
                Napi::TypeError::New(env, tmp).ThrowAsJavaScriptException();
            }

            ::CloseHandle(pi.hProcess);
            ::CloseHandle(pi.hThread);
            
            return Napi::Number::New(env, 0);
        }

        Napi::Value WinhookNapi::FindWindow(const Napi::CallbackInfo& args) {
            Napi::Env env = args.Env();
            Napi::HandleScope scope(env);
            if (!args[0].IsString()) {
                Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
            }
            Napi::String windowName = args[0].As<Napi::String>();
            HWND hwnd;
            int retries = 0;
            while((hwnd = ::FindWindowA(NULL, windowName.Utf8Value().c_str())) == NULL) {
                if (retries > MAX_RETRIES) {
                    Napi::TypeError::New(env, "Could not find specified window.").ThrowAsJavaScriptException();
                }
                Sleep(30);
                retries++;
            }

            int index = _GetHandleIndex(hwnd);

            if (index == -1) {
                handles.push_back(hwnd);
                return Napi::Boolean::New(env, false);
            }

            return Napi::Boolean::New(env, true);
        }
        
        Napi::Value WinhookNapi::SendInput(const Napi::CallbackInfo& args) {
            Napi::Env env  = args.Env();
            Napi::HandleScope scope(env);

            INPUT *nativeInputs = (INPUT*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(INPUT) * args.Length());

            for (int i = 0; i < args.Length(); i++) {
                if (!args[i].IsObject()) {
                    // use FormatMessage
                    Napi::TypeError::New(env, "Expected an instance of Input").ThrowAsJavaScriptException();
                }
                input::InputNapi* inputWrap = Napi::ObjectWrap<input::InputNapi>::Unwrap(args[i].ToObject());
                if (inputWrap == NULL) {
                    Napi::TypeError::New(env, "Input malformed").ThrowAsJavaScriptException();
                }
                winhook::input::Input *input = inputWrap->GetInputInstance();

                switch(input->type) {
                    case INPUT_KEYBOARD: {
                        printf("KI. wVk=%x wScan=%x dwFlags=%x time=%d\n", input->ki->wVk, input->ki->wScan, input->ki->dwFlags, input->ki->time);
                    }
                    break;
                }

                input->raw(&nativeInputs[i]);

                switch(input->type) {
                    case INPUT_KEYBOARD: {
                        printf("KI. wVk=%x wScan=%x dwFlags=%x time=%d\n", nativeInputs[i].ki.wVk, nativeInputs[i].ki.wScan, nativeInputs[i].ki.dwFlags, nativeInputs[i].ki.time);
                    }
                    break;
                }


            }

            int n = ::SendInput(args.Length(), nativeInputs, sizeof(INPUT));

            return Napi::Number::New(env, n);
        }

        Napi::Value WinhookNapi::SetForegroundWindow(const Napi::CallbackInfo& args) {
            Napi::Env env  = args.Env();
            Napi::HandleScope scope(env);
            
            if (!args[0].IsNumber()) {
                Napi::TypeError::New(env, "Expected an index of a window").ThrowAsJavaScriptException();
            }
            int index = args[0].ToNumber().Int32Value();
            if (index >= handles.max_size()) {
                Napi::TypeError::New(env, "Window index out of range").ThrowAsJavaScriptException();
            }
            HWND target = (HWND)handles[index];
            return Napi::Boolean::New(env, ::SetForegroundWindow(target));
        }


        Napi::Value WinhookNapi::GetForegroundWindow(const Napi::CallbackInfo& args) {
            Napi::Env env  = args.Env();
            Napi::HandleScope scope(env);
            
            int retries = 0;
            HWND target;

            while((target = ::GetForegroundWindow()) == NULL) {
                if (retries > MAX_RETRIES) {
                    Napi::TypeError::New(env, "Could not find the current foreground window.").ThrowAsJavaScriptException();
                }
                Sleep(30);
                retries++;
            }

            int index = _GetHandleIndex(target);

            if (index == -1) {
                handles.push_back(target);
                index = handles.size() - 1;
            }

            return Napi::Number::New(env, index);
        }

        Napi::Value WinhookNapi::GetLastError(const Napi::CallbackInfo& args) {
            Napi::Env env = args.Env();
            Napi::HandleScope scope(env);
            DWORD lastErr = ::GetLastError();
            return Napi::Number::New(env, lastErr);
        }

        int WinhookNapi::_GetHandleIndex(HANDLE target) {
            bool found = false;
            int index = 0;
            int sz = handles.size();
            for (; index < sz; ++index) {
                if (target == handles[index]) {
                    return index;
                }
            }
            return -1;
        }

    }
}
