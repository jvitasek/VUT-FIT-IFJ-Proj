int foo(int param1, double param2)
{
	return param1;
}

int lol()
{
	int a = 3;
	{
		int pass1 = 2;
		double pass2 = 2;
		foo(pass1, pass2);
	}

	return 0;
}