# vsechno ok
rm test.out
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
./main tests/test15.cc
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 15: SUCCESS" >> test.out
else
	echo "Test 15: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

echo "#################"
echo "SUCCESS: $succ/15"
