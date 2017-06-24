#include "var.h"


enum TokenType {
    NUMBER, IDENTIFIER, STRING, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACKET,
    RIGHT_BRACKET, LEFT_BRACE, RIGHT_BRACE, PLUS, DASH, STAR, PERCENT,
    SLASH, PLUS_EQUAL, DASH_EQUAL, STAR_EQUAL, PERCENT_EQUAL,
    SLASH_EQUAL,NOT, COMMA, SEMICOLON, DOT, COLON, EQUAL, EQ_OP,
    NOT_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL, LOGICAL_AND,
    LOGICAL_OR, IF, ELSE, WHILE, FOR, RETURN, BREAK, CONTINUE, FUNCTION, GLOBAL, LOCAL, EQ_ARR, TRUE, FALSE, ARRAY, VARIABLE, AS, FOREACH, START, PLUS_PLUS, START_PHP, END_PHP, QUESTION, ELSEIF
};


var keywords = var({
    {"if", IF}, {"else", ELSE}, {"elseif", ELSEIF}, {"while", WHILE}, {"for", FOR},
    {"return", RETURN}, {"break", BREAK}, {"continue", CONTINUE},
    {"function", FUNCTION}, {"global", GLOBAL}, { "true", TRUE }, { "true", FALSE }, { "array", ARRAY }, { "as", AS }, { "foreach", FOREACH }
}).to_kv();


std::string file_get_contents( std::string file_name ) {
    std::string content;
    std::string line;
    std::ifstream myfile ( file_name );
    if ( myfile.is_open() )
    {
        while ( std::getline (myfile,line) )
        {
          content += line+"\n";
        }
        return std::move( content );
        myfile.close();
    }

    return "";
}


