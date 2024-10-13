#include "Block.h"

typedef struct ChunkSection {
    short block_count;
    Block blocks[16][16][16];
} ChunkSection;

typedef struct Chunk {
    int x;
    int z;
    ChunkSection sections[24];
} Chunk;

typedef struct HeightMap {
    int heightmap[16][16];
} HeightMap;

HeightMap Chunk_heightmap(Chunk chunk);
ChunkSection Chunk_section(Chunk chunk, int index);
ByteArray HeightMap_to_bytearray(HeightMap heightmap);
