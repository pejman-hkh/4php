//define new class
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

//init class
var init_class_test( var &params ) {

    std::shared_ptr<test> a = std::make_shared<test>();

    var out;
    //add method pejmanhkh to class test ...
    out["pejmanhkh"] = std::function <var(var&)>( std::bind( &test::pejmanhkh, a, _1 ) );

    //add method sss to class test ...
   	out["ppp"] = std::function <var(var&)>( std::bind( &test::ppp, a, _1 ) );

    return out;
}