<?4php

$a = 1;
$b = 0;

echo( $a && $b );

echo( $a || $b );

if( $a == 1 && $b == 0 ) {
	echo("true");
} else {
	echo("false");	
}

if( $a || $b ) {
	echo("true");
}

$p = 3 + 2 * 8;

echo( $p );


$text = 'test';

$text .= 'ttttttt';

echo( $text );


$a = 10;
$a += 2;
echo( $a );


$g = [ 1, 2, [ 3, 4, [ 5 , 6 ] ] ];

print_r( $g );

$g[2][2][0] = '4php';
$g[2][2][0] += 'peji';
$g[2][2][1] *= 6;

print_r( $g );

?>