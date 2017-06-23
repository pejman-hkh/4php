# 4php
Simple php interpreter


1. No syntax check
2. No run time error
3. Not support PHP OOP yet
4. Not support switch statement
5. Not support php casting

# About
This simple php interpreter written in c++11 and can be used as template engine in c++ too.

# Writing Extension
writing extension in c++ is very easy

Example :
```cpp
var exten_implode( var p )  {
    var out = "";
    var pre = "";
    for( auto x : p[1] ) {
        out += pre+p[1][x];
        pre = p[0];
    }

    return out;
}
```
And add it to functions :

```cpp
functions["implode"] = exten_implode;
```
