cd build
gcc -c ../src/server_singlethread.c ../src/md5.c ../src/ByteArray.c ../src/Packet.c ../src/Protocol.c ../src/Client.c ../src/cJSON/cJSON.c ../src/registries.c ../src/Chunk.c -g
gcc server_singlethread.o md5.o ByteArray.o Packet.o Protocol.o Client.o cJSON.o registries.o Chunk.o -o server -lws2_32 -g 