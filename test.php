<?$t = 15?>
t is :
<?=$t?>

test
out of php tag

<?4php
echo("test");



?>

test ast...

<?
//no syntax check yet
//no runtime error

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
	echo("u is 2\n ");
} else {
	echo("u is ".$u."\n" );	
}

//simple echo with paren ( echo with no paren not supported yet )
echo("start\n");

$t = microtime(true);

//call function before defination
test('peji');

//define function
function test( $a ) {
	echo( "a is : ".$a."\n" );

	$u = 2;

	$l = 0;
	if( $u == "2" ) {
		echo("u is 1\n");
	} else if ( $l == 0 ) {
		echo("u is 2\n");
	} else {
		echo("u is ".$u."\n" );	
	}

	echo("end\n");
}


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


//for
for( $i = 0; $i < 10; $i++ ) {
	echo( $i."\n" );
}

echo("\nwhat is wrong ? ");



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

print_r( $g, true );

echo( microtime(true) - $t );
