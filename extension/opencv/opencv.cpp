#include "opencv2/opencv.hpp"
#include "var.h"


class opencv : var {
private:
	int z = 10;
    refrence<cv::Mat> ref;
public:
	opencv() {
		//std::cout << "construced" << std::endl;
	}

    var imread( var &p ) {
        cv::Mat src = cv::imread( p[0].string(), 1 );
        return ref.set( src );
    }

    var threshold( var &p ) {
        cv::Mat dst;
        cv::threshold( ref.get( p[0].to_int() ), dst, p[1].to_int(), p[2].to_int(),p[3].to_int() ); 
        return ref.set( dst );
    }

    var imwrite( var &p ) {
        cv::imwrite( p[0].string(), ref.get( p[1].to_int() ) );
    }

    ~opencv() {
    	//std::cout << "destructed" << std::endl;
    }
};


var opencv_version( var &p ) {
    return "4.1.9";
}

extern "C" {
    //init class

    var load_functions( var p ) {
        var out;
        out["opencv_version"] = opencv_version;
        return out;
    }

    var load( var &params ) {

        std::shared_ptr<opencv> a = std::make_shared<opencv>();

        var out;

        out["imread"] = std::function <var(var&)>( std::bind( &opencv::imread, a, _1 ) );
        out["threshold"] = std::function <var(var&)>( std::bind( &opencv::threshold, a, _1 ) );
       	out["imwrite"] = std::function <var(var&)>( std::bind( &opencv::imwrite, a, _1 ) );

        return out;
    }
}