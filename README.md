The **sys-xattr** module is a Node.js native plugin that wraps the Linux system
calls for reading and writing extended filesystem attributes.  Unlike most
modules written to address this concern, the **sys-xttr** module exposes all of
the available *xattr* functions from `sys/xattr.h`.  Specifically, this allows
Node.js apps to use the `fsetxattr`, `fgetxattr`, and `fremovexattr` functions,
which can be used to avoid race conditions when reading both attributes and
data from a file.

sys-xattr Node.js Module
========================

Installation
------------
```sh
npm install sys-xattr
```

Usage
-----
```js
const {getxattr} = require("sys-xattr");

getxattr("/path/to/file", "user.attrib-name", (err, attr) => {
    if (err) console.error("could not read extended attribute");
    else console.log("user.attrib-name is", attr);
});
```

API
---

### getxattr(path, attr, done)
Read extended file system attribute `attr` from the file system at `path` and
then call `done(err, value)`.

##### Arguments
**path** *<string>* File system path from which to read attribute.  
**attr** *<string>* Name of extended attribute to read.  
**done** *<function>* Callback called with `(err, value)`  

### getxattrSync(path, attr): string
Read extended file system attribute `attr` from the file system at `path`.

##### Arguments
**path** *<string>* File system path from which to read attribute.  
**attr** *<string>* Name of extended attribute to read.  

##### Returns
Returns the attribute value, or undefined if the attribute value is not set.
