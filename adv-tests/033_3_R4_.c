string func( string p1, int p2, double p4 ) {
	concat( p1, "asd" );
	return 0;
}

int main() {
	int x;
	func( "lol", true, true );

	if( x == "lolasd" )
		return 0;
}