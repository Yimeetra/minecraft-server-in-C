gcc -c src/server.c
gcc -c src/md5.c
gcc server.o md5.o -o server -lws2_32