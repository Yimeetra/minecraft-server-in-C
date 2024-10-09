cd build
gcc -c ../src/test_ByteArray.c ../src/ByteArray.c
gcc test_ByteArray.o ByteArray.o -o test_ByteArray.exe


gcc -c ../src/test_Packet.c ../src/ByteArray.c ../src/Packet.c
gcc test_Packet.o ByteArray.o Packet.o -o test_Packet.exe

test_ByteArray.exe
test_Packet.exe