# vsechno ok
rm adv-test.out
success=0
error=0

./main adv-tests/001_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 1: SUCCESS" > adv-test.out
else
	error=$((error+1))
	echo "Test 1: ERROR, EXPECTED 0, GOT $ret" > adv-test.out
fi

./main adv-tests/002_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 2: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 2: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/003_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 3: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 3: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/004_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 4: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 4: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/005_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 5: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 5: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/006_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 6: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 6: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/007_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 7: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 7: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/008_R2_.c
ret=$?

if [ "$ret" -eq 2 ];
then
	succ=$((succ+1))
	echo "Test 8: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 8: ERROR, EXPECTED 2, GOT $ret" >> adv-test.out
fi

./main adv-tests/009_R1_.c
ret=$?

if [ "$ret" -eq 1 ];
then
	succ=$((succ+1))
	echo "Test 9: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 9: ERROR, EXPECTED 1, GOT $ret" >> adv-test.out
fi

./main adv-tests/011_R2_.c
ret=$?

if [ "$ret" -eq 2 ];
then
	succ=$((succ+1))
	echo "Test 11: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 11: ERROR, EXPECTED 2, GOT $ret" >> adv-test.out
fi

./main adv-tests/012_R2_.c
ret=$?

if [ "$ret" -eq 2 ];
then
	succ=$((succ+1))
	echo "Test 12: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 12: ERROR, EXPECTED 2, GOT $ret" >> adv-test.out
fi

./main adv-tests/013_R2_.c
ret=$?

if [ "$ret" -eq 2 ];
then
	succ=$((succ+1))
	echo "Test 13: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 13: ERROR, EXPECTED 2, GOT $ret" >> adv-test.out
fi

./main adv-tests/014_R2_.c
ret=$?

if [ "$ret" -eq 2 ];
then
	succ=$((succ+1))
	echo "Test 14: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 14: ERROR, EXPECTED 2, GOT $ret" >> adv-test.out
fi

./main adv-tests/015_R2_.c
ret=$?

if [ "$ret" -eq 2 ];
then
	succ=$((succ+1))
	echo "Test 15: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 15: ERROR, EXPECTED 2, GOT $ret" >> adv-test.out
fi

./main adv-tests/016_R2_.c
ret=$?

if [ "$ret" -eq 2 ];
then
	succ=$((succ+1))
	echo "Test 16: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 16: ERROR, EXPECTED 2, GOT $ret" >> adv-test.out
fi

./main adv-tests/017_R3_.c
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 17: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 17: ERROR, EXPECTED 3, GOT $ret" >> adv-test.out
fi

# PROBLEM: jinak pojmenovany parametr @todo!
./main adv-tests/018_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 18: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 18: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

# PROBLEM: jina navratova hodnota funkce @todo!
./main adv-tests/019_R3_.c
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 19: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 19: ERROR, EXPECTED 3, GOT $ret" >> adv-test.out
fi

./main adv-tests/020_R3_.c
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 20: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 20: ERROR, EXPECTED 3, GOT $ret" >> adv-test.out
fi

./main adv-tests/021_R3_.c
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 21: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 21: ERROR, EXPECTED 3, GOT $ret" >> adv-test.out
fi

./main adv-tests/022_R3_.c
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 22: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 22: ERROR, EXPECTED 3, GOT $ret" >> adv-test.out
fi

# PROBLEM: nevraci chybu, mel by vracet semantickou, protoze main
# 		   nesmi mit parametry
./main adv-tests/023_R3_.c
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 23: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 23: ERROR, EXPECTED 3, GOT $ret" >> adv-test.out
fi

# PROBLEM: vraci syntaktickou chybu, ma vracet semantickou,
# 		   protoze foo je jen deklarovano, ne definovano,
# 		   ale je volano v mainu
./main adv-tests/024_R3_.c
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 24: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 24: ERROR, EXPECTED 3, GOT $ret" >> adv-test.out
fi

