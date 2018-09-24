#include "var.h"
#include <dlfcn.h>

enum TokenType {
    NUMBER, IDENTIFIER, STRING, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACKET,
    RIGHT_BRACKET, LEFT_BRACE, RIGHT_BRACE, PLUS, DASH, STAR, PERCENT,
    SLASH ,NOT, COMMA, SEMICOLON, DOT, COLON, EQUAL, EQ_OP,
    NOT_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL, LOGICAL_AND,
    LOGICAL_OR, IF, ELSE, WHILE, FOR, RETURN, BREAK, CONTINUE, FUNCTION, GLOBAL, LOCAL, EQ_ARR, TRUE, FALSE, ARRAY, VARIABLE, AS, FOREACH, START, PLUS_PLUS, START_PHP, END_PHP, QUESTION, ELSEIF, SHEBANG, DASH_DASH, NEW, OBJECT_OPERATOR
};


var keywords = var({
    {"if", IF}, {"else", ELSE}, {"elseif", ELSEIF}, {"while", WHILE}, {"for", FOR},
    {"return", RETURN}, {"break", BREAK}, {"continue", CONTINUE},
    {"function", FUNCTION}, {"global", GLOBAL}, { "new", NEW }, { "true", TRUE }, { "false", FALSE }, { "array", ARRAY }, { "as", AS }, { "foreach", FOREACH }
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

var loaded_extension;

bool load_extension( const std::string &extension ) {

    if( loaded_extension.isset( extension ) ) return false;

    loaded_extension[extension];

    typedef var(*func)(var&);
    typedef var(*func1)(var);

    char *error;
    void *myso = dlopen( ("extension/"+extension+"/"+extension+".so").c_str(), RTLD_NOW);

    func1 load_functions = (func1)dlsym(myso, "load_functions" );
    if ((error = dlerror()) == NULL)  {
        var funcs = load_functions(0);
        for( auto &d : funcs ) {
            functions[d] = funcs[d];
        } 
    } else {
        //puts( error );
    }

    func load = (func)dlsym(myso, "load" );
    if ((error = dlerror()) == NULL)  {
        classes[extension] = load;
    } else {
        //puts(error);
    }

    //dlclose(myso);    
}

static bool shebang_seted = false;

var tokenize( std::string &source, bool eval = false )
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
                    if( strings.substr(0, 2) == "#!" && ! shebang_seted ) {
                        tokens[it++] = {SHEBANG, strings};
                        shebang_seted = true;
                    } else if( strings == "?>" && eval ) {
                        tokens[it++] = {END_PHP};
                    } else {
                        tokens[it++] = {IDENTIFIER, "echo"};
                        tokens[it++] = {LEFT_PAREN};
                        tokens[it++] = {STRING, strings};
                        tokens[it++] = {RIGHT_PAREN};
                        tokens[it++] = {SEMICOLON};                        
                    }

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
                if(i+1 != end && source[i+1] == '+'){ 
                    tokens[it++] = {PLUS_PLUS}; i+=2;
                } else {
                    tokens[it++] = {PLUS}; ++i;
                }
                break;
            case '-':
                if(i+1 != end && source[i+1] == '>'){
                    tokens[it++] = {OBJECT_OPERATOR}; i+=2;
                } else if(i+1 != end && source[i+1] == '-'){
                    tokens[it++] = {DASH_DASH}; i+=2;
                } else {
                    tokens[it++] = {DASH}; ++i;
                }
                break;
            case '*':
                tokens[it++] = {STAR}; ++i;
                
                break;
            case '%':
                
                tokens[it++] = {PERCENT}; ++i;
                
                break;
            case '/':
                if(i+1 != end && source[i+1] == '/') {
                    ++i; while(source[i] != '\n') ++i; ++i;
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

                while(!(source[i] == '"' && source[i-1] != '\\')) {
    
                    if(source[i] == '\\') {

                       switch(source[++i]) {
                            case 'a': passes += '\a'; break;
                            case 'b': passes += '\b'; break;
                            case 'f': passes += '\f'; break;
                            case 'n': passes += '\n'; break;
                            case 'r': passes += '\r'; break;
                            case 't': passes += '\t'; break;
                            case 'v': passes += '\v'; break;
                            case '\'': passes += '\''; break;
                            case '\"': passes += '\"'; break;
                            default:
                                passes += '\\';
                                passes += source[i];
                        }
                        i++;
                    } else {
                        passes += source[i++];  
                    } 
                }  
                    
                
                ++i;
                tokens[it++] = {STRING, passes };

                break;            
            case '\'':
                ++i;
                while(!(source[i] == '\'' && source[i-1] != '\\')) passes += source[i++];
                ++i;
                tokens[it++] = {STRING, passes};
                break;
            default:
                if(std::isdigit(source[i])) {
                    bool is_int = true;
                    while(std::isdigit(source[i])) passes += source[i++];
                    if(source[i]=='.') {
                        is_int = false;
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

                        var val;
                        if( is_int ) {
                            val = to_number<int>(passes);
                        } else {
                            val = to_number<double>(passes);
                        }

                        
                        tokens[it++] = {NUMBER, val};   
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

static var tokens;


class interpreter {
public:

    interpreter( var &tokens ) : tokens( tokens ) {

    }


    var &get_variables() {
        return variables;
    }

    void start() {

        do_operator();

        if( offset != tokens.count() - 1 ) {
            switch( tokens[ offset ][0].to_int() ) {
                case SEMICOLON:
                case END_PHP:
                    offset++;
                    start();
                break;                    
            }
        }
 

    }

    ~interpreter() {
      //print_r( variables );
    }
private:

    bool get_val( var &val ) {

        switch( tokens[offset][0].to_int() ) {
            case START_PHP: {
                offset++;
                if( tokens[offset][0] == IDENTIFIER && (tokens[offset][1] == "php" || tokens[offset][1] == "4php" ) ) {
                    offset++;
                } else if( tokens[offset][0] == EQUAL ) {
                    offset++;
                    val = do_operator();
                    echo( val );
                }

                start();  

                return true;
            } break;


            case END_PHP: {
                offset++;
                start();
                return true;              
            } break;


            case VARIABLE: {
                var &var_name = tokens[offset++][1];

                val = do_variable( variables[ var_name ], var_name );

                for( auto &x : super_global_variables ) {
                    variables[x] = super_global_variables[x];
                }

                return true;
            } break;

            
            case IDENTIFIER: {
                if( functions.isset( tokens[offset][1] ) ) {
                    val = do_function();
                    return true;
                } else if( local_functions.isset( tokens[offset][1] ) ) {

                    do_function( true );
                    val = return_val;
                    return true;
                } else if( tokens[offset][1] == "eval" || tokens[offset][1] == "include" ) {
                    val = do_eval();
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
            } break;
            
 
            case STRING: case NUMBER: case TRUE: case FALSE : {
                switch( tokens[offset][0].to_int() ) {
                    case TRUE :
                        val = true;
                        offset++;
                        break;
                    case FALSE :
                        val = false;
                        offset++;                   
                        break;
                    default:
                        val = tokens[offset++][1]; 
                        break;

                }
           
                return true;            
            } break;


            case LEFT_BRACKET: case LEFT_BRACE: case ARRAY: {
                val = do_array();
                return true;
            } break;
        

            case LEFT_PAREN: {
                offset++;
                val = do_paren();
                return true;            
            } break;


            case FOREACH: {
                offset++;
                do_foreach();
                return true;            
            } break;

            case FUNCTION: {
                offset++;
                make_function();
                return true;            
            } break;


            case IF:{
                offset++;
                do_statement();
                return true;            
            } break;

            case ELSE:{
                offset++;

                if( tokens[offset++][0] == IF ) {
                    offset++;
                    find_end_paren();
                } 
                offset++;
                find_end_block();
                start();
                return true;            
            } break;

            case RETURN:{
                int temp_offset = offset++;
                val = do_operator();

                return_val = val;
                offset = temp_offset;

                return true;            
            } break;
            
            case WHILE:{
                offset++;
                int end_offset = 0;
                offset++;
                do_while( end_offset );
                return true;            
            } break;
            
            case FOR:{
                offset++;

                offset++;   
                do_operator();

                offset++;

                do_for();
                return true;            
            } break;
            
            case GLOBAL:{
                offset++;

                variables[ tokens[offset][1] ] = temp_variables[ tokens[offset][1] ];
                offset++;
                return true;            
            } break;

            case PLUS_PLUS:{
                offset++;
                return true;            
            } break;

            case NEW:{
                offset++;
                val = make_class();

               return true;            
            } break;

            case SHEBANG: {
                offset++;
                start();
                return true;            
            } break;

        }

       return false;
    }

    var do_first_operator() {
        var ret;
        if( ! get_val( ret ) )
            return ret;

        switch( tokens[offset][0].to_int() ) {
            case EQ_OP :
                offset++;
                ret = ret == do_first_operator();
            break;

            case LESS_EQUAL : 
                offset++;
                ret = ret <= do_first_operator();
            break;

            case LESS :
                offset++;
                ret = ret < do_first_operator();
            break;    
            case GREATER :     
                offset++;
                ret = ret > do_first_operator();
            break;

            case GREATER_EQUAL :     
                offset++;
                ret = ret >= do_first_operator();
            break;

        }

        return ret;
    }

    var do_second_operator() {

        var ret = do_first_operator();

        switch( tokens[offset][0].to_int() ) {
            case PERCENT :
                offset++;
                ret %= do_second_operator();
            break;

            case STAR : 
                offset++;
                ret *= do_second_operator();
            break;

            case SLASH :
                offset++;
                ret /= do_second_operator();
            break;    
            case LOGICAL_AND :     
                offset++;
                ret = ret && do_second_operator();
            break;

        }

        return ret;
    }

    var do_operator() {

        var ret = do_second_operator();
        
        switch( tokens[offset][0].to_int() ) {
            case DOT :
                offset++;
                ret = ret.concat( do_operator() );
            break;

            case PLUS : 
                offset++;
                ret += do_operator();
            break;

            case DASH :
                offset++;
                ret -= do_operator();
            break;    
            case LOGICAL_OR :     
                offset++;
                ret = ret || do_operator();
            break;

        }

        return ret;
    }


    var make_class() {
   
        var &class_name = tokens[offset][1];
        offset++;
        offset++;

        var params;
        int i = 0;
        while( true ) {

            if( tokens[offset][0] == RIGHT_PAREN ) {
                break;
            }

            params[i++] = do_operator();

            if( tokens[offset][0] == COMMA ) {
                offset++;
                continue;
            }

        }
        offset++;

        load_extension( class_name.string() );

        //define new object
        if( classes.isset( class_name ) ) {
           return classes[ class_name ].get()( params ); 
        }


        return 0;
    }

    var do_variable( var &vars, var &var_name ) {
        if( tokens[offset][0] == RIGHT_PAREN || tokens[offset][0] == COMMA ) {
            return vars;
        } else if( tokens[offset][0] == EQUAL ) {

            offset++;

            vars = do_operator();

            return vars;

        } else {
            var var_val;
            var index;
            bool index_t = false;
            if( tokens[offset][0] == LEFT_BRACKET ) {

                offset++;

                index = do_operator();

                offset++;
                index_t = true;
            }

            if( tokens[offset][0] == OBJECT_OPERATOR ) {
                offset++;

                var &method_name = tokens[offset++][1];
                offset++;

                var params;
                int i = 0;
                while( true ) {

                    if( tokens[offset][0] == RIGHT_PAREN ) {
                        break;
                    }

                    params[i++] = do_operator();

                    if( tokens[offset][0] == COMMA ) {
                        offset++;
                        continue;
                    }

                }

                offset++;
                //call method
                if( vars.isset( method_name ) ) {
                    return vars[ method_name ].get_m()( params );
                }

            }

            if( tokens[offset][0] == PLUS_PLUS ) {
                offset++;
                if( index_t )
                    vars[index] += 1;
                else
                    vars += 1;
            }


            if( index == "" ) {
                index = vars.count();
            }

            if( tokens[offset][0] == EQUAL ) {
                offset++;

                var_val = do_operator();

                if( index_t )
                    vars[index] = var_val;
                else
                    vars = var_val;
            } else if( tokens[ offset ][0] == STAR || tokens[ offset ][0] == PLUS || tokens[ offset ][0] == DOT || tokens[ offset ][0] == SLASH || tokens[ offset ][0] == DASH  ) {

                if( tokens[ offset + 1 ][0] == EQUAL ) {
                    int operator_k = tokens[ offset ][0].to_int();
                    offset++;

                    offset++;
                    var_val = do_operator();
                    
                    switch( operator_k ) {
                        case STAR :
                            if( index_t )
                                vars[index] *= var_val;
                            else
                                vars *= var_val; 
                            break;
                        case DOT :
                            if( index_t )
                                vars[index] = vars[index].concat( var_val );
                            else
                                vars = vars.concat( var_val ); 
                            break;
                        case PLUS :
                            if( index_t )
                                vars[index] += var_val;
                            else
                                vars += var_val; 
                            break;
                        case SLASH :
                            if( index_t )
                                vars[index] /= var_val;
                            else
                                vars /= var_val; 
                            break; 
                        case PERCENT :
                            if( index_t )
                                vars[index] %= var_val;
                            else
                                vars %= var_val; 
                            break; 
                        case DASH :
                            if( index_t )
                                vars[index] -= var_val;
                            else
                                vars -= var_val; 
                            break;  
                    }

                }
            }

            //var empty;
            var &out = index_t ? vars[ index ] : vars ;

            if( var_name == "SUPERGLOBALS" && index_t ) {
                super_global_variables[ index ] = var_val;
            }
            
            if( tokens[offset][0] == LEFT_BRACKET ) {

                return do_variable( out, var_name );
            } 
        
            return out; 

        }

    }


    var do_eval() {

        int start_offset = offset;
        var &kind = tokens[offset][1];

        offset++;
        offset++;

        std::string source;
        if( kind == "eval" ) {
            source = do_operator().string() ;  
        } else {
            source = file_get_contents( tokens[offset][1].string() ) ;
            offset++;
        }
        
        offset++;
        offset++;

        
        if( source != "" ) {

            var new_toks = tokenize( source, true );
            
            //for include remove start & end php tag
            if( kind != "eval" ) {
                 if( new_toks[0][0] == START_PHP ) {
                    new_toks.unset(0);
                    if( new_toks[1][0] == IDENTIFIER && ( new_toks[1][1] == "php" || new_toks[1][1] == "4php" ) ) {
                        new_toks.unset(1);
                    }

                }
                
                var end_index = *(new_toks.end());
                if( new_toks[ end_index ][0] == END_PHP ) {
                    new_toks.unset( end_index );
                }
            }

            var new_tokens;
            int i = 0;
            int index = 0;
            for( auto &x : tokens ) {
                if( ! tokens.isset( i ) ) break;

                new_tokens[ index++ ] = tokens[i++];
           

                if( i == start_offset ) {
                    for( auto &x1 : new_toks ) {
                        new_tokens[ index++ ] = new_toks[x1];
                    }

                    while( i < offset ) {
                        i++;
                    }

                }
            }

            tokens = new_tokens;
            new_tokens.unset();
        }




        offset = start_offset;

        start();
        return 0;  
    }


    var do_array() {
       
        offset++;
       
        int index = 0;
        var array;
        while( true ) {
            switch( tokens[offset][0].to_int() ){
                case RIGHT_BRACKET:
                case RIGHT_PAREN :
                case RIGHT_BRACE :
                    offset++;
                    goto exit_loop;
                break;    
            }
          
            var gindex = do_operator();
            
            if( tokens[offset][0] == EQ_ARR || tokens[offset][0] == COLON ) {
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
        exit_loop:
        return array;
    }


    var do_function( bool local = false ) {
        var &func_name = tokens[ offset++ ][1];
        offset++;
        
        int index = 0;
        var out;
        while( true ) {

            if( tokens[ offset ][0] == RIGHT_PAREN ) {
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


            var &ff = local_functions[ func_name ];

            int temp_offset = offset;
            offset = ff[0].to_num()+1;
            var &params = ff[2];
            var &params_val = ff[3];

            for( auto &x : params ) {
                variables[ params[x] ] = out.isset( x ) ? out[x] : ( params_val.isset( x ) ? params_val[x] : "" );
            }

            //define super global variable in functions
            for( auto &s : super_global_variables ) {
                variables[ s ] = super_global_variables[s];
            }

            start();

            variables = temp_variables;

            offset = temp_offset;

            return 0;   
        }

        return functions[ func_name ].get()( out );
    }

    void find_function() {
        //find function if call before definetion
        var &func_name = tokens[ offset++ ][1];

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

    var do_paren() {
        var out = do_operator();
        offset++;

        return out;
    }


    void do_while( int &end_offset ) {
        int start_offset = offset;

        var statement = do_operator();

        offset += 2;

        int st_while_offset = offset;
        if( statement ) {

            start();

            if( tokens[offset-2][0] == BREAK ) {
           
                offset = st_while_offset;
                find_end_block();                   
           
                start();
            } else  {
                end_offset = offset;
                offset = start_offset;
                do_while( end_offset );         
            }

        } else {

            offset = end_offset+1;
            start();
        }
    }



    void do_for() {
        int start_offset = offset;

        var statement = do_operator();
        offset++;

        int start_offset_end = offset;
 
        find_end_paren();

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

        if( statement ) {
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
        var &func_name = tokens[offset++][1];
        if( local_functions.isset( func_name ) ) {
            offset = local_functions[ func_name ][1].to_num();
        } else {
            offset++;
            var params;
            var params_val;
            int i = 0;
            while( true ) {

                if( tokens[offset][0] == RIGHT_PAREN ) {
                    offset++;
                    break;
                }

                params[ i++ ] = tokens[offset++][1];
                
                if( tokens[offset][0] == EQUAL ) {
                    offset++;
                    params_val[ i - 1 ] = do_operator();

                }

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
            ff[3] = params_val;
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

        var &value = tokens[++offset][1];
        offset++;
        if( tokens[offset][0] == EQ_ARR ) {
            offset++;
            key = value;
            value = tokens[offset++][1];
        }
        offset++;

        if( array.count() == 0 ) {

            offset++;
            find_end_block();

        } else {
            int temp_offset = ++offset;

            for( auto &x : array ) {
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


    int offset = 0;
    var &tokens;
    var variables;
    var temp_variables;
    var return_val;
    var local_functions;
    var super_global_variables;
};

var exten_define( var &p )  {
    if( ! constants.isset( p[0] ) ) {
        constants[ p[0] ] = p[1];
    }

    return 0;
}

var exten_sum( var &p ) {
    var out = 0;
    for( auto &x : p ) {
        out += p[x];
    }
    return out;
}


var exten_echo( var &p ) {
    echo( p[0] );

}

var exten_print_r( var &p ) {

    if( p[1] == "true" ) {
        std::string out;
        print_r( p[0], out );
        return out;
    }
    else 
       print_r( p[0] );
    
 
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

}

var exten_die( var &p )  {
    echo( p[0] );
    exit(0);

}

var exten_implode( var &p )  {
    var out = "";
    var pre = "";
    for( auto &x : p[1] ) {
        out += pre+p[1][x];
        pre = p[0];
    }

    return out;
}


var exten_get_defined_functions( var &p )  {
    var internal;
    int i = 0;
    for( auto &x : functions ) {
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

    functions["define"] = exten_define;
    functions["sum"] = exten_sum;
    functions["echo"] = exten_echo;
    functions["print"] = exten_echo;
    functions["print_r"] = exten_print_r;
    functions["is_array"] = exten_is_array;
    functions["type"] = exten_type;
    functions["count"] = exten_count;
    functions["microtime"] = exten_microtime;
    functions["time"] = exten_time;
    functions["round"] = exten_round;
    functions["usleep"] = exten_usleep;
    functions["exit"] = exten_exit;
    functions["die"] = exten_die;
    functions["implode"] = exten_implode;
    functions["explode"] = exten_explode;
    functions["get_defined_functions"] = exten_get_defined_functions;


    tokens = tokenize( source );

    interpreter i( tokens );
    i.start();

}
