#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <initializer_list>
#include <unistd.h>
#include <map>
#include <ctime>
#include <sys/time.h>

template <typename T>
std::string to_string(T const& value) {
    std::stringstream sstr;
    sstr << value;
    return sstr.str();
}

template <typename T>
T to_number( const std::string& str ) {
    std::istringstream ss(str);
    T result;
    ss >> result;
    return result;
}

class var {
public:
	friend void echo( var a );
	friend bool empty( var a );
	void unset( var index = "" ) {
		if( index == "") {
			keys.clear();
			data.clear();
		} else {
			for( int i = 0; i < keys.size(); i++ ) {
				if( keys[i].container == index.container ) {
					keys.erase( keys.begin() + i );
					data.erase( data.begin() + i );
				}
			}			
		}
	}

	var &sort() {

		var temp_k;
		var temp_v;
			
		for( int i = 0; i < keys.size(); i++ ) {
			for( int j = 0; j < keys.size(); j++ ) {
				if( to_number<int>( keys[i].container ) < to_number<int>( keys[j].container ) ) {
					temp_k = keys[i];
					temp_v = data[i];
					data[i] = data[j];
					keys[i] = keys[j];
					data[j] = temp_v;
					keys[j] = temp_k;
				}
			}
		}
		
		return *this;
	}

    var explode( std::string delim ) {
	    var out;
		size_t pos = 0;
		size_t start = 0;
		int i = 0;
		while ( ( pos = container.find( delim, pos ) ) != std::string::npos ) {

			out[ i++ ] = container.substr( start, ( pos - start ) );

			pos += delim.length();

			start = pos;

		}	
	
		out[ i ] = container.substr( start, container.length() );
		return std::move( out );

    }
    
	bool isset( var index ) {
		for( int i = 0; i < keys.size(); i++ ) {
			if( keys[i].container == index.container ) {
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

    var key( var i ) {
        return keys[atol(i.container.c_str())];
    }
            
    var value( var i ) {
        return data[atol(i.container.c_str())];
    }

    std::string string() {
    	return container;
    }

    var in_array( var arr ) {
   		for( auto x : arr ) {
   			if( container == arr[x].container ) {
   				return true;
   			}

   		}
    	return false;
    }

    int count() {
        if( _type == "array" ) {
            return keys.size();
        } else {
            return 0;
        }
    }

    int length() {
        return container.size();
    }

	var replace( var a )
	{

		for( int i = 0; i < a.size(); i++ ) {
			
			size_t pos = 0;
    		while ( ( pos = container.find( a.key( i ).container, pos ) ) != std::string::npos ) {
				container.replace( pos, a.key( i ).container.length(), a.value( i ).container );
				pos += a.value( i ).container.length();
			}	
		}

		return container;
	}

	int to_int() {
		return to_number<int>( container );
	}	

	double to_num() {
		return to_number<double>( container );
	}

	long to_long() {
		return to_number<long>( container );
	}

    int size() {
       return count();
    }

    var() : _pos(0) {
    	_type = "string";
        container = "";
    }

	var( int a ) {
		_type = "int";
		container = to_string( a );
	}

	var( long int a ) {
		_type = "int";
		container = to_string( a );
	}

	var( unsigned int a ) {
		_type = "int";
		container = to_string( a );
	}

	var( long unsigned int a ) {
		_type = "int";
		container = to_string( a );
	}

	var( std::string a ) {
		_type = "string";
		container = a;
	}

	var( const char *a ) {
       	_type = "string";
		container = to_string( a );
	}

	var( const unsigned char*a ) {
       	_type = "string";
		container = to_string( a );
	}

	var( char *a ) {
		_type = "string";
		container = to_string( a );
	}

	var( float a ) {
		_type = "float";
		container = to_string( a );
	}

	var( double a ) {
		_type = "double";
		container = to_string( a );
	}

	var( long double a ) {
		_type = "double";
		container = to_string( a );
	}

	var to_kv() {
		var out;
		for( int i = 0; i < keys.size(); ++i ) {
			var value = data[i];

			if( value[0]._type == "array" ) {
				for( auto x : value ) {
					out[ i ][ value[x][0] ] = value[x][1];
				}
			} else {
				var val = data[i];
				if( val._type == "array" ) {
					out[ val[0] ] = val[1];
				} else {
					out[i] = val;
				}	
			}
		}

		return out;
	}

	var( std::initializer_list<var> a ) {
		_type = "array";

		int i = 0;
		for (auto n : a ) {
			keys.push_back( i );
			data.push_back( n );
			i++;
		}
	}

	var concat( var a ) {
        var out;
        out = container + a.container;
     	
        container =  out.string();

		return out;
	}


	var operator+( var a ) {
   		var out;
   		if( _type == "string" && a._type == "string" ) {
        	out = container + a.container;
        } else {
       		out = to_number<double>( container ) + to_number<double>( a.container );
       	}
       		
        container =  out.string();

		return out;
	}	

    var( bool a ) {
    	_type = "bool";
        if( a )
            container = "1";
        else
            container = "0";
        
    }

	explicit operator bool() const 
	{

		if( container != "0" ) {
			return true;
		}

	    return false;
	}


	var operator-( var a ) {

		var out = to_number<double>( container ) - to_number<double>( a.container );
		container = out.string();
		return out;

	}

	var operator+=( var a ) {
        var out;

        if( _type == "string" && a._type == "string" ) {

        	out = container + a.container;
        } else {
        	out = to_number<double>( container ) + to_number<double>( a.container );
        }
        
        container =  out.string();

		return out;

	}

	var operator-=( var a ) {

		var out = to_number<double>( container ) - to_number<double>( a.container );
		container = out.string();
		return out;

	}

	var operator*=( var a ) {
		var out = to_number<double>( container ) * to_number<double>( a.container );
		container = out.string();
		return out;

	}

	var operator/=( var a ) {
		var out = to_number<double>( container ) / to_number<double>( a.container );
		container = out.string();
		return out;
	}



    int operator++() {
        int ret = atol(container.c_str()) + 1;
        container = to_string(ret);
        ++_pos;
        return ret;
    }
        
    var operator*(var a) {
        std::string out;
        if( _type == "int" || _type == "double" ) {
            out = to_string( std::stoi( a.container ) * std::stoi( container ) );
        }
        
        container = out;

        return out;
    }

	var operator/(var a) {
		return (var)( to_number<int>( a.container ) * to_number<int>( container ) );
	}
		     
	bool operator&&( var a ) {
		
		if(  to_number<double>( container ) && to_number<double>( a.container ) ) {
			return true;
		}
		return false;
	}	

	bool operator||( var a ) {
		
		if(  to_number<double>( container ) || to_number<double>( a.container ) ) {
			return true;
		}
		return false;
	}	


	bool operator<( var a ) {
		
		if( to_number<double>( container ) <  to_number<double>( a.container )  ) {
			return true;
		}
		return false;
	}
				     
	bool operator>( var a ) {
		if( to_number<double>( container ) >  to_number<double>( a.container )  ) {
			return true;
		}
		return false;
	}
		     
	bool operator<=( var a ) {
		if( to_number<double>( container ) <=  to_number<double>( a.container )  ) {
			return true;
		}
		return false;
	}

	bool operator>=( var a ) {
		if( to_number<double>( container ) >=  to_number<double>( a.container )  ) {
			return true;
		}
		return false;
	}	
		     
	bool operator==( var a ) {

		if( a.container == container && a._type == _type  ) {
			return true;
		}
		return false;
	}	

	bool operator!=( var a ) {
		if( a.container != container ) {
			return true;
		}
		return false;
	}

	var &operator[]( var a ) {

		_type = "array";

        int i = 0;
        for(i = 0;i < keys.size(); ++i)
        {
            if( a.container.compare( keys[i].container ) == 0) {
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
	std::string container;
	std::string _type;

    std::vector<var> keys;
    std::vector<var> data;
    int _pos; 	
};


void echo( var i ) {
    if( i._type == "array" ) {
        std::cout << "Array" << std::endl;
    } else {
        std::cout << i.container;
    }
}


bool empty( var a ) {
    if( a.container == "" || a.container == "0" ) {
        return true;
    }
    
    return false;
}

void print_r( var a, std::string &ret_str, std::string tab = "" ) {
    if( a.type() == "array" ) {
        int i = 0;
     
        ret_str += "Array (\n";
    
        for( auto x : a ) {
			
			ret_str += tab + "    [" + x.string() + "] => ";
		
            if( a[x].type() == "array" ) {
                print_r( a[x], ret_str, tab + "    " );
            } else {
                ret_str += a[x].string() +"\n";
            }
        }

        ret_str += tab + ")\n";
    } else {
        ret_str += a.string();
    }
}

void print_r( var a ) {
	std::string out;
	print_r( a, out );
	std::cout << out;
}

