# vsechno ok
rm test.out
./main adv-tests/001_R0_.c
ret=$?
success=0
error=0

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 1: SUCCESS" > test.out
else
	error=$((error+1))
	echo "Test 1: ERROR, EXPECTED 0, GOT $ret" > test.out
fi

./main adv-tests/002_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 2: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 2: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/003_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 3: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 3: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/004_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 4: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 4: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/005_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 5: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 5: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/006_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 6: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 6: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/007_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 7: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 7: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/008_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 8: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 8: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/009_R1_.c
ret=$?

if [ "$ret" -eq 10 ];
then
	succ=$((succ+1))
	echo "Test 9: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 9: ERROR, EXPECTED 10, GOT $ret" >> test.out
fi

./main adv-tests/010_R1_.c
ret=$?

if [ "$ret" -eq 10 ];
then
	succ=$((succ+1))
	echo "Test 10: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 10: ERROR, EXPECTED 10, GOT $ret" >> test.out
fi

./main adv-tests/011_R2_.c
ret=$?

if [ "$ret" -eq 20 ];
then
	succ=$((succ+1))
	echo "Test 11: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 11: ERROR, EXPECTED 20, GOT $ret" >> test.out
fi

./main adv-tests/012_R2_.c
ret=$?

if [ "$ret" -eq 20 ];
then
	succ=$((succ+1))
	echo "Test 12: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 12: ERROR, EXPECTED 20, GOT $ret" >> test.out
fi

./main adv-tests/013_R2_.c
ret=$?

if [ "$ret" -eq 20 ];
then
	succ=$((succ+1))
	echo "Test 13: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 13: ERROR, EXPECTED 20, GOT $ret" >> test.out
fi

./main adv-tests/014_R2_.c
ret=$?

if [ "$ret" -eq 20 ];
then
	succ=$((succ+1))
	echo "Test 14: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 14: ERROR, EXPECTED 20, GOT $ret" >> test.out
fi

./main adv-tests/015_R2_.c
ret=$?

if [ "$ret" -eq 20 ];
then
	succ=$((succ+1))
	echo "Test 15: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 15: ERROR, EXPECTED 20, GOT $ret" >> test.out
fi

./main adv-tests/016_R2_.c
ret=$?

if [ "$ret" -eq 20 ];
then
	succ=$((succ+1))
	echo "Test 16: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 16: ERROR, EXPECTED 20, GOT $ret" >> test.out
fi

./main adv-tests/017_R3_.c
ret=$?

if [ "$ret" -eq 30 ];
then
	succ=$((succ+1))
	echo "Test 17: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 17: ERROR, EXPECTED 30, GOT $ret" >> test.out
fi

./main adv-tests/018_R3_.c
ret=$?

if [ "$ret" -eq 30 ];
then
	succ=$((succ+1))
	echo "Test 18: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 18: ERROR, EXPECTED 30, GOT $ret" >> test.out
fi

./main adv-tests/019_R3_.c
ret=$?

if [ "$ret" -eq 30 ];
then
	succ=$((succ+1))
	echo "Test 19: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 19: ERROR, EXPECTED 30, GOT $ret" >> test.out
fi

./main adv-tests/020_R3_.c
ret=$?

if [ "$ret" -eq 30 ];
then
	succ=$((succ+1))
	echo "Test 20: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 20: ERROR, EXPECTED 30, GOT $ret" >> test.out
fi

./main adv-tests/021_R3_.c
ret=$?

if [ "$ret" -eq 30 ];
then
	succ=$((succ+1))
	echo "Test 21: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 21: ERROR, EXPECTED 30, GOT $ret" >> test.out
fi

./main adv-tests/022_R3_.c
ret=$?

if [ "$ret" -eq 30 ];
then
	succ=$((succ+1))
	echo "Test 22: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 22: ERROR, EXPECTED 30, GOT $ret" >> test.out
fi

./main adv-tests/023_R3_.c
ret=$?