# PROBLEM: parametry nesouhlasi
./main adv-tests/025_R4_.c
ret=$?

if [ "$ret" -eq 4 ];
then
	succ=$((succ+1))
	echo "Test 25: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 25: ERROR, EXPECTED 4, GOT $ret" >> adv-test.out
fi

./main adv-tests/026_R4_.c
ret=$?

if [ "$ret" -eq 4 ];
then
	succ=$((succ+1))
	echo "Test 26: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 26: ERROR, EXPECTED 4, GOT $ret" >> adv-test.out
fi

./main adv-tests/027_R4_.c
ret=$?

if [ "$ret" -eq 4 ];
then
	succ=$((succ+1))
	echo "Test 27: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 27: ERROR, EXPECTED 4, GOT $ret" >> adv-test.out
fi

./main adv-tests/028_R4_.c
ret=$?

if [ "$ret" -eq 4 ];
then
	succ=$((succ+1))
	echo "Test 28: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 28: ERROR, EXPECTED 4, GOT $ret" >> adv-test.out
fi

./main adv-tests/030_2_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 30-2: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 30-2: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/030_3_R10_.c
ret=$?

if [ "$ret" -eq 1 ];
then
	succ=$((succ+1))
	echo "Test 30-3: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 30-3: ERROR, EXPECTED 1, GOT $ret" >> adv-test.out
fi

./main adv-tests/030_R5_.c
ret=$?

if [ "$ret" -eq 50 ];
then
	succ=$((succ+1))
	echo "Test 30: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 30: ERROR, EXPECTED 50, GOT $ret" >> adv-test.out
fi

./main adv-tests/031_2_R6_.c
ret=$?

if [ "$ret" -eq 60 ];
then
	succ=$((succ+1))
	echo "Test 31-2: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 31-2: ERROR, EXPECTED 60, GOT $ret" >> adv-test.out
fi

./main adv-tests/031_R6_.c
ret=$?

if [ "$ret" -eq 60 ];
then
	succ=$((succ+1))
	echo "Test 31: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 31: ERROR, EXPECTED 60, GOT $ret" >> adv-test.out
fi

./main adv-tests/032_2_R8_.c
ret=$?

if [ "$ret" -eq 8 ];
then
	succ=$((succ+1))
	echo "Test 32-2: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 32-2: ERROR, EXPECTED 8, GOT $ret" >> adv-test.out
fi

./main adv-tests/032_R6_.c
ret=$?

if [ "$ret" -eq 60 ];
then
	succ=$((succ+1))
	echo "Test 32: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 32: ERROR, EXPECTED 8, GOT $ret" >> adv-test.out
fi

./main adv-tests/033_2_R4_.c
ret=$?

if [ "$ret" -eq 4 ];
then
	succ=$((succ+1))
	echo "Test 33-2: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 33-2: ERROR, EXPECTED 4, GOT $ret" >> adv-test.out
fi

./main adv-tests/033_3_R4_.c
ret=$?

if [ "$ret" -eq 4 ];
then
	succ=$((succ+1))
	echo "Test 33-3: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 33-3: ERROR, EXPECTED 4, GOT $ret" >> adv-test.out
fi

./main adv-tests/033_4_R4_.c
ret=$?

if [ "$ret" -eq 4 ];
then
	succ=$((succ+1))
	echo "Test 33-4: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 33-4: ERROR, EXPECTED 4, GOT $ret" >> adv-test.out
fi

./main adv-tests/033_5_R4_.c
ret=$?

if [ "$ret" -eq 4 ];
then
	succ=$((succ+1))
	echo "Test 33-5: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 33-5: ERROR, EXPECTED 4, GOT $ret" >> adv-test.out
fi

./main adv-tests/033_6_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 33-6: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 33-6: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/033_7_R10_.c
ret=$?

