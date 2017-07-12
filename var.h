#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <initializer_list>
#include <unistd.h>
#include <ctime>
#include <sys/time.h>
#include <functional>
#include <memory>
#include "dtoa_milo.h"
#include "itoa_milo.h"


double microtime() {
    struct timeval time;
    gettimeofday(&time, NULL);
    double microsec = (float)time.tv_usec / (float)1000000 ;//time.tv_sec ;

    return microsec;
}


enum PHP_TYPE {
    PHP_STRING, PHP_INT, PHP_ARRAY, PHP_FUNCTION, PHP_CLASS, PHP_BOOL, PHP_DOUBLE
};


void exit() {
    exit(0);
}

template <typename T>
std::string to_string(T const& value) {
    std::stringstream sstr;
    sstr << value;
    return sstr.str();
}


std::string int_to_string( const int &a ) {
	char ret[11];
	i32toa_sse2( a, ret );
	return ret;  
}

std::string double_to_string( const double &a ) {
	char ret[11];
	dtoa_milo( a, ret );
	return ret;
}


template <typename T>
T to_number( const std::string& str ) {
    std::istringstream ss(str);
    T result;
    ss >> result;
    return result;
}
using namespace std::placeholders;

template<class T>
class refrence {
public:
    refrence() {}  
    int set( const T &t ) {
        vect.push_back( t );
        return vect.size() - 1;
    }

    T &get( int index ) {
        return vect[index];
    }
private:
    std::vector< T > vect; 
};


class var {
public:
    typedef var (*func)(var&);
    typedef var (*func1)(var);
    func function;
    func1 function1;
    std::function <var(var&)> methods;
    std::function <var(var)> methods1;
	friend void echo( var a );
	friend bool empty( var a );

    std::string string() {
    	if( _type == PHP_DOUBLE ){
    		return double_to_string(_int);
    	} else if( _type == PHP_INT ) {

    		return int_to_string( _int );    		
    	}
		else
    		return _string;  
    }

    var(): _type(PHP_STRING), _string("") {}

	var( const std::function <var(var&)> &a ) : _type( PHP_CLASS ), _string("Function"), methods( a ) {}

	var( const std::function <var(var)> &a ) :_type( PHP_CLASS ), _string("Function"), methods1(a) {}

	var( const func &a ) : _type(PHP_FUNCTION), function(a), _string("Function") {}

	var( const func1 &a ) : _type(PHP_FUNCTION), function1(a), _string("Function") {}

	var( const int &a ): _type(PHP_INT), _int( a ) {}

	var( const long int &a ): _type(PHP_INT), _int( a ) {}

	var( const unsigned int &a ): _type(PHP_INT), _int( a ) {}

	var( const long unsigned int &a ): _type(PHP_INT), _int( a ) {}

	var( const float &a ): _type(PHP_DOUBLE), _int( a ) {}

	var( const double &a ): _type(PHP_DOUBLE), _int( a ) {}

	var( const long double &a ): _type(PHP_DOUBLE), _int( a ) {}
	

	var( const std::string &a ): _type(PHP_STRING), _string( a ) {}

	var( const char * const &a ) : _type(PHP_STRING), _string( a ) {}


	var( const unsigned char* const &a ) : _type(PHP_STRING), _string( to_string(a) ) {}

	var( char *a ) : _type(PHP_STRING), _string( a )  {}

    var( const bool &a ) : _type(PHP_INT), _int( a ? 1 : 0 ) {}

	explicit operator bool() 
	{

		switch( _type ) {
			case PHP_STRING : {
				if( ( _string != "0" && _string != "" ) )
					return true;
			} break;


			case PHP_ARRAY : {
				if( this->count() > 0 )
					return true;
			}

			default : {
				if( _int != 0 )
					return true;
			}  break;

		}


	    return false;
	}

    var key( var i ) {
        return keys[atol(i.string().c_str())];
    }
            
    var value( var i ) {
        return data[atol(i.string().c_str())];
    }

	void unset( var index = "" ) {
		if( index == "") {
			keys.clear();
			data.clear();
		} else {
			for( int i = 0; i < keys.size(); i++ ) {
				if( keys[i].string() == index.string() ) {
					keys.erase( keys.begin() + i );
					data.erase( data.begin() + i );
				}
			}			
		}
	}