if [ "$ret" -eq 30 ];
then
	succ=$((succ+1))
	echo "Test 23: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 23: ERROR, EXPECTED 30, GOT $ret" >> test.out
fi

./main adv-tests/024_R3_.c
ret=$?

if [ "$ret" -eq 30 ];
then
	succ=$((succ+1))
	echo "Test 24: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 24: ERROR, EXPECTED 30, GOT $ret" >> test.out
fi

./main adv-tests/025_R4_.c
ret=$?

if [ "$ret" -eq 40 ];
then
	succ=$((succ+1))
	echo "Test 25: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 25: ERROR, EXPECTED 40, GOT $ret" >> test.out
fi

./main adv-tests/026_R4_.c
ret=$?

if [ "$ret" -eq 40 ];
then
	succ=$((succ+1))
	echo "Test 26: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 26: ERROR, EXPECTED 40, GOT $ret" >> test.out
fi

./main adv-tests/027_R4_.c
ret=$?

if [ "$ret" -eq 40 ];
then
	succ=$((succ+1))
	echo "Test 27: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 27: ERROR, EXPECTED 40, GOT $ret" >> test.out
fi

./main adv-tests/028_R4_.c
ret=$?

if [ "$ret" -eq 40 ];
then
	succ=$((succ+1))
	echo "Test 28: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 28: ERROR, EXPECTED 40, GOT $ret" >> test.out
fi

./main adv-tests/030_2_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 30-2: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 30-2: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/030_3_R10_.c
ret=$?

if [ "$ret" -eq 10 ];
then
	succ=$((succ+1))
	echo "Test 30-3: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 30-3: ERROR, EXPECTED 10, GOT $ret" >> test.out
fi

./main adv-tests/030_R5_.c
ret=$?

if [ "$ret" -eq 50 ];
then
	succ=$((succ+1))
	echo "Test 30: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 30: ERROR, EXPECTED 50, GOT $ret" >> test.out
fi

./main adv-tests/031_2_R6_.c
ret=$?

if [ "$ret" -eq 60 ];
then
	succ=$((succ+1))
	echo "Test 31-2: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 31-2: ERROR, EXPECTED 60, GOT $ret" >> test.out
fi

./main adv-tests/031_R6_.c
ret=$?

if [ "$ret" -eq 60 ];
then
	succ=$((succ+1))
	echo "Test 31: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 31: ERROR, EXPECTED 60, GOT $ret" >> test.out
fi

./main adv-tests/032_2_R8_.c
ret=$?

if [ "$ret" -eq 80 ];
then
	succ=$((succ+1))
	echo "Test 32-2: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 32-2: ERROR, EXPECTED 80, GOT $ret" >> test.out
fi

./main adv-tests/032_R6_.c
ret=$?

if [ "$ret" -eq 60 ];
then
	succ=$((succ+1))
	echo "Test 32: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 32: ERROR, EXPECTED 80, GOT $ret" >> test.out
fi

./main adv-tests/033_2_R4_.c
ret=$?

if [ "$ret" -eq 40 ];
then
	succ=$((succ+1))
	echo "Test 33-2: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 33-2: ERROR, EXPECTED 40, GOT $ret" >> test.out
fi

./main adv-tests/033_3_R4_.c
ret=$?

if [ "$ret" -eq 40 ];
then
	succ=$((succ+1))
	echo "Test 33-3: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 33-3: ERROR, EXPECTED 40, GOT $ret" >> test.out
fi

./main adv-tests/033_4_R4_.c
ret=$?

if [ "$ret" -eq 40 ];
then
	succ=$((succ+1))
	echo "Test 33-4: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 33-4: ERROR, EXPECTED 40, GOT $ret" >> test.out
fi

./main adv-tests/033_5_R4_.c
ret=$?

if [ "$ret" -eq 40 ];
then
	succ=$((succ+1))
	echo "Test 33-5: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 33-5: ERROR, EXPECTED 40, GOT $ret" >> test.out
