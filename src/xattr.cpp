#include <sys/xattr.h>
#include "xattr.h"

class GetXattrAsyncWorker : public Napi::AsyncWorker {
    public:
        GetXattrAsyncWorker(const char* path, const char* name, Napi::Function &callback)
            : Napi::AsyncWorker(callback), path(path), name(name) {}

    private:
        void Execute() {
            size_t len;
            char* buf;

            if (-1 == (ssize_t)(len = getxattr(path.c_str(), name.c_str(), NULL, 0))) {
                Napi::TypeError::New(Env(), "Failed getting xattr").ThrowAsJavaScriptException();
                return;
            }

            buf = (char*)std::malloc(len+1);
            buf[len] = '\0';

            if (-1 == getxattr(path.c_str(), name.c_str(), buf, len)) {
                Napi::TypeError::New(Env(), "Failed getting xattr").ThrowAsJavaScriptException();
            } else {
                value = std::string(buf);
            }

            std::free(buf);
        }

        void OnOK() override {
            Callback().Call({{Env().Null(), Napi::String::New(Env(), value)}});
        }

        void OnError(const Napi::Error &err) override {
            Callback().Call({err.Value(), Env().Undefined()});
        }

        std::string path;
        std::string name;
        std::string value;
};

void getxattr_async(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return;
    } else if (info.Length() < 3 || !info[2].IsFunction()) {
        Napi::TypeError::New(env, "Function expected").ThrowAsJavaScriptException();
        return;
    }

    std::string path = info[0].As<Napi::String>().Utf8Value();
    std::string name = info[1].As<Napi::String>().Utf8Value();
    Napi::Function callback = info[2].As<Napi::Function>();

    (new GetXattrAsyncWorker(path.c_str(), name.c_str(), callback))->Queue();
}

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
        std::free(value);
        return Napi::String();
    }

    std::string valuestr(value);
    std::free(value);

    return Napi::String::New(env, valuestr);
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
    exports.Set("getxattr", Napi::Function::New(env, getxattr_async));
    exports.Set("getxattrSync", Napi::Function::New(env, getxattr_sync));
    return exports;
}

NODE_API_MODULE(xattr, init);
