int foo()
{
	int a;
}

int main()
{
	int a = 3;
	{
		int a = 4; // toto a ma zastinit a z predchoziho ramce
		a = foo();
	}

	return 0;
}