cd build
gcc -c ../src/server.c ../src/md5.c ../src/ByteArray.c ../src/Packet.c ../src/Protocol.c ../src/Client.c -g
gcc server.o md5.o ByteArray.o Packet.o Protocol.o Client.o -o server -lws2_32 -g