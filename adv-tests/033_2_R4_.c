string func( string p1, int p2, bool p3, double p4 ) {
	return concat( p1, "asd" );
}

int main() {
	int x = func( "lol" );

	if( x == "lolasd" )
		return 0;
}