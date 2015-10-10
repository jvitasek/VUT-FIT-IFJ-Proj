/* Program : Vypocet faktorialu (rekurzivne) */

int factorial(int n) // Definice funkce pro vypocet hodnoty faktorialu
{
	int temp_result;
	auto decremented_n = n - 1;

	if (n < 2) {
		return 1;
	} else {
		temp_result = factorial(decremented_n);
		return n * temp_result;
	}
}

int main()
{
	int a; int vysl;

	cout << "Zadajte_cislo_pro_vypocet_faktorialu:_";
	cin >> a;

	if (a < 0) {
		cout << "Faktorial_nelze_spocitat!\n";
	}	else {
		vysl = factorial(a);
		auto neg = 0 - vysl;
		cout << "Vysledek:_" << vysl << "_(zaporny:_" << neg << ")\n";
	}

	return 0;
}