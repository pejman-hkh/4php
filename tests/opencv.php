<?4php

$o = new opencv();

print_r( $o );

$a = $o->imread("test.png");


print_r( $o );

$d = $o->threshold( $a, 100,255,1 );

echo("after threshold");

$o->imwrite("test1.png", $d );

echo("end");

?>