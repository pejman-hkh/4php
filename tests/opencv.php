<?4php

$o = new opencv();

print_r( $o );

$a = $o->imread("test.png");


print_r( $o );

$o->threshold( $a, 255,1,1,5,5 );

echo("after threshold");

$o->imwrite("test1.png", $a );

echo("end");

?>