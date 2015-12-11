# vsechno ok
rm test.out

echo "1:"
./main tests/test1.cc
ret=$?
success=0

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 1: SUCCESS" > test.out
else
	echo "Test 1: ERROR, EXPECTED 0, GOT $ret" > test.out
fi

# lexikalni chyba
echo "2:"
./main tests/test2.cc
ret=$?

if [ "$ret" -eq 1 ];
then
	succ=$((succ+1))
	echo "Test 2: SUCCESS" >> test.out
else
	echo "Test 2: ERROR, EXPECTED 1, GOT $ret" >> test.out
fi

# syntakticka chyba
echo "3:"
./main tests/test3.cc
ret=$?

if [ "$ret" -eq 2 ];
then
	succ=$((succ+1))
	echo "Test 3: SUCCESS" >> test.out
else
	echo "Test 3: ERROR, EXPECTED 2, GOT $ret" >> test.out
fi

# semanticka chyba – redefinice funkce
echo "4:"
./main tests/test4.cc
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 4: SUCCESS" >> test.out
else
	echo "Test 4: ERROR, EXPECTED 3, GOT $ret" >> test.out
fi

# semanticka chyba – nedefinovana promenna
# plus test promenne z jineho ramce
echo "5:"
./main tests/test5.cc
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 5: SUCCESS" >> test.out
else
	echo "Test 5: ERROR, EXPECTED 3, GOT $ret" >> test.out
fi

# semantika – ramce funkci, stejna promenna
# v ramci jine funkce
echo "6:"
./main tests/test6.cc
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 6: SUCCESS" >> test.out
else
	echo "Test 6: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# precedencni syntakticka analyza
# expression ukonecny zavorkou
echo "7:"
./main tests/test7.cc
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 7: SUCCESS" >> test.out
else
	echo "Test 7: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# precedencni syntakticka analyza
# expression ukonecny strednikem
echo "8:"
./main tests/test8.cc
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 8: SUCCESS" >> test.out
else
	echo "Test 8: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# precedencni syntakticka analyza
# expression ukonecny strednikem
# > o jednu zavorku mene
echo "9:"
./main tests/test9.cc
ret=$?

if [ "$ret" -eq 2 ];
then
	succ=$((succ+1))
	echo "Test 9: SUCCESS" >> test.out
else
	echo "Test 9: ERROR, EXPECTED 2, GOT $ret" >> test.out
fi

# precedencni syntakticka analyza
# expression ukonecny zavorkou
# > o jednu zavorku mene
echo "10:"
./main tests/test10.cc
ret=$?

if [ "$ret" -eq 2 ];
then
	succ=$((succ+1))
	echo "Test 10: SUCCESS" >> test.out
else
	echo "Test 10: ERROR, EXPECTED 2, GOT $ret" >> test.out
fi

# precedencni syntakticka analyza
# expression ukonecny strednikem
# > o jednu zavorku vice
echo "11:"
./main tests/test11.cc
ret=$?

if [ "$ret" -eq 2 ];
then
	succ=$((succ+1))
	echo "Test 11: SUCCESS" >> test.out
else
	echo "Test 11: ERROR, EXPECTED 2, GOT $ret" >> test.out
fi

# precedencni syntakticka analyza
# expression ukonecny zavorkou
# > o jednu zavorku vice
echo "12:"
./main tests/test12.cc
ret=$?

if [ "$ret" -eq 2 ];
then
	succ=$((succ+1))
	echo "Test 12: SUCCESS" >> test.out
else
	echo "Test 12: ERROR, EXPECTED 2, GOT $ret" >> test.out
fi

# precedencni syntakticka analyza
# dve plus vedle sebe
echo "13:"
./main tests/test13.cc
ret=$?

if [ "$ret" -eq 2 ];
then
	succ=$((succ+1))
	echo "Test 13: SUCCESS" >> test.out
else
	echo "Test 13: ERROR, EXPECTED 2, GOT $ret" >> test.out
fi

# prazdny soubor
echo "14:"
./main tests/test14.cc
ret=$?

if [ "$ret" -eq 2 ];
then
	succ=$((succ+1))
	echo "Test 14: SUCCESS" >> test.out