var tokenize( std::string &source )
{

    var tokens;
    int i = 0;
    int end = source.length();
    
    std::string passes;
    bool plz_tokenize = false;
    int it = 0;
    std::string strings;
    while( true )
    {

        if( source[i] == '<' ) {
            if( i+1 != end && source[i+1] == '?' ) {

                if( strings != "" ) {
                    tokens[it++] = {IDENTIFIER, "echo"};
                    tokens[it++] = {LEFT_PAREN};
                    tokens[it++] = {STRING, strings};
                    tokens[it++] = {RIGHT_PAREN};
                    tokens[it++] = {SEMICOLON};
                }


                tokens[it++] = {START_PHP};
                i+=2;
                strings = "";
                plz_tokenize = true;
            }
        }
        
        if( i+1 == end ) {

            if( strings != "" ) {
                tokens[it++] = {IDENTIFIER, "echo"};
                tokens[it++] = {LEFT_PAREN};
                tokens[it++] = {STRING, strings};
                tokens[it++] = {RIGHT_PAREN};
                tokens[it++] = {SEMICOLON};
            }            
            break;
        }

        if( ! plz_tokenize ) {
            strings += source[i++];

            continue;
        }

    	passes = "";
        int start = i;
        switch(source[i])
        {
            case ' ': case '\t': case '\n':
                ++i; break;
            case '(':
                tokens[it++] = {LEFT_PAREN}; ++i; break;
            case ')':
                tokens[it++] = {RIGHT_PAREN}; ++i; break;
            case '[':
                tokens[it++] = {LEFT_BRACKET}; ++i; break;
            case ']':
                tokens[it++] = {RIGHT_BRACKET}; ++i; break;
            case '{':
                tokens[it++] = {LEFT_BRACE}; ++i; break;
            case '}':
                tokens[it++] = {RIGHT_BRACE}; ++i; break;
            case ',':
                tokens[it++] = {COMMA}; ++i; break;
            case '.':
                tokens[it++] = {DOT}; ++i; break;
            case ';':
                tokens[it++] = {SEMICOLON}; ++i; break;
            case ':':
                tokens[it++] = {COLON}; ++i; break;
            case '+':
                if(i+1 != end && source[i+1] == '='){
                    tokens[it++] = {PLUS_EQUAL}; i+=2;
                } else if(i+1 != end && source[i+1] == '+'){ 
                	tokens[it++] = {PLUS_PLUS}; i+=2;
                } else {
                    tokens[it++] = {PLUS}; ++i;
                }
                break;
            case '-':
                if(i+1 != end && source[i+1] == '='){
                    tokens[it++] = {DASH_EQUAL}; i+=2;
                } else {
                    tokens[it++] = {DASH}; ++i;
                }
                break;
            case '*':
                if(i+1 != end && source[i+1] == '='){
                    tokens[it++] = {STAR_EQUAL}; i+=2;
                } else {
                    tokens[it++] = {STAR}; ++i;
                }
                break;
            case '%':
                if(i+1 != end && source[i+1] == '='){
                    tokens[it++] = {PERCENT_EQUAL}; i+=2;
                } else {
                    tokens[it++] = {PERCENT}; ++i;
                }
                break;
            case '/':
                if(i+1 != end && source[i+1] == '/') {
                    ++i; while(source[i] != '\n') ++i; ++i;
                } else if(i+1 != end && source[i+1] == '='){
                    tokens[it++] = {SLASH_EQUAL}; i+=2;
                } else {
                    tokens[it++] = {SLASH}; ++i;
                }
                break;
            case '?':
                if(i+1 != end && source[i+1] == '>'){
                    tokens[it++] = {END_PHP}; i+=2;
                    if(i+1 != end && source[i] == '\n'){
                        i++;
                    }

                    strings = "";
                    plz_tokenize = false;
                } else {
                    tokens[it++] = {QUESTION}; ++i;
                }
                break;

            case '<':
            	if(i+1 != end && source[i+1] == '='){
                    tokens[it++] = {LESS_EQUAL}; i+=2;
                } else {
                    tokens[it++] = {LESS}; ++i;
                }
                break;
            case '>':
                if(i+1 != end && source[i+1] == '='){
                    tokens[it++] = {GREATER_EQUAL}; i+=2;
                } else {
                    tokens[it++] = {GREATER}; ++i;
                }
                break;
            case '=':
                if(i+1 != end && source[i+1] == '='){
                    tokens[it++] = {EQ_OP}; i+=2;
                } else if(i+1 != end && source[i+1] == '>') {
                    tokens[it++] = {EQ_ARR}; i+=2;
                } else {
                    tokens[it++] = {EQUAL}; ++i;
                }
                break;
            case '!':
                if(i+1 != end && source[i+1] == '='){
                    tokens[it++] = {NOT_EQUAL}; i+=2;
                } else {
                    tokens[it++] = {NOT}; i+=2;
                }
                break;
            case '&':
                if(i+1 != end && source[i+1] == '&'){
                    tokens[it++] = {LOGICAL_AND}; i+=2;
                } else {
                }
                break;
            case '|':
                if(i+1 != end && source[i+1] == '|'){
                    tokens[it++] = {LOGICAL_OR}; i+=2;
                } else {
                }
                break;

            case '"':
                ++i;

                while(!(source[i] == '"' && source[i-1] != '\\')) passes += source[i++];
                ++i;
	            tokens[it++] = {STRING, var(passes).replace( var( { { "\\n", "\n" }, { "\\r", "\r"}, { "\\t", "\t" }  } ).to_kv() ) };

                break;            
            case '\'':
                ++i;
                while(!(source[i] == '\'' && source[i-1] != '\\')) passes += source[i++];
                ++i;
                tokens[it++] = {STRING, passes};
                break;
            default:
                if(std::isdigit(source[i])) {
                    while(std::isdigit(source[i])) passes += source[i++];
                    if(source[i]=='.') {
                        passes += ".";
                        ++i;
                        while(std::isdigit(source[i])) passes += source[i++];
                    }
                    /*if(source[i]=='e') {
                        ++i;
                        if(source[i]=='-') ++i;
                        while(std::isdigit(source[i])) passes += source[i++];
                    }*/

                    if( i+1 != end && source[i] == 'p') {
                        i++;
                        if( i+1 != end && source[i] == 'h' ) {
                            i++;
                            if( i+1 != end && source[i] == 'p' ) {
                                i++;
                                tokens[it++] = { IDENTIFIER, "4php" };
                            }                            
                        }
                    } else {
                        tokens[it++] = {NUMBER, to_number<double>(passes)};   
                    }

                    
                    break;
                } else if(std::isalpha(source[i]) || source[i] == '_' || source[i] == '$' ) {
                    if( source[i] == '$' )
                    	++i;

                    while((std::isalnum(source[i]) || source[i] == '_')) passes += source[i++];
                    auto kw = keywords[ passes ];
                    if( source[start] == '$' ) {
                        tokens[it++] = {VARIABLE, passes};
                    } else if(kw == "" ) {
                        tokens[it++] = {IDENTIFIER, passes };
                    }
                    else {
                        if( kw == ELSEIF ) {
                            tokens[it++] = {ELSE, passes};
                            tokens[it++] = {IF, passes};
                        } else {             
                            tokens[it++] = var( { kw } );
                        }
                    }
                    break;
                } else {
                }
        }

       
        if( i == start ) {
            break;
        }
        
        

    }
    return tokens;
}

