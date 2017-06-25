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


//nested for check
for( $i = 0; $i < 5; $i++ ) {
	echo( $i );
	for( $j = 0; $j < 5; $j++ ) {
		echo( $j );
	}
	echo("\n");
}
