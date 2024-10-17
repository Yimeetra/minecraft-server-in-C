#pragma once

#include "Block.h"
#include <stdbool.h>

typedef struct ChunkSection {
    short block_count;
    unsigned int states[16][16][16];
} ChunkSection;

typedef struct Chunk {
    int x;
    int z;
    ChunkSection sections[24];
} Chunk;

typedef struct Palette {
    Byte bits_per_entry;
    int length;
    int *map;
} Palette;


typedef struct HeightMap {
    int heightmap[16][16];
} HeightMap;

void Chunk_to_ByteArray(Chunk *chunk, ByteArray *byteArray);
void ChunkSection_to_ByteArray(ChunkSection *chunkSection, ByteArray *byteArray);
void Palette_add_state(Palette *palette, int state);
Palette Palette_new();