//var variables;
typedef var (*func)(var&);
typedef std::map<std::string, func> funcs;
funcs functions;
static var constants;
static var tokens;

class interpreter {
public:

    interpreter( var &tokens ) : tokens( tokens ) {

    }

    void start() {

        do_operator();

        //continue intepreting
        if( ( tokens[ offset ][0] == SEMICOLON  || tokens[ offset ][0] == END_PHP ) && offset != tokens.count() - 1  ) {

            offset++;

            start();
        }

    }

    ~interpreter() {
      //print_r( variables );
    }
private:

    bool get_val( var &val ) {


        if( tokens[offset][0] == IDENTIFIER ) {
            
            if( tokens[offset][1] == "include") {
                offset++;
                val = do_include();
                return true;
            } else if( functions[ tokens[offset][1].string() ] ) {
                val = do_function();
                return true;
            } else if( local_functions.isset( tokens[offset][1] ) ) {
                do_function( true );
                val = return_val;
                return true;
            } else {
                //find function and execute it
                int temp_offset = offset;
                find_function();
                offset = temp_offset;

                if( local_functions.isset( tokens[offset][1] ) && tokens[offset+1][0] == LEFT_PAREN ) {
                    do_function( true );
                    val = return_val;                     
                } else {

                    val = constants[ tokens[offset++][1] ];  
                }                
   
                return true;
            }
        } else if( tokens[offset][0] == START_PHP ) {
            offset++;
            if( tokens[offset][0] == IDENTIFIER && tokens[offset][1].in_array({ "php", "4php" } ) ) {
                offset++;
            }

            if( tokens[offset][0] == EQUAL ) {
                offset++;
                val = do_operator();
                echo( val );
            }

            start();  
            
        }  else if( tokens[offset][0] == END_PHP ) {
            offset++;
            start();            
        } else if( tokens[offset][0].in_array( { STRING, NUMBER, TRUE, FALSE } ) ) {

            val = tokens[offset++][1];
            return true;
        } else if( tokens[ offset ][0] == LEFT_BRACKET ) {
            val = do_array();
            return true;
        } else if( tokens[offset][0] == VARIABLE ) {

            var var_name = tokens[ offset++ ][1];


            if( tokens[ offset ][0] == EQUAL ) {
                offset++;

                variables[ var_name ] = do_operator();

            }

            if( tokens[offset][0] == PLUS_PLUS ) {
                offset++;
                variables[ var_name ] += 1;
            }

            if( tokens[offset][0] == LEFT_BRACKET ) {

                val = do_variable( variables[ var_name ] );

                return true;
            }


            val = variables[ var_name ];
            return true;
        } else if( tokens[offset][0] == LEFT_PAREN ) {
            offset++;
            val = do_paren();
            return true;
        } else if( tokens[offset][0] == FOREACH ) {
            offset++;
            do_foreach();
            return true;
        } else if( tokens[offset][0] == FUNCTION ) {

            offset++;
            make_function();
            return true;
        } else if( tokens[ offset ][0] == LEFT_BRACE ) {          
            val = do_array();
            return true;

        } else if( tokens[offset][0] == IF ) {

            offset++;
            do_statement();
            return true;
        } else if( tokens[offset][0] == ELSE ) {
            offset++;

            if( tokens[offset++][0] == IF ) {
                offset++;
                find_end_paren();
            } 
            offset++;
            find_end_block();
            start();
            return true;
        } else if( tokens[offset][0] == RETURN ) {
            int temp_offset = offset++;
            val = do_operator();

            return_val = val;
            offset = temp_offset;

            return true;
        } else if( tokens[offset][0] == WHILE ) {
            offset++;
            do_while();
            return true;

        } else if( tokens[offset][0] == ARRAY ) {
            offset++;
            val = do_array();
            return true;
        } else if( tokens[offset][0] == FOR ) {
            offset++;

            offset++;   
            do_operator();

            offset++;

            do_for();
            return true;
        } else if( tokens[offset][0] == GLOBAL ) {
            offset++;

            variables[ tokens[offset][1] ] = temp_variables[ tokens[offset][1] ];
            offset++;
            return true;
        }

        return false;
    }