if [ "$ret" -eq 1 ];
then
	succ=$((succ+1))
	echo "Test 33-7: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 33-7: ERROR, EXPECTED 1, GOT $ret" >> adv-test.out
fi

./main adv-tests/033_R4_.c
ret=$?

if [ "$ret" -eq 4 ];
then
	succ=$((succ+1))
	echo "Test 33: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 33: ERROR, EXPECTED 4, GOT $ret" >> adv-test.out
fi

./main adv-tests/034_2_R7__cin.txt
ret=$?

if [ "$ret" -eq 7 ];
then
	succ=$((succ+1))
	echo "Test 34-2cin: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 34-2cin: ERROR, EXPECTED 7, GOT $ret" >> adv-test.out
fi

./main adv-tests/034_2_R7_.c
ret=$?

if [ "$ret" -eq 7 ];
then
	succ=$((succ+1))
	echo "Test 34-2: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 34-2: ERROR, EXPECTED 7, GOT $ret" >> adv-test.out
fi

./main adv-tests/034_3_R0__cin.txt
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 34-3cin: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 34-3cin: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/034_3_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 34-3: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 34-3: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/034_4_R0__cin.txt
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 34-4cin: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 34-4cin: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/034_4_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 34-4: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 34-4: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/034_R7__cin.txt
ret=$?

if [ "$ret" -eq 7 ];
then
	succ=$((succ+1))
	echo "Test 34cin: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 34cin: ERROR, EXPECTED 7, GOT $ret" >> adv-test.out
fi

./main adv-tests/034_R7_.c
ret=$?

if [ "$ret" -eq 7 ];
then
	succ=$((succ+1))
	echo "Test 34: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 34: ERROR, EXPECTED 7, GOT $ret" >> adv-test.out
fi

./main adv-tests/035_2_R10__cin.txt
ret=$?

if [ "$ret" -eq 1 ];
then
	succ=$((succ+1))
	echo "Test 35-2cin: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 35-2cin: ERROR, EXPECTED 1, GOT $ret" >> adv-test.out
fi

./main adv-tests/035_2_R10_.c
ret=$?

if [ "$ret" -eq 1 ];
then
	succ=$((succ+1))
	echo "Test 35-2: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 35-2: ERROR, EXPECTED 1, GOT $ret" >> adv-test.out
fi

./main adv-tests/035_3_R10__cin.txt
ret=$?

if [ "$ret" -eq 1 ];
then
	succ=$((succ+1))
	echo "Test 35-3cin: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 35-3cin: ERROR, EXPECTED 1, GOT $ret" >> adv-test.out
fi

./main adv-tests/035_3_R10_.c
ret=$?

if [ "$ret" -eq 1 ];
then
	succ=$((succ+1))
	echo "Test 35-3: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 35-3: ERROR, EXPECTED 1, GOT $ret" >> adv-test.out
fi

./main adv-tests/035_4_R10__cin.txt
ret=$?

if [ "$ret" -eq 1 ];
then
	succ=$((succ+1))
	echo "Test 35-4cin: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 35-4cin: ERROR, EXPECTED 1, GOT $ret" >> adv-test.out
fi

./main adv-tests/035_4_R10_.c
ret=$?

if [ "$ret" -eq 1 ];
then
	succ=$((succ+1))
	echo "Test 35-4: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 35-4: ERROR, EXPECTED 1, GOT $ret" >> adv-test.out
fi

./main adv-tests/035_R0__cin.txt
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 35cin: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 35cin: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/035_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 35: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 35: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/036_2_R8_.c
ret=$?

if [ "$ret" -eq 8 ];
then
	succ=$((succ+1))
	echo "Test 36-2: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 36-2: ERROR, EXPECTED 8, GOT $ret" >> adv-test.out
fi

./main adv-tests/036_4_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 36-4: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 36-4: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/036_5_R8_.c
ret=$?

