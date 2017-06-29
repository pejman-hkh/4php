#include "var.h"

class test : var {
private:
	int z = 10;
public:
	test( var a ) {
		print_r( a );
		std::cout << "construced" << std::endl;
	}

    var pejmanhkh( var &p ) {
    	print_r( p );
    	std::cout << "z is " << z << std::endl;
    	z = 4;
    	return { "out", "out1" };
    }

    var ppp( var &p ) {
    	std::cout << "z is " << z << std::endl;
    	return 0;
    }

    ~test() {
    	std::cout << "destructed" << std::endl;
    }
};

extern "C" {

    var load_functions( var p ) {

        return 0;
    }

    
    //init class
    var load( var &params ) {

        std::shared_ptr<test> a = std::make_shared<test>( params );

        var out;
        //add method pejmanhkh to class test ...
        out["pejmanhkh"] = std::function <var(var&)>( std::bind( &test::pejmanhkh, a, _1 ) );

        //add method sss to class test ...
       	out["ppp"] = std::function <var(var&)>( std::bind( &test::ppp, a, _1 ) );

        return out;
    }    
}