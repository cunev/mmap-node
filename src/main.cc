#include <MemoryModule/Loader.h>
#include <blackbone/manualmap/mmap.h>
#include <blackbone/process/process.h>
#include <memory>
#include <msdia_data.h>
#include <napi.h>
#include <stdexcept>
#include <string>

using namespace blackbone;

namespace {
bool is_msdia_loaded = false;
}

Napi::Value mmap(const Napi::CallbackInfo &args) {
  if (!is_msdia_loaded) {
    HMEMORYMODULE module;
    LdrLoadDllMemoryExW(&module, nullptr, 0, msdia_data, 0, L"msdia140.dll",
                        nullptr);
    is_msdia_loaded = true;
  }

  Napi::Env env = args.Env();
  if (args.Length() < 2) {
    Napi::Error::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  auto handle =
      reinterpret_cast<HANDLE>(args[0].As<Napi::Number>().Int32Value());
  auto buffer = args[1].As<Napi::Uint8Array>();

  return Napi::Boolean::From(
      env, MMap::MapImage(handle, buffer.ByteLength(), buffer.Data()));
}

Napi::Value openProcess(const Napi::CallbackInfo &args) {
  Napi::Env env = args.Env();

  if (args.Length() < 1) {
    Napi::Error::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  const auto windowName = args[0].As<Napi::String>().Utf8Value();

  DWORD pid = 0;
  GetWindowThreadProcessId(FindWindowA(NULL, windowName.c_str()), &pid);
  auto process = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);

  return Napi::Number::From(env, reinterpret_cast<uint32_t>(process));
}

Napi::Value closeHandle(const Napi::CallbackInfo &args) {
  Napi::Env env = args.Env();

  if (args.Length() < 1) {
    Napi::Error::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  auto handle =
      reinterpret_cast<HANDLE>(args[0].As<Napi::Number>().Int32Value());

  CloseHandle(handle);

  return env.Undefined();
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
  exports["mmap"] = Napi::Function::New(env, mmap);
  exports["openProcess"] = Napi::Function::New(env, openProcess);
  exports["closeHandle"] = Napi::Function::New(env, closeHandle);
  return exports;
}

NODE_API_MODULE(addon, init)