if [ "$ret" -eq 8 ];
then
	succ=$((succ+1))
	echo "Test 36-5: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 36-5: ERROR, EXPECTED 8, GOT $ret" >> adv-test.out
fi

./main adv-tests/036_6_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 36-6: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 36-6: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

./main adv-tests/036_problematic_3_R8_.c
ret=$?

if [ "$ret" -eq 8 ];
then
	succ=$((succ+1))
	echo "Test 36prob: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 36prob: ERROR, EXPECTED 8, GOT $ret" >> adv-test.out
fi

./main adv-tests/036_R8_.c
ret=$?

if [ "$ret" -eq 8 ];
then
	succ=$((succ+1))
	echo "Test 36: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 36: ERROR, EXPECTED 8, GOT $ret" >> adv-test.out
fi

./main adv-tests/037_2_R9_.c
ret=$?

if [ "$ret" -eq 9 ];
then
	succ=$((succ+1))
	echo "Test 37-2: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 37-2: ERROR, EXPECTED 9, GOT $ret" >> adv-test.out
fi

./main adv-tests/037_3_R9_.c
ret=$?

if [ "$ret" -eq 9 ];
then
	succ=$((succ+1))
	echo "Test 37-3: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 37-3: ERROR, EXPECTED 9, GOT $ret" >> adv-test.out
fi

./main adv-tests/037_4_R9_.c
ret=$?

if [ "$ret" -eq 9 ];
then
	succ=$((succ+1))
	echo "Test 37-4: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 37-4: ERROR, EXPECTED 9, GOT $ret" >> adv-test.out
fi

./main adv-tests/037_R9_.c
ret=$?

if [ "$ret" -eq 9 ];
then
	succ=$((succ+1))
	echo "Test 37: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 37: ERROR, EXPECTED 9, GOT $ret" >> adv-test.out
fi

./main adv-tests/038_R10_.c
ret=$?

if [ "$ret" -eq 1 ];
then
	succ=$((succ+1))
	echo "Test 38: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 38: ERROR, EXPECTED 1, GOT $ret" >> adv-test.out
fi

./main adv-tests/039_2_R10_.c
ret=$?

if [ "$ret" -eq 1 ];
then
	succ=$((succ+1))
	echo "Test 39-2: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 39-2: ERROR, EXPECTED 1, GOT $ret" >> adv-test.out
fi

./main adv-tests/039_3_R3_.c
ret=$?

if [ "$ret" -eq 3 ];
then
	succ=$((succ+1))
	echo "Test 39-3: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 39-3: ERROR, EXPECTED 3, GOT $ret" >> adv-test.out
fi

./main adv-tests/039_4_R2_.c
ret=$?

if [ "$ret" -eq 4 ];
then
	succ=$((succ+1))
	echo "Test 39-4: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 39-4: ERROR, EXPECTED 4, GOT $ret" >> adv-test.out
fi

./main adv-tests/039_5_R1_.c
ret=$?

if [ "$ret" -eq 1 ];
then
	succ=$((succ+1))
	echo "Test 39-5: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 39-5: ERROR, EXPECTED 1, GOT $ret" >> adv-test.out
fi

./main adv-tests/039_6_R4_.c
ret=$?

if [ "$ret" -eq 4 ];
then
	succ=$((succ+1))
	echo "Test 39-6: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 39-6: ERROR, EXPECTED 4, GOT $ret" >> adv-test.out
fi

./main adv-tests/039_7_R8_.c
ret=$?

if [ "$ret" -eq 8 ];
then
	succ=$((succ+1))
	echo "Test 39-7: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 39-7: ERROR, EXPECTED 8, GOT $ret" >> adv-test.out
fi

./main adv-tests/039_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 39: SUCCESS" >> adv-test.out
else
	error=$((error+1))
	echo "Test 39: ERROR, EXPECTED 0, GOT $ret" >> adv-test.out
fi

total=$((succ+error))
echo "#################"
echo "SUCCESS: $succ/$total"