    var do_include(){
        offset++;

        std::string source = file_get_contents( tokens[offset][1].string() ) ;  
        offset++;
        offset++;

        if( source != "" ) {

            var new_toks = tokenize( source );
                
            var cache_tokens;
            int i = offset;
            int j = 0;

            cache_tokens[j] = tokens[ i ];
            tokens[i++] = {END_PHP};

            for( auto x : new_toks ) {
                if (tokens[ i ].count() > 0 )
                    cache_tokens[j] = tokens[ i ];
                
                
                tokens[i++] = new_toks[j++];
            }

            cache_tokens[j] = tokens[ i ];
            tokens[i++] = {START_PHP};

            for( auto x1 : cache_tokens ) {
                tokens[i++] = cache_tokens[x1];
            }

        } 
        return 0;      
    }


    var do_operator() {
        var ret = do_second_operator();
        
        if( tokens[ offset ][0] == DOT ) {
            offset++;
            ret = ret.concat( do_operator() );
        } else if( tokens[offset][0] == EQ_OP ) {
            offset++;
            ret = ret == do_operator();
        } else if( tokens[offset][0] == LESS_EQUAL ) {
            offset++;
            ret = ret <= do_operator();
        } else if( tokens[offset][0] == LESS ) {
            offset++;
            ret = ret < do_operator();
        } else if( tokens[offset][0] == GREATER ) {
            offset++;
            ret = ret > do_operator();
        } else if( tokens[offset][0] == GREATER_EQUAL ) {
            offset++;
            ret = ret >= do_operator();
        }

        return ret;
    }

    var do_first_operator() {
        var ret;
        
        if( ! get_val( ret ) ) {
            return ret;
        }

        if( tokens[offset][0] == STAR ) {
            offset++;
            ret *= do_first_operator();
        } else if( tokens[offset][0] == SLASH ) {
            offset++;
            ret /= do_first_operator();
        } else if( tokens[offset][0] == LOGICAL_AND ) {
            offset++;
            ret = ret && do_first_operator();
        }

        return ret;
    }