	func get() {
		return function;
	}

	func1 get_s() {
		return function1;
	}

	std::function <var(var&)> get_m() {
		return methods;
	}

	std::function <var(var)> get_ms() {
		return methods1;
	}


    var explode( std::string delim ) {
	    var out;
		size_t pos = 0;
		size_t start = 0;
		int i = 0;
		while ( ( pos = string().find( delim, pos ) ) != std::string::npos ) {

			out[ i++ ] = string().substr( start, ( pos - start ) );

			pos += delim.length();

			start = pos;

		}	
	
		out[ i ] = string().substr( start, string().length() );
		return std::move( out );

    }

    var replace( var a )
	{

		for( int i = 0; i < a.size(); i++ ) {
			
			size_t pos = 0;
    		while ( ( pos = string().find( a.key( i ).string(), pos ) ) != std::string::npos ) {
				string().replace( pos, a.key( i ).string().length(), a.value( i ).string() );
				pos += a.value( i ).string().length();
			}	
		}

		return string();
	}

    
	bool isset( var index ) {
		for( int i = 0; i < keys.size(); i++ ) {

     		if( index._type == keys[i]._type ) {
        		if( index._type == PHP_STRING ) {
        			if( index._string == keys[i]._string )
        				return true;
        		} else {
        			return index._int == keys[i]._int;
        		}
        	}
        	else {
        		if(  index.string() == keys[i].string() )
                	return true;
            }

		}

		return false;			
	}

    

	var type() {
		return _type;
	}
	
    var* begin() {
        return &keys[0];
    }
    
    var* end() {
    	return &keys[ keys.size() ];
    }

    var in_array( var arr ) {
   		for( auto x : arr ) {
   			if( string() == arr[x].string() ) {
   				return true;
   			}

   		}
    	return false;
    }

    int count() {
        if( _type == PHP_ARRAY ) {
            return keys.size();
        } else {
            return 0;
        }
    }

    int length() {
        return string().size();
    }

	double to_num() {
		return _int;
	}

	int to_int() {
		return (int)_int;
	}

    int size() {
       return count();
    }


	var to_kv() {
		var out;
		for( int i = 0; i < keys.size(); ++i ) {
			var value = data[i];

			if( value[0]._type == PHP_ARRAY ) {
				for( auto x : value ) {
					out[ i ][ value[x][0] ] = value[x][1];
				}
			} else {
				var val = data[i];
				if( val._type == PHP_ARRAY ) {
					out[ val[0] ] = val[1];
				} else {
					out[i] = val;
				}	
			}
		}
		
		return out;
	}

	var( std::initializer_list<var> a ) {
		_type = PHP_ARRAY;

		int i = 0;
		for (auto n : a ) {
			keys.push_back( i );
			data.push_back( n );
			i++;
		}

	}

	var concat( var a ) {

        _string =  string() + a.string();

		return _string;
	}


	var operator+( var a ) {
   		var out;
   		if( _type == PHP_STRING || a._type == PHP_STRING ) {
        	out = string() + a.string();
        } else {
       		out = _int + a._int;
       	}

		return out;
	}	


	var operator-( var a ) {
		return _int - a._int;
	}

	var operator+=( var a ) {
        var out;

        if( _type == PHP_STRING || a._type == PHP_STRING ) {

        	_string = string() + a.string();

        	out = _string;
        } else {
        	_int = _int + a._int;
        	out = _int;
        }

		return out;

	}

	var operator-=( var a ) {

		_int = _int - a._int;

		return _int;

	}

	var operator*=( var a ) {

		_int = _int * a._int;

		return _int;

	}

	var operator/=( var a ) {
	
		_int = _int / a._int;

		return _int;
	}

	var operator%=( var a ) {
		_int = (int)_int % (int)a._int;

		return _int;
	}

    int operator++() {
        _int = _int + 1;
    
        return _int;
    }
        
    var operator*(var a) {
		return _int * a._int;
    }

	var operator/(var a) {
		return _int / a._int;
	}
		     
	bool operator&&( var a ) {
		
		
		return _int && a._int;
	}	

	bool operator||( var a ) {
		return _int || a._int;
	}	


	bool operator<( var a ) {
	
		return _int < a._int;
	}
				     
