#include <stdbool.h>

typedef struct ServerSettings {
    bool is_hardcore;
    int dimension_count;
    char *dimension_name;
    int max_players;
    int view_distance;
    int simulation_distance;
    bool reduced_debug_info;
    bool enable_respawn_screen;
    bool do_limited_crafting;
    long hashed_seed;
    bool enforces_secure_chat;
} ServerSettings;