    var do_second_operator() {

        var ret = do_first_operator();


        if( tokens[ offset ][0] == PLUS ) {
            offset++;
            ret += do_second_operator();
        } else if( tokens[ offset ][0] == DASH  ) {
            offset++;
            ret -= do_second_operator();
        } else if( tokens[ offset ][0] == LOGICAL_OR ) {
            offset++;
            ret = ret || do_second_operator();
        }

        return ret;
    }


    var do_array() {
       
        offset++;
       
        int index = 0;
        var array;
        while( true ) {
            if( tokens[offset][0].in_array( { RIGHT_BRACKET, RIGHT_PAREN, RIGHT_BRACE } )  ) {
                offset++;
                break;
            }

            var gindex = do_operator();

            if( tokens[offset][0].in_array( { EQ_ARR, COLON } ) ) {
                offset++;
                array[gindex] = do_operator();
            } else {
                array[index++] = gindex;
            }

            if( tokens[offset][0] == COMMA ) {
                offset++;
                continue;
            }
        }

        return array;
    }

    var do_function( bool local = false ) {
        std::string func_name = tokens[ offset++ ][1].string();
        offset++;
        
        int index = 0;
        var out;
        while( true ) {

            if( tokens[ offset ][0].in_array( {  RIGHT_PAREN } ) ) {
                offset++;
                break;
            }  

            out[ index++ ] = do_operator();

            if( tokens[ offset ][0] == COMMA ) {
                offset++;
                continue;
            }

        }
        

        if( local ) {
 
            temp_variables = variables;
            variables.unset();

            var ff = local_functions[ func_name ];

            int temp_offset = offset;
            offset = ff[0].to_int()+1;
            var &params = ff[2];
            for( auto x : params ) {
                variables[ params[x] ] = out.isset( x ) ? out[x] : "";
            }


            start();

            variables = temp_variables;

            offset = temp_offset;

            return 0;   
        }

        return functions[ func_name ]( out );
    }

    void find_function() {
        //find function if call before definetion
        std::string func_name = tokens[ offset++ ][1].string();

        while( true ) {
            if( offset == tokens.count() - 1 ) {
                break;
            }

            if( tokens[offset++][0] == FUNCTION ) {

                if( tokens[offset][1] == func_name ) {
                    make_function( false );
                    break;
                }
            }
        }

    }

    var do_variable( var &vars ) {

        offset++;

        var index = do_operator();
        var empty;
        var &out = vars.isset(index) ? vars[ index ] : empty ;
        offset++;

        if( tokens[offset][0] == EQUAL ) {
            offset++;

            if( index == "" ) {
                index = vars.count();
            }


            vars[index] = do_operator();
        }


        if( tokens[offset][0] == LEFT_BRACKET ) {
            return do_variable( out );
        }
 
        return out;
    }



    var do_paren() {
        var out = do_operator();
        offset++;

        return out;
    }


    void do_while() {
        int start_offset = offset;

        offset++;

        var statement = do_operator();

        offset++;

        offset++;
        int st_while_offset = offset;
        if( statement ) {

            start();

            if( tokens[offset-2][0] == BREAK ) {
  
                offset = st_while_offset;
                find_end_block();
                start();
            } else  {

                offset = start_offset;
                do_while();            
            }

        } else {
            
            find_end_block();
            start();
        }
    }


    void do_for() {
        int start_offset = offset;

        var statement = do_operator();
        offset++;

        int start_offset_end = offset;
        while( true ) {
            if( tokens[offset++][0] == RIGHT_PAREN )
                break;
        }

        offset++;
        int st_while_offset = offset;
        if( statement ){
            start();
            if( tokens[offset-2][0] == BREAK ) {
                offset = st_while_offset;
                find_end_block();
                start();
            } else {
                offset = start_offset_end;

                do_operator();

                offset = start_offset;

                do_for();               
            }
            
        } else {

            find_end_block();
   
            start();
        }

    }


