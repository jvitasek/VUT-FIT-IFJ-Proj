int foo(int param1)
{
	return param1;
}

int main()
{
	int a = 3;
	{
		double pass1 = 2;
		foo(pass1);
	}

	return 0;
}