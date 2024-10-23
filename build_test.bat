cd build
::gcc -c ../src/test.c ../src/md5.c ../src/ByteArray.c ../src/Packet.c ../src/Protocol.c ../src/Client.c ../src/cJSON/cJSON.c -g
::gcc test.o md5.o ByteArray.o Packet.o Protocol.o Client.o cJSON.o -o test -lws2_32 -g
gcc ../src/test.c -o test -O0
cd ..
.\build\test.exe