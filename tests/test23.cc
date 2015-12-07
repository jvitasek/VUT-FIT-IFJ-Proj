int foo(int param1, double param2)
{
	return param1;
}

int main()
{
	int a = 3;
	{
		int pass1 = 2;
		int pass2 = 2;
		foo(pass1, pass2);
	}

	return 0;
}