<?4php
$SUPERGLOBALS['_GET'] = array( 'test', 'test1', 'test2' );

print_r( $_GET );

function test() {
	print_r( $_GET );
}

test();

?>