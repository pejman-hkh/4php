# 4php
Simple php interpreter Inspire from Binaryphp

1. No syntax check
2. No run time error
3. Not support PHP Class defination ( OOP supported )
4. Not support switch statement
5. Not support php casting
6. Not support do while

# About
This simple php interpreter written in c++11 and can be used as template engine in c++ too.

# Porpus
My porpus is converting php code too c++ and compile it at the end

# OOP
You can write c++ class and add call it in 4php

See extension directory

```c++
class test : var {
private:
	int z = 10;
public:
	test() {
		std::cout << "construced" << std::endl;
	}

    var pejmanhkh( var &p ) {
    	print_r( p );
    	std::cout << "z is " << z << std::endl;
    	z = 4;
    	return 0;
    }

    var ppp( var &p ) {
    	std::cout << "z is " << z << std::endl;
    	return 0;
    }

    ~test() {
    	std::cout << "destructed" << std::endl;
    }
};

```

Usage : 
```php
<?4php

$a = new test();

print_r( $a );

$a->pejmanhkh( "test", 1, 2 );

echo("dadsas\n");

$a->ppp( "test" );


echo("dadsas\n");

$aa = new test();
$aa->pejmanhkh( "test", 1, 2 );


?>
```

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

# Building

```code
g++ -std=c++11 main.cpp  -o 4php -ldl
```

# Usage
```code
./4php test.php
```

# Web programming
You can use 4php in cgi with shebang

Request function make web request array & you can set superglobal functions with $SUPERGLOBALS variable and use it in function scope too.

```php
#!4php
<?
echo("Content-type: text/html\r\n\r\n");

$request = request();

//define super global function in 4php
$SUPERGLOBALS['_GET'] = $request['get'];
$SUPERGLOBALS['_POST'] = $request['post'];
$SUPERGLOBALS['_SERVER'] = $request['server'];
print_r( $_SERVER );

function test() {
	print_r( $_SERVER );
}

test();
?>
```

# Update
1. Constant added
2. Globalize in function added
3. Include support added
4. Eval support added
5. Define superglobal variable
6. Web programming added ( cgi with shebang )
7. Not showing shebang added
8. Update variable set and add .= and += & ... operator
