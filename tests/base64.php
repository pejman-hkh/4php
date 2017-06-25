<?4php

$t = base64_encode("test");
echo( "base64 encoded : " . $t ."\n" );

echo( "base64 decoded : " .base64_decode( $t ) );

?>