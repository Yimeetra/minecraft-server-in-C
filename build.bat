cd build
gcc -c ../src/server.c ../src/md5.c ../src/ByteArray.c ../src/Packet.c ../src/Protocol.c ../src/Client.c ../src/cJSON/cJSON.c -g
gcc server.o md5.o ByteArray.o Packet.o Protocol.o Client.o cJSON.o -o server -lws2_32 -g