	bool operator>( var a ) {
		return _int > a._int;
	}
		     
	bool operator<=( var a ) {
		return _int <= a._int;
	}

	bool operator>=( var a ) {
		return _int >= a._int;
	}	
		     
	bool operator==( const char * const &a ) {
		return a == string();
	}

	bool operator==( const std::string &a ) {
		
		return a == string();
	}
		     
	bool operator==( const int &a ) {
		if( _type != PHP_STRING ){
			return a == _int;
		}
		
		return false;
	}

	bool operator==( var a ) {
		if( _type == a._type ){
			if( _type == PHP_STRING ){
				return _string == a._string;
			} else {
				return _int == a._int ;
			}
		}
		
		return false;
	}	

	bool operator!=( var a ) {
		return _int != a._int;
	}

	var &operator[]( const std::string &a ) {

		_type = PHP_ARRAY;

        int i = 0;
        for(i = 0;i < keys.size(); ++i)
        {
        	if(  a == keys[i].string() )
                return data[i];
        }

        var temp;
	    keys.push_back( a );
        data.push_back( temp );

		return data[i];		
	}

	var &operator[]( const char * const &a ) {

		_type = PHP_ARRAY;

        int i = 0;
        for(i = 0;i < keys.size(); ++i)
        {
        	if(  a == keys[i].string() )
                return data[i];
        }

        var temp;
	    keys.push_back( a );
        data.push_back( temp );

		return data[i];		
	}

	var &operator[]( const int &a ) {

		_type = PHP_ARRAY;

	
        int i = 0;
        for(i = 0;i < keys.size(); ++i)
        {
        	if( keys[i]._type != PHP_STRING ){
        		if(  a == keys[i]._int )
	                return data[i]; 
        	} else {
        		if(  int_to_string( a ) == keys[i].string() )
                	return data[i];		         	     		
        	}

        }

        var temp;

	    keys.push_back( a );
        data.push_back( temp );

		return data[i];		
	}

	var &operator[]( const double &a ) {

		_type = PHP_ARRAY;

        int i = 0;
        for(i = 0;i < keys.size(); ++i)
        {
        	if( keys[i]._type != PHP_STRING ){
        		if(  a == keys[i]._int )
	                return data[i];
        	} else {
        		if(  double_to_string( a ) == keys[i].string() )
                	return data[i];		         	      		
        	}
        }

        var temp;
	    keys.push_back( a );
        data.push_back( temp );

		return data[i];		
	}

	var &operator[]( var a ) {

		_type = PHP_ARRAY;

        int i = 0;
        for(i = 0;i < keys.size(); ++i)
        {
        	if( _type == keys[i]._type ) {
        		if( _type == PHP_STRING ) {
        			if( _string == keys[i]._string )
        				return data[i];
        		} else {
        			if( _int == keys[i]._int )
        				return data[i];
        		}
        	}
        	else {
        		if(  a.string() == keys[i].string() )
                	return data[i];
            }
        }

        var temp;
	    keys.push_back( a );
        data.push_back( temp );

		return data[i];		
	}

	~var() {

	}


private:
	std::string _string;
	int _type;
	double _int;

    std::vector<var> keys;
    std::vector<var> data;
};


void echo( var i ) {
    if( i._type == PHP_ARRAY ) {
        std::cout << "Array" << std::endl;
    } else {
        std::cout << i.string();
    }
}


bool empty( var a ) {
    if( a.string() == "" || a.string() == "0" ) {
        return true;
    }
    
    return false;
}

void print_r( var a, std::string &ret_str, const std::string &tab = "" ) {
    if( a.type() == PHP_ARRAY ) {
        int i = 0;
     
        ret_str += "Array (\n";
    
        for( auto x : a ) {
			
			ret_str += tab + "    [" + x.string() + "] => ";
		
            if( a[x].type() == PHP_ARRAY ) {
                print_r( a[x], ret_str, tab + "    " );
            } else {
                ret_str += a[x].string() +"\n";
            }
        }

        ret_str += tab + ")\n";
    } else if ( a.type() == PHP_FUNCTION ) {
    	ret_str += "function";
    } else {
        ret_str += a.string();
    }
}


void print_r( const var &a ) {
	std::string out;
	print_r( a, out );
	std::cout << out;
}

static var classes;
static var functions;
static var constants;