cd build
gcc -c ../src/test_ByteArray.c ../src/ByteArray.c
gcc test_ByteArray.o ByteArray.o -o test_ByteArray.exe
test_ByteArray.exe