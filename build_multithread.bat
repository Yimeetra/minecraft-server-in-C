cd build
gcc -c ../src/server_multithread.c ../src/md5.c ../src/ByteArray.c ../src/Packet.c ../src/Protocol.c ../src/Client.c ../src/cJSON/cJSON.c ../src/registries.c ../src/Chunk.c -g2 -O0
gcc server_multithread.o md5.o ByteArray.o Packet.o Protocol.o Client.o cJSON.o registries.o Chunk.o -o server -lws2_32 -lpthread -g2 -O0