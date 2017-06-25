<?php
define('pejman', 123);

echo("Constant is " .pejman."\n" );

$ppp = 20;

$ret = pejman( 'test', 'test1' );


function pejman( $a, $b ) {
	global $ppp;

	global $sss;

	echo( 'global ppp is :'. $ppp."\n" );

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