#include "registries.h"
#include "cJSON/cJSON.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

Registries registries;

void init_registries() {
    printf("init_registries()\n");
    char *buffer;
    long length;
    cJSON *registries_json;
    FILE *f = fopen("registries.json", "r");

    if (!f) {
        printf("registries.json not found\n");
        exit(-1);
    }
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc(length);

    if (!buffer) {
        printf("failed to allocate memory for buffer\n");
        exit(-1);
    }
    fread(buffer, 1, length, f);
    registries_json = cJSON_Parse(buffer);
    free(buffer);
    fclose (f);

    printf("started parsing\n");
    cJSON *registry_json;
    cJSON_ArrayForEach(registry_json, registries_json) {
        if (strcmp(registry_json->string, "minecraft:painting_variant") == 0) {
            cJSON *entry;
            cJSON_ArrayForEach(entry, registry_json) {
                Registry registry;
                PaintingVariant paintingVariant;
                strcpy(paintingVariant.name, entry->string);

                strcpy(paintingVariant.payload.asset_id, cJSON_GetObjectItem(entry, "asset_id")->valuestring);
                paintingVariant.payload.height = cJSON_GetObjectItem(entry, "height")->valuedouble;
                paintingVariant.payload.width = cJSON_GetObjectItem(entry, "width")->valuedouble;

                registry.type = PAINTING_VARIANT;
                registry.registry.paintingVariant = paintingVariant;
                Registries_append(registry);
            }
        } else if (strcmp(registry_json->string, "minecraft:wolf_variant") == 0) {
            cJSON *entry;
            cJSON_ArrayForEach(entry, registry_json) {
                Registry registry;
                WolfVariant wolfVariant;
                strcpy(wolfVariant.name, entry->string);

                strcpy(wolfVariant.payload.angry_texture, cJSON_GetObjectItem(entry, "angry_texture")->valuestring);
                strcpy(wolfVariant.payload.biomes, cJSON_GetObjectItem(entry, "biomes")->valuestring);
                strcpy(wolfVariant.payload.tame_texture, cJSON_GetObjectItem(entry, "tame_texture")->valuestring);
                strcpy(wolfVariant.payload.wild_texture, cJSON_GetObjectItem(entry, "wild_texture")->valuestring);
                
                registry.type = WOLF_VARIANT;
                registry.registry.wolfVariant = wolfVariant;
                Registries_append(registry);
            }
        } else if (strcmp(registry_json->string, "minecraft:worldgen/biome") == 0) {
            cJSON *entry;
            cJSON_ArrayForEach(entry, registry_json) {
                Registry registry;
                WorldgenBiome worldgenBiome;
                strcpy(worldgenBiome.name, entry->string);
                /*----------------------------------------------------------------------TODO------------------------------------------------------------------------------------------------------------------\
                strcpy(worldgenBiome.payload.effects.mood_sound.sound, cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(entry, "effects"), "mood_sound"), "sound")->valuestring);
                strcpy(worldgenBiome.payload.effects.music.sound, cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(entry, "effects"), "music"), "sound")->valuestring);
                worldgenBiome.payload.downfall                               = cJSON_GetObjectItem(entry, "downfall")->valuedouble;
                worldgenBiome.payload.has_precipitation                      = cJSON_GetObjectItem(entry, "has_precipitation")->type-1;
                worldgenBiome.payload.temperature                            = cJSON_GetObjectItem(entry, "temperature")->valuedouble;
                worldgenBiome.payload.effects.fog_color                      = cJSON_GetObjectItem(cJSON_GetObjectItem(entry, "effects"), "fog_color")->valuedouble;
                worldgenBiome.payload.effects.foliage_color                  = cJSON_GetObjectItem(cJSON_GetObjectItem(entry, "effects"), "foliage_color")->valuedouble;
                worldgenBiome.payload.effects.grass_color                    = cJSON_GetObjectItem(cJSON_GetObjectItem(entry, "effects"), "grass_color")->valuedouble;
                worldgenBiome.payload.effects.sky_color                      = cJSON_GetObjectItem(cJSON_GetObjectItem(entry, "effects"), "sky_color")->valuedouble;
                worldgenBiome.payload.effects.water_color                    = cJSON_GetObjectItem(cJSON_GetObjectItem(entry, "effects"), "water_color")->valuedouble;
                worldgenBiome.payload.effects.water_fog_color                = cJSON_GetObjectItem(cJSON_GetObjectItem(entry, "effects"), "water_fog_color")->valuedouble;
                worldgenBiome.payload.effects.mood_sound.block_search_extent = cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(entry, "effects"), "mood_sound"), "search_extent")->valuedouble;
                worldgenBiome.payload.effects.mood_sound.offset              = cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(entry, "effects"), "mood_sound"), "offset")->valuedouble;
                worldgenBiome.payload.effects.mood_sound.tick_delay          = cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(entry, "effects"), "mood_sound"), "tick_delay")->valuedouble;
                worldgenBiome.payload.effects.music.max_delay                = cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(entry, "effects"), "music"), "max_delay")->valuedouble;
                worldgenBiome.payload.effects.music.min_delay                = cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(entry, "effects"), "music"), "min_delay")->valuedouble;
                worldgenBiome.payload.effects.music.replace_current_music    = cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(entry, "effects"), "music"), "replace_current_music")->type-1;
                \--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

                
                registry.type = WORLDGEN_BIOME;
                registry.registry.worldgenBiome = worldgenBiome;
                Registries_append(registry);
            }
        } else {
            printf("registry %s is unexpected\n", registry_json->string);
            //exit(-1);
        }
    }
    printf("parse all registries\n");
}

void Registries_append(Registry registry) {
    if (registries.count == registries.length) {
        registries.length++;
        registries.registries = realloc(registries.registries, registries.length*sizeof(Registry));
    }
    registries.registries[registries.length-1] = registry;
    registries.count++;
}

int Registries_get_registry_count(RegistryType type) {
    int count = 0;
    for (int i = 0; i < registries.count; ++i) {
        if (registries.registries[i].type == type) count++;
    }
    return count;
}