fi

./main adv-tests/033_6_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 33-6: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 33-6: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/033_7_R10_.c
ret=$?

if [ "$ret" -eq 10 ];
then
	succ=$((succ+1))
	echo "Test 33-7: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 33-7: ERROR, EXPECTED 10, GOT $ret" >> test.out
fi

./main adv-tests/033_R4_.c
ret=$?

if [ "$ret" -eq 40 ];
then
	succ=$((succ+1))
	echo "Test 33: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 33: ERROR, EXPECTED 40, GOT $ret" >> test.out
fi

./main adv-tests/034_2_R7__cin.txt
ret=$?

if [ "$ret" -eq 70 ];
then
	succ=$((succ+1))
	echo "Test 34-2cin: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 34-2cin: ERROR, EXPECTED 70, GOT $ret" >> test.out
fi

./main adv-tests/034_2_R7_.c
ret=$?

if [ "$ret" -eq 70 ];
then
	succ=$((succ+1))
	echo "Test 34-2: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 34-2: ERROR, EXPECTED 70, GOT $ret" >> test.out
fi

./main adv-tests/034_3_R0__cin.txt
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 34-3cin: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 34-3cin: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/034_3_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 34-3: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 34-3: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/034_4_R0__cin.txt
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 34-4cin: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 34-4cin: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/034_4_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 34-4: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 34-4: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/034_R7__cin.txt
ret=$?

if [ "$ret" -eq 70 ];
then
	succ=$((succ+1))
	echo "Test 34cin: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 34cin: ERROR, EXPECTED 70, GOT $ret" >> test.out
fi

./main adv-tests/034_R7_.c
ret=$?

if [ "$ret" -eq 70 ];
then
	succ=$((succ+1))
	echo "Test 34: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 34: ERROR, EXPECTED 70, GOT $ret" >> test.out
fi

./main adv-tests/035_2_R10__cin.txt
ret=$?

if [ "$ret" -eq 10 ];
then
	succ=$((succ+1))
	echo "Test 35-2cin: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 35-2cin: ERROR, EXPECTED 10, GOT $ret" >> test.out
fi

./main adv-tests/035_2_R10_.c
ret=$?

if [ "$ret" -eq 10 ];
then
	succ=$((succ+1))
	echo "Test 35-2: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 35-2: ERROR, EXPECTED 10, GOT $ret" >> test.out
fi

./main adv-tests/035_3_R10__cin.txt
ret=$?

if [ "$ret" -eq 10 ];
then
	succ=$((succ+1))
	echo "Test 35-3cin: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 35-3cin: ERROR, EXPECTED 10, GOT $ret" >> test.out
fi

./main adv-tests/035_3_R10_.c
ret=$?

if [ "$ret" -eq 10 ];
then
	succ=$((succ+1))
	echo "Test 35-3: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 35-3: ERROR, EXPECTED 10, GOT $ret" >> test.out
fi

./main adv-tests/035_4_R10__cin.txt
ret=$?

if [ "$ret" -eq 10 ];
then
	succ=$((succ+1))
	echo "Test 35-4cin: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 35-4cin: ERROR, EXPECTED 10, GOT $ret" >> test.out
fi

./main adv-tests/035_4_R10_.c
ret=$?

if [ "$ret" -eq 10 ];
then
	succ=$((succ+1))
	echo "Test 35-4: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 35-4: ERROR, EXPECTED 10, GOT $ret" >> test.out
fi

./main adv-tests/035_R0__cin.txt
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 35cin: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 35cin: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/035_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 35: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 35: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/036_2_R8_.c
ret=$?

if [ "$ret" -eq 80 ];
then
	succ=$((succ+1))
	echo "Test 36-2: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 36-2: ERROR, EXPECTED 80, GOT $ret" >> test.out
fi

./main adv-tests/036_4_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 36-4: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 36-4: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/036_5_R8_.c
ret=$?