    void do_statement() {
        offset++;

        var statement = do_operator();

        offset++;

        if( ( statement > 0 && statement != "" && statement != 0 ) || ( statement.count() > 0 ) ) {
            offset++;
            start();
            offset++;
        } else {
            offset++;
            find_end_block();

            if( tokens[offset][0] == ELSE ) {
                offset++;

                if( tokens[offset][0] == IF ) {
                    offset++;
                    do_statement();  
                } else {
                    offset++;
                    start();
                    offset++;                           
                }
            }
        }

        start();
    }


    int find_end_block() {

        while( true ) {

            if( tokens[offset][0] == RIGHT_BRACE ) {
                return ++offset;
            }

            if( tokens[offset++][0] == LEFT_BRACE ) {
                find_end_block();
            }

        }
    }


    int find_end_paren() {

        while( true ) {

            if( tokens[offset][0] == RIGHT_PAREN ) {
                return ++offset;
            }

            if( tokens[offset++][0] == LEFT_PAREN ) {
                find_end_paren();
            }

        }
    }

    void make_function( bool plz_start = true ) {
        var func_name = tokens[offset++][1];
        if( local_functions.isset( func_name ) ) {
            offset = local_functions[ func_name ][1].to_int();
        } else {
            offset++;
            var params;
            int i = 0;
            while( true ) {

                if( tokens[offset][0] == RIGHT_PAREN ) {
                    offset++;
                    break;
                }

                params[ i++ ] = tokens[offset++][1];

                if( tokens[offset][0] == COMMA ) {
                    offset++;
                    continue;
                }
            }


            var st = offset++;
            var end = find_end_block();

            var ff;
            ff[0] = st;
            ff[1] = end;
            ff[2] = params;
            local_functions[ func_name ] = ff;           
        }

        if( plz_start )
            start();
    }

    void do_foreach() {
        if( tokens[offset][0] == LEFT_PAREN ) {
            offset++;
        }

        var array = do_operator();
        var key;

        var value = tokens[++offset][1];
        offset++;
        if( tokens[offset][0] == EQ_ARR ) {
            offset++;
            key = value;
            value = tokens[offset++][1];
        }
        offset++;

        if( array.type() != "array" ) {
            offset++;
            find_end_block();

        } else {
            int temp_offset = ++offset;

            for( auto x : array ) {
                variables[ key ] = x;
                variables[ value ] = array[x];
                offset = temp_offset;

                start();

                if( tokens[offset-2][0] == BREAK ) {
                    
                    offset = temp_offset;
                    find_end_block();
                    offset--;
                    break;
                }
                
            }
            offset++;           
        }
        

        start();
    }

    void error( var error ) {
        echo( error );
    }

    int offset = 0;
    var tokens;
    var variables;
    var temp_variables;
    var return_val;
    var local_functions;
    //var whiles;
};



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

    return std::time(0);;
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

var exten_define( var &p )  {
    if( ! constants.isset( p[0] ) ) {
        constants[ p[0] ] = p[1];
    }

    return 0;
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



int main( int argc, char** argv ) {

    if( argv[1] == NULL ) {
        std::cout << "Please select a file" << std::endl;
        exit(0);
    }

	std::string source = file_get_contents( argv[1] );

    if( source == "" ) {
        std::cout << "File open problem" << std::endl;
        exit(0);
    }

    functions["sum"] = exten_sum;
    functions["echo"] = exten_echo;
	functions["print"] = exten_echo;
	functions["print_r"] = exten_print_r;
	functions["is_array"] = exten_is_array;
	functions["type"] = exten_type;
    functions["count"] = exten_count;
    functions["microtime"] = exten_microtime;
    functions["time"] = exten_time;
    functions["usleep"] = exten_usleep;
    functions["exit"] = exten_exit;
    functions["die"] = exten_die;

    functions["implode"] = exten_implode;
    functions["explode"] = exten_explode;
    functions["define"] = exten_define;


	tokens = tokenize( source );


    interpreter i( tokens );
    i.start();

}
