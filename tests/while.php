<?4php

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