if [ "$ret" -eq 80 ];
then
	succ=$((succ+1))
	echo "Test 36-5: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 36-5: ERROR, EXPECTED 80, GOT $ret" >> test.out
fi

./main adv-tests/036_6_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 36-6: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 36-6: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

./main adv-tests/036_problematic_3_R8_.c
ret=$?

if [ "$ret" -eq 80 ];
then
	succ=$((succ+1))
	echo "Test 36prob: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 36prob: ERROR, EXPECTED 80, GOT $ret" >> test.out
fi

./main adv-tests/036_R8_.c
ret=$?

if [ "$ret" -eq 80 ];
then
	succ=$((succ+1))
	echo "Test 36: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 36: ERROR, EXPECTED 80, GOT $ret" >> test.out
fi

./main adv-tests/037_2_R9_.c
ret=$?

if [ "$ret" -eq 90 ];
then
	succ=$((succ+1))
	echo "Test 37-2: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 37-2: ERROR, EXPECTED 90, GOT $ret" >> test.out
fi

./main adv-tests/037_3_R9_.c
ret=$?

if [ "$ret" -eq 90 ];
then
	succ=$((succ+1))
	echo "Test 37-3: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 37-3: ERROR, EXPECTED 90, GOT $ret" >> test.out
fi

./main adv-tests/037_4_R9_.c
ret=$?

if [ "$ret" -eq 90 ];
then
	succ=$((succ+1))
	echo "Test 37-4: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 37-4: ERROR, EXPECTED 90, GOT $ret" >> test.out
fi

./main adv-tests/037_R9_.c
ret=$?

if [ "$ret" -eq 90 ];
then
	succ=$((succ+1))
	echo "Test 37: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 37: ERROR, EXPECTED 90, GOT $ret" >> test.out
fi

./main adv-tests/038_R10_.c
ret=$?

if [ "$ret" -eq 10 ];
then
	succ=$((succ+1))
	echo "Test 38: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 38: ERROR, EXPECTED 10, GOT $ret" >> test.out
fi

./main adv-tests/039_2_R10_.c
ret=$?

if [ "$ret" -eq 10 ];
then
	succ=$((succ+1))
	echo "Test 39-2: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 39-2: ERROR, EXPECTED 10, GOT $ret" >> test.out
fi

./main adv-tests/039_3_R3_.c
ret=$?

if [ "$ret" -eq 30 ];
then
	succ=$((succ+1))
	echo "Test 39-3: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 39-3: ERROR, EXPECTED 30, GOT $ret" >> test.out
fi

./main adv-tests/039_4_R2_.c
ret=$?

if [ "$ret" -eq 40 ];
then
	succ=$((succ+1))
	echo "Test 39-4: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 39-4: ERROR, EXPECTED 40, GOT $ret" >> test.out
fi

./main adv-tests/039_5_R1_.c
ret=$?

if [ "$ret" -eq 10 ];
then
	succ=$((succ+1))
	echo "Test 39-5: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 39-5: ERROR, EXPECTED 10, GOT $ret" >> test.out
fi

./main adv-tests/039_6_R4_.c
ret=$?

if [ "$ret" -eq 40 ];
then
	succ=$((succ+1))
	echo "Test 39-6: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 39-6: ERROR, EXPECTED 40, GOT $ret" >> test.out
fi

./main adv-tests/039_7_R8_.c
ret=$?

if [ "$ret" -eq 80 ];
then
	succ=$((succ+1))
	echo "Test 39-7: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 39-7: ERROR, EXPECTED 80, GOT $ret" >> test.out
fi

./main adv-tests/039_R0_.c
ret=$?

if [ "$ret" -eq 0 ];
then
	succ=$((succ+1))
	echo "Test 39: SUCCESS" >> test.out
else
	error=$((error+1))
	echo "Test 39: ERROR, EXPECTED 0, GOT $ret" >> test.out
fi

total=$((succ+error))
echo "#################"
echo "SUCCESS: $succ/$total"
