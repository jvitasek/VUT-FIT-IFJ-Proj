/* Program : Vypocet faktorialu (iterativne) */
int main()
{
	int a;
	int vysl;
	cout << "Zadajte_cislo_pro_vypocet_faktorialu:_";
	cin >> a;
	if (a < 0)	// cin nacitani zaporneho cisla nemusi podporovat
	{
		cout << "Faktorial_nelze_spocitat!\n";
	}
	else
	{
		vysl = 1;
		for (int foo; a > 0; a = a - 1)
		{
			vysl = vysl * a;
		}
		cout << "Vysledek_je:_" << vysl << "\n";
	}
	return 0;
}