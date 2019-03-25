#ifndef XATTR_H
#define XATTR_H

#include <napi.h>

void getxattr_async(const Napi::CallbackInfo&);
Napi::String getxattr_sync(const Napi::CallbackInfo&);

#endif
