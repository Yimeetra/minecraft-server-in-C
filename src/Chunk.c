#include "Chunk.h"
#include <malloc.h>
#include <stdio.h>

void Chunk_to_ByteArray(Chunk *chunk, ByteArray *byteArray) {
    ByteArray chunkData = ba_new(0);

    ba_append_int(byteArray, chunk->x);
    ba_append_int(byteArray, chunk->z);

    // Heightmaps NBT
    ba_append_byte(byteArray, 0x0A);
    ba_append_byte(byteArray, 0x00);

    for (int i = 0; i < 24; ++i) {
        ChunkSection_to_ByteArray(&chunk->sections[i], &chunkData);
    }

    ba_append_varint(byteArray, chunkData.count);
    ba_append(byteArray, chunkData.bytes, chunkData.count);
    ba_append_varint(byteArray, 0);
}

void ChunkSection_to_ByteArray(ChunkSection *chunkSection, ByteArray *byteArray) {
    Palette palette = Palette_new();
    palette.bits_per_entry = 8;

    bool state_in_palette = false;
    Palette_add_state(&palette, chunkSection->states[0][0][0]);
    for (int y = 0; y < 16; ++y) {
        for (int z = 0; z < 16; ++z) {
            for (int x = 0; x < 16; ++x) {
                if (chunkSection->states[y][z][x] > 0) chunkSection->block_count++;
                state_in_palette = false;
                for (int i = 0; i < palette.length; ++i) {
                    if (chunkSection->states[y][z][x] == palette.map[i]) {
                        state_in_palette = true;
                        break;
                    }
                }
                if (!state_in_palette) {
                    Palette_add_state(&palette, chunkSection->states[y][z][x]);
                }
            }
        }
    }

    ba_append_short(byteArray, chunkSection->block_count);
    ba_append_byte(byteArray, palette.bits_per_entry);
    ba_append_varint(byteArray, palette.length);
    for (int i = 0; i < palette.length; ++i) {
        ba_append_varint(byteArray, palette.map[i]);
    }
    ba_append_varint(byteArray, 16*16*16*palette.bits_per_entry/64);
    for (int y = 0; y < 16; ++y) {
        for (int z = 0; z < 16; ++z) {
            for (int x = 0; x < 16; ++x) {
                for (int i = 0; i < palette.length; ++i) {
                    if (chunkSection->states[y][z][x] == palette.map[i]) {
                        ba_append_byte(byteArray, i);               
                    }
                }
            }
        }
    }

    ba_append_byte(byteArray, 0);
    ba_append_varint(byteArray, 0);
    ba_append_varint(byteArray, 0);
    free(palette.map);
}

void Palette_add_state(Palette *palette, int state) {
    palette->length++;
    palette->map = realloc(palette->map, sizeof(int)*palette->length);
    palette->map[palette->length-1] = state;
}

Palette Palette_new() {
    Palette palette = {0};
    palette.bits_per_entry = 0;
    palette.length = 0;
    palette.map = malloc(sizeof(int));
    return palette;
}