int foo(int param1, int param2)
{

}

int main()
{
	int a = 3;

	{
		int prom = 2;
		foo(prom, prom);
	}

	return 0;
}