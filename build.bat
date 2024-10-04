cd build
gcc -c ../src/server.c ../src/md5.c ../src/ByteArray.c ../src/Packet.c
gcc server.o md5.o ByteArray.o Packet.o -o server -lws2_32