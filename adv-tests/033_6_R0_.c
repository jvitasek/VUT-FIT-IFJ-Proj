string func( string p1, int p2, double p4 ) {
	concat( p1, "asd" );
	return 0;
}

int main() {
	string x;
	x = func( "lol", 3.4, 5.2 );

	if( x == "lolasd" )
		return 0;
}