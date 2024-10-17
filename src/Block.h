#include "ByteArray.h"

typedef enum BlockState {
    AIR,
    STONE,
} BlockType;

typedef struct Block {
    BlockType type;
    unsigned int state;
} Block;