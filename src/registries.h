#include <stdbool.h>

typedef enum RegistryType {
    PAINTING_VARIANT,
    WOLF_VARIANT,
    WORLDGEN_BIOME,
    REGISTRY_TYPE_TOP
} RegistryType;

typedef struct PaintingVariant {
    char name[256];
    struct {
        char asset_id[256];
        int height;
        int width;
    } payload;
} PaintingVariant;

typedef struct WolfVariant {
    char name[256];
    struct {
        char angry_texture[256];
        char biomes[256];
        char tame_texture[256];
        char wild_texture[256];
    } payload;
} WolfVariant;

typedef struct WorldgenBiome {
    char name[256];
    struct {
        float downfall;
        struct {
            int fog_color;
            int foliage_color;
            int grass_color;
            struct {
                int block_search_extent;
                float offset;
                char sound[256];
                int tick_delay;
            } mood_sound;
            struct {
                int max_delay;
                int min_delay;
                bool replace_current_music;
                char sound[256];
            } music;
            int sky_color;
            int water_color;
            int water_fog_color;
        } effects;
        bool has_precipitation;
        float temperature;
    } payload;
} WorldgenBiome;

typedef struct Registry {
    RegistryType type;
    union {
        PaintingVariant paintingVariant;
        WolfVariant wolfVariant;
        WorldgenBiome worldgenBiome;
    } registry;
} Registry;

typedef struct Registries {
    int length;
    int count;
    Registry *registries;
} Registries;

extern Registries registries;

void init_registries();
void Registries_append();
int Registries_get_registry_count(RegistryType type);