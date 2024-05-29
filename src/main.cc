#include <blackbone/manualmap/mmap.h>
#include <blackbone/process/process.h>
#include <memory>
#include <napi.h>
#include <stdexcept>
#include <string>

using namespace blackbone;

Napi::Value mmap(const Napi::CallbackInfo &args) {
  Napi::Env env = args.Env();
  if (args.Length() < 2) {
    Napi::Error::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  auto handle =
      reinterpret_cast<HANDLE>(args[0].As<Napi::Number>().Int32Value());
  auto buffer = args[1].As<Napi::Uint8Array>();

  Process process;
  if (process.Attach(handle) != STATUS_SUCCESS) {
    return Napi::Boolean::From(env, false);
  }
  const auto mmap_result = process.mmap().MapImage(
      buffer.ByteLength(), buffer.Data(), false, ManualImports | WipeHeader);
  if (!mmap_result.success()) {
    return Napi::Boolean::From(env, false);
  }
  if (process.Detach() != STATUS_SUCCESS) {
    return Napi::Boolean::From(env, false);
  }

  return Napi::Boolean::From(env, true);
}

Napi::Value openProcess(const Napi::CallbackInfo &args) {
  Napi::Env env = args.Env();

  return Napi::Number::From(env, reinterpret_cast<uint32_t>(OpenProcess(
                                     PROCESS_ALL_ACCESS, false,
                                     args[0].As<Napi::Number>().Int32Value())));
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
  exports["mmap"] = Napi::Function::New(env, mmap);
  exports["openProcess"] = Napi::Function::New(env, openProcess);
  return exports;
}

NODE_API_MODULE(addon, init)