else
	echo "Test 14: ERROR, EXPECTED 2, GOT $ret" >> test.out
fi

# volani funkce s parametry
echo "15:"
./main tests/test15.cc
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 15: SUCCESS" >> test.out
else
	echo "Test 15: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# parametr pouzit v tele funkce
echo "16:"
./main tests/test16.cc
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 16: SUCCESS" >> test.out
else
	echo "Test 16: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# redefinice parametru
echo "17:"
./main tests/test17.cc
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 17: SUCCESS" >> test.out
else
	echo "Test 17: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# kontrola volani funkce
echo "18:"
./main tests/test18.cc
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 18: SUCCESS" >> test.out
else
	echo "Test 18: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# kontrola volani funkce
echo "19:"
./main tests/test19.cc
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 19: SUCCESS" >> test.out
else
	echo "Test 19: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# kontrola typu parametru volani funkce
echo "20:"
./main tests/test20.cc
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 20: SUCCESS" >> test.out
else
	echo "Test 20: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# kontrola typu parametru volani funkce
echo "21:"
./main tests/test21.cc
ret=$?

if [ "$ret" -eq 4 ];
then
	succ=$((succ+1))
	echo "Test 21: SUCCESS" >> test.out
else
	echo "Test 21: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# kontrola typu parametru volani funkce, vice parametru
echo "22:"
./main tests/test22.cc
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 22: SUCCESS" >> test.out
else
	echo "Test 22: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# kontrola typu parametru volani funkce, vice parametru
echo "23:"
./main tests/test23.cc
ret=$?

if [ "$ret" -eq 4 ];
then
	succ=$((succ+1))
	echo "Test 23: SUCCESS" >> test.out
else
	echo "Test 23: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# kontrola, zda existuje funkce main
echo "24:"
./main tests/test24.cc
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 24: SUCCESS" >> test.out
else
	echo "Test 24: ERROR, EXPECTED 3, GOT $ret" >> test.out
fi

# pocet parametru pri volani funkce
echo "25:"
./main tests/test25.cc
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 25: SUCCESS" >> test.out
else
	echo "Test 25: ERROR, EXPECTED 3, GOT $ret" >> test.out
fi

# pocet parametru pri volani funkce
echo "26:"
./main tests/test26.cc
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 26: SUCCESS" >> test.out
else
	echo "Test 26: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# volani deklarovane, ale nedefinovane funkce
echo "27:"
./main tests/test27.cc
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 27: SUCCESS" >> test.out
else
	echo "Test 27: ERROR, EXPECTED 3, GOT $ret" >> test.out
fi

# volani definovane funkce
echo "28:"
./main tests/test28.cc
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 28: SUCCESS" >> test.out
else
	echo "Test 28: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# deleni nulou
echo "29:"
./main tests/test29.cc
ret=$?

if [ "$ret" -eq 9 ];
then
	succ=$((succ+1))
	echo "Test 29: SUCCESS" >> test.out
else
	echo "Test 29: ERROR, EXPECTED 9, GOT $ret" >> test.out
fi

# deleni nulou
echo "30:"
./main tests/test30.cc
ret=$?

if [ "$ret" -eq 9 ];
then
	succ=$((succ+1))
	echo "Test 30: SUCCESS" >> test.out
else
	echo "Test 30: ERROR, EXPECTED 9, GOT $ret" >> test.out
fi

# vetev else
echo "31:"
./main tests/test31.cc
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 31: SUCCESS" >> test.out
else
	echo "Test 31: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

# zpracovani vice parametru pri volani fce ve vyrazech (zde chyba)
echo "32:"
./main tests/test32.cc
ret=$?

if [ "$ret" -eq 4 ];
then
	succ=$((succ+1))
	echo "Test 32: SUCCESS" >> test.out
else
	echo "Test 32: ERROR, EXPECTED 4, GOT $ret" >> test.out
fi

# zpracovani vice parametru pri volani fce ve vyrazech
echo "33:"
./main tests/test33.cc
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 33: SUCCESS" >> test.out
else
	echo "Test 33: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

echo "#################"
echo "SUCCESS: $succ/33"



