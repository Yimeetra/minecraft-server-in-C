#include "ByteArray.h"

typedef struct Block {
    int x;
    int y;
    int z;
    int type;
    ByteArray data;
} Block;