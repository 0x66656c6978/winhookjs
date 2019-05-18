#include <iostream>
#include <napi.h>

#include "winhook/napi/WinhookNapi.h"
#include "winhook/napi/HwndNapi.h"
#include "winhook/napi/input/InputNapi.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  winhook::napi::WinhookNapi::Init(env, exports);
  winhook::napi::HwndNapi::Init(env, exports);
  winhook::napi::input::InputNapi::Init(env, exports);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll)