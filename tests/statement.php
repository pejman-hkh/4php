<?4php

//simple if
$a = 10;
if( $a == 10 ) {
	echo("a is 10\n");
}

//if & else
$s = 12;
if( $s == 10 ) {
	echo("s is 10\n");
} else {
	echo("s is 12\n");
}

//if & else if & elseif & else
$u = 2;
$l = 0;
if( $u == "2" ) {
	echo("u is 1 \n");
} elseif ( $l == 0 ) {
	echo("u is 2\n");
} else {
	echo("u is ".$u."\n" );	
}


$r = [ 10 => [ 5 => 3 ], 'ss' => 'ss1' ];

echo( "index value is : ". $r[10][5]."\n" );

//check if isset variable index
if( $r[11] ) {
	echo("r[11] exists\n");
} else {
	echo("r[11] not exists\n");
}

if( $r[10] ) {
	echo("r[10] exists\n");
} else {
	echo("r[10] not exists\n");
}
