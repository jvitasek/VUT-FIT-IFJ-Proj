int main()
{
	int a = 3;
	{
		int a = 4; // toto a ma zastinit a z predchoziho ramce
		int a = 5;
	}
	int a = 5;
	return 0;
}