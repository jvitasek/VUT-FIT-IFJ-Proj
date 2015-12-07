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
echo "#################"
echo "SUCCESS: $succ/19"



