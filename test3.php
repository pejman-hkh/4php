<?4php
echo(  2.2 + 3 * 8 );

$a = [];

$a[] = "teeest";
$a[] = 3;
$a[] = 4;


print_r( $a );

?>

<?4php
echo( 3 * ( 5 + 2 ) );

echo("\n");

//nested for check
for( $i = 0; $i < 5; $i++ ) {
	echo( $i );
	for( $j = 0; $j < 5; $j++ ) {
		echo( $j );
	}
	echo("\n");
}

echo("end");

?>
