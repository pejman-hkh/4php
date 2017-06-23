short open tag for php with equal for printing
<?="test ast";print('testtttt');?>

<?="test ast"?>

<?4php

echo( 3 * 8 + 2.2 );
echo( implode(",", [ 'a', 'b', 'c' ] ) );


print_r( explode(",", "a,b,c") );

//die('test');
//exit();

//while statement
$i = 0;
while( $i < 10 ) {
	echo( $i );

	if( $i == 5 ) {
		$i++;
		continue;
	}

	if( $i == 6 ) {
		break;
	}

	echo("dddd");
	$i++;
}
?>

<?
//for with break & continue;
for( $i = 0; $i < 10; $i++ ) {
	echo( $i );

	if( $i == 5 ) {
		$i++;
		continue;
	}

	if( $i == 6 ) {
		break;
	}

	echo("dddd");
}



$arr = [
	0 => [ 'id' => 1, 'name' => 'peji1' ],
	1 => [ 'id' => 2, 'name' => 'peji2' ],
	2 => [ 'id' => 3, 'name' => 'peji3' ],
];

//foreach with break and continue. you can use break too.
foreach( $arr as $k => $v ) {
	
	if( $v['name'] == "peji2" ) {
		continue;
	}

	print_r( $v );
}

echo("test");
echo("end");
?>
asdasd

test