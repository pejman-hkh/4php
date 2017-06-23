<?4php

$ret = pejman( 'test', 'test1' );

echo( 'ret is '. $ret."\n" );

function pejman( $a, $b ) {
	echo('a is : '. $a."\n" );
	echo('b is : '. $b."\n" );

	return "return function";
	//after return test
	echo("test\n");
	echo("test\n");
	echo("test\n");
	echo("test\n");
	echo("test\n");
}


echo("after function\n")
?>