typedef enum EntityType {
    PLAYER = 122;
}

typedef struct Entity {
    int x, y, z;
    int type;
} Entity;