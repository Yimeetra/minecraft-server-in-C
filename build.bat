gcc -c src/server.c src/md5.c src/Protocol.c src/ByteArray.c
gcc server.o md5.o Protocol.o ByteArray.o -o server -lws2_32