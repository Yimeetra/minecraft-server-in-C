#include "Client.h"
#include "Chunk.h"

typedef struct World {
    Client *clients;
    int clients_count;
    Chunk *Chunks;
    int chunk_count;
    char *dimension;
} World;

void World_add_client(World world, Client client);
void World_remove_client(World world, Client client);
void World_tick(World world);