class request {
public:
	var server( var a = "" ) {
		if( a != "" ) {
			var o1 = getenv( a.string().c_str() );
			return o1;
		}
		
		const std::string ENV[ 24 ] = {                
			"COMSPEC", "DOCUMENT_ROOT", "GATEWAY_INTERFACE",   
			"HTTP_ACCEPT", "HTTP_ACCEPT_ENCODING",             
			"HTTP_ACCEPT_LANGUAGE", "HTTP_CONNECTION",         
			"HTTP_HOST", "HTTP_USER_AGENT", "PATH",            
			"QUERY_STRING", "REMOTE_ADDR", "REMOTE_PORT",      
			"REQUEST_METHOD", "REQUEST_URI", "SCRIPT_FILENAME",
			"SCRIPT_NAME", "SERVER_ADDR", "SERVER_ADMIN",      
			"SERVER_NAME","SERVER_PORT","SERVER_PROTOCOL",     
			"SERVER_SIGNATURE","SERVER_SOFTWARE" };
		
		var array;
		for( int i = 0; i < 24; ++i ) {
			if( char *out = getenv( ENV[i].c_str() ) ) {
				array[ENV[i]] = out;
			}
		}
		
		return array;
	}

	std::string url_decode( std::string &eString ) {
		std::string ret;
		char ch;
		int i, j;
		for ( i = 0; i < eString.length(); i++ ) {
			if ( int( eString[i] ) == 37 ) {
				sscanf( eString.substr(i+1,2).c_str(), "%x", &j );
				ch = static_cast<char>(j);
				ret += ch;
				i = i+2;
			} else {
				ret += eString[i];
			}
		}
		return (ret);
	}

	var post() {
		if( this->server( "REQUEST_METHOD" ) == "POST" ) {
			var cole = this->server( "CONTENT_LENGTH" );
			
			int cl = atol( cole.string().c_str() );

			char *temp = new char[cl];
			std::cin.read(temp, cl);

			var post_data = std::string(temp, cl);
			delete [] temp;
			
			var arr1 = post_data.explode("&");

			var out;
			for( auto x : arr1 ) {
				var a = arr1[x].explode( "=" );
				std::string val = a[1].string();
				out[a[0]] = (var)url_decode( val );
			}
			
			return out;
		}
		return (var)("");
	}
	
	var get( var i = "" ) {
		var qs = getenv( "QUERY_STRING" );
		
		var arr1 = qs.explode("&");
		var out = {};
		for( auto x : arr1 ) {
			var a = arr1[x].explode( "=" );
			std::string val = a[1].string();
			if( a[0] != "")
				out[a[0]] = url_decode( val );
		}

		if( i == "" ) {
			return out;
		}
		
		return ( out[ i ] );
	}	
};
