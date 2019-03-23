#include <sys/xattr.h>
#include "xattr.h"

Napi::String getxattr_sync(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return Napi::String();
    }

    size_t len;
    char* value;

    std::string pathstr = info[0].As<Napi::String>().Utf8Value();
    std::string namestr = info[1].As<Napi::String>().Utf8Value();

    const char* path = pathstr.c_str();
    const char* name = namestr.c_str();

    if (-1 == (ssize_t)(len = getxattr(path, name, NULL, 0))) {
        Napi::TypeError::New(env, "Failed getting xattr").ThrowAsJavaScriptException();
        return Napi::String();
    }

    value = (char*)std::malloc(len+1);
    value[len] = '\0';

    if (-1 == getxattr(path, name, value, len)) {
        Napi::TypeError::New(env, "Failed getting xattr").ThrowAsJavaScriptException();
        return Napi::String();
    }

    std::string valuestr(value);
    std::free(value);

    return Napi::String::New(env, valuestr);
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
    exports.Set("getxattrSync", Napi::Function::New(env, getxattr_sync));
    return exports;
}

NODE_API_MODULE(xattr, init);
