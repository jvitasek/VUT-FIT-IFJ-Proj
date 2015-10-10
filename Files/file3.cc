/* Program 3: Prace s retezci a vestavenymi funkcemi */
int main()
{
	string str1;
	{ // vnoreny blok s lokalni promennou str2 a pristupem k str1
		int x;
		str1 = "Toto_je_nejaky_text";
		string str2;
		str2 = concat(str1, ",_ktery_jeste_trochy_obohatime");
		x = find(str2, "text");
		cout << "Pozice_retezce_\"text\"_v_retezci_str2:_"
			 << x << "\n";
		cout << "Zadajte_nejakou_posloupnost_vsech_malych_pismen_a-h,_"
				 << "pricemz_se_pismena_nesmeji_v_posloupnosti_opakovat:";
	}
	cin >> str1;
	str1 = sort(str1);
	if (str1 != "abcdefgh")
	{
		for (auto s = str1; s != "abcdefgh"; s = s)
		{
			cout << "Spatne_zadana_posloupnost,_zkuste_znovu:";
			cin >> str1;
			s = sort(str1);
		}
	}
	else {}
	return 0;
}