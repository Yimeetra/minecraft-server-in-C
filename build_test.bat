gcc -c src/test.c
gcc -c src/md5.c
gcc test.o md5.o -o test