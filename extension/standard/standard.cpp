#include "var.h"
#include "base64.h"
#include "request.h"
#include <cmath>

var exten_sum( var &p ) {
    var out = 0;
    for( auto x : p ) {
        out += p[x];
    }
    return out;
}


var exten_echo( var &p ) {
    echo( p[0] );
    
    return 0;
}

var exten_print_r( var &p ) {

    if( p[1] == "true" ) {
        std::string out;
        print_r( p[0], out );
        return out;
    }
    else 
       print_r( p[0] );
    
    return 0;
}

var exten_is_array( var &p ) {

    if( p[0].type() == "array" )
       return true;
   
    return false;
}

var exten_type( var &p ) {
    return p[0].type();
}

var exten_count( var &p ) {
    if( p[0].type() == "array" ) {
        return p[0].count();
    }

    return 0;
}

var exten_microtime( var &p ) {
    struct timeval time;
    gettimeofday(&time, NULL);
    double microsec = (float)time.tv_usec / (float)1000000 ;//time.tv_sec ;

    return microsec;
}


var exten_usleep( var &p ) {
    long t = p[0].to_num();
    std::cout.flush();

    usleep( t );
    return 0;
}

var exten_time( var &p ) {

    return std::time(0);
}

var exten_round( var &p ) {

    return round( p[0].to_num() );
}

var exten_exit( var &p )  {
    exit(0);
    return 0;
}

var exten_die( var &p )  {
    echo( p[0] );
    exit(0);
    return 0;
}

var exten_implode( var &p )  {
    var out = "";
    var pre = "";
    for( auto x : p[1] ) {
        out += pre+p[1][x];
        pre = p[0];
    }

    return out;
}


var exten_get_defined_functions( var &p )  {
    var internal;
    int i = 0;
    for( auto x : functions ) {
        internal[ i++ ] = x;

    }

    var out;
    out["internal"] = internal; 

    return out;
}


var exten_explode( var &p ) {
    var out;
    size_t pos = 0;
    size_t start = 0;
    std::string container = p[1].string();
    std::string delim = p[0].string();

    int i = 0;
    while ( ( pos = container.find( delim, pos ) ) != std::string::npos ) {

        out[ i++ ] = container.substr( start, ( pos - start ) );

        pos += delim.length();

        start = pos;

    }   

    out[ i ] = container.substr( start, container.length() );
    return std::move( out );

}

var exten_base64_encode( var &p )  {

    std::string str = p[0].string();
    return php_base64_encode( str, str.length() );
}


var exten_base64_decode( var &p )  {
    std::string str = p[0].string();
    return php_base64_decode( str, str.length() );
}


var exten_request( var &p )  {
    request req;

    var out;
    out["post"] = req.post();
    out["get"] = req.get();
    out["server"] = req.server();

    return out;
}


extern "C" {
    //init class
   /* var load_constants( var p ) {

        return constants;
    }
*/
    var load_functions( var p ) {
        var out;
        out["sum"] = exten_sum;
        out["echo"] = exten_echo;
        out["print"] = exten_echo;
        out["print_r"] = exten_print_r;
        out["is_array"] = exten_is_array;
        out["type"] = exten_type;
        out["count"] = exten_count;
        out["microtime"] = exten_microtime;
        out["time"] = exten_time;
        out["round"] = exten_round;
        out["usleep"] = exten_usleep;
        out["exit"] = exten_exit;
        out["die"] = exten_die;
        out["implode"] = exten_implode;
        out["explode"] = exten_explode;
        out["get_defined_functions"] = exten_get_defined_functions;

        out["base64_encode"] = exten_base64_encode;
        out["base64_decode"] = exten_base64_decode;
        out["request"] = exten_request;

        return out;
    }

}

