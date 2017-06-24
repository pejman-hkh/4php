# 4php
Simple php interpreter


1. No syntax check
2. No run time error
3. Not support PHP OOP yet
4. Not support switch statement
5. Not support php casting
6. Not support do while

# About
This simple php interpreter written in c++11 and can be used as template engine in c++ too.

# Porpus
My porpus is converting php code too c++ and compile it at the end

# Writing Extension
writing extension in c++ is very easy

Example :
```cpp
var exten_implode( var &p )  {
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
# Building

```code
g++ -std=c++11 main.cpp -o 4php
```

# Usage
```code
./4php test.php
```


# Update
1. Constant added
2. Globalize in function added
3. Include support added
4. Eval support added
