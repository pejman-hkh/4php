<?4php

$r = [ 10 => [ 5 => 3 ], 'ss' => 'ss1' ];


//change value of array
$r[10][5] = "test";
print_r( $r );

echo( "index value is : "+ $r[10][5]."\n" );

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

echo( count( $r ) );
?>