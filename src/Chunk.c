#include "Chunk.h"
#include <malloc.h>

HeightMap Chunk_heightmap(Chunk chunk) {
    HeightMap heightmap = {0};
    for (int s = 0; s < 24; ++s) {
        for (int y = 0; y < 16; ++y) {
            for (int z = 0; z < 16; ++z) {
                for (int x = 0; x < 16; ++x) {
                    if (chunk.sections[s].blocks[y][z][x] != NULL) {
                        if (heightmap.heightmap[z][x] < y) heightmap.heightmap[z][x] = y;
                    }
                }
            }
        }
    }
}

ByteArray HeightMap_to_bytearray(HeightMap heightmap) {
    
}