<?4php

$arr = [
	0 => [ 'id' => 1, 'name' => 'peji1' ],
	1 => [ 'id' => 2, 'name' => 'peji2' ],
	2 => [ 'id' => 3, 'name' => 'peji3' ],
];

//nested foreach
foreach( $arr as $k => $v ) {
	//get array index
	print_r( $v['name'] );
	foreach( $v as $k1 => $v1 ) {
		echo(  $k1.' => '.$v1."\n" );
	}
}


//define array like js json
$g = { "test" : "test1", array( 1 => 2 ), [ 1 => 2, 3 ] };

echo( print_r( $g, true ) );


$r = [ 10 => [ 5 => 3 ], 'ss' => 'ss1' ];


//change value of array
$r[10][5] = "test";

print_r( $r );


?>