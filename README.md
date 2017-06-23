# 4php
Simple php interpreter


No syntax check
No run time error
Not support PHP OOP yet
Not support switch statement
Not support php casting

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
