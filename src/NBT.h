#include "ByteArray.h"

typedef enum NBTTagType {
    NBT_END,
    NBT_BYTE,
    NBT_SHORT,
    NBT_INT,
    NBT_LONG,
    NBT_FLOAT,
    NBT_DOUBLE,
    NBT_BYTEARRAY,
    NBT_STRING,
    NBT_LIST,
    NBT_COMPOUND,
    NBT_INT_ARRAY,
    NBT_LONG_ARRAY
} NBTTagType;

/*
typedef struct NBTEntry {
    NBTTagType type;
    char name[64];
    int length;
    union {
        char byte_value;
        short short_value;
        int int_value;
        long long long_value;
        float float_value;
        double double_value;
        char *bytearray;
        char *string;
        NBTEntry list;
        NBTEntry compound;
        int *int_array;
        long long *long_array;
    } test; 
} NBTEntry;
*/

void NBT_ByteArray_add_byte(ByteArray *nbt, Byte value);
void NBT_ByteArray_add_short(ByteArray *nbt, short value);
void NBT_ByteArray_add_int(ByteArray *nbt, int value);
void NBT_ByteArray_add_long(ByteArray *nbt, long long value);
void NBT_ByteArray_add_float(ByteArray *nbt, float value);
void NBT_ByteArray_add_double(ByteArray *nbt, double value);
void NBT_ByteArray_add_byte_array(ByteArray *nbt, char *byte_array, int length);
void NBT_ByteArray_add_string(ByteArray *nbt, char *value, int length);
void NBT_ByteArray_add_list(ByteArray *nbt, short value);
void NBT_ByteArray_add_compound(ByteArray *nbt, short value);
void NBT_ByteArray_add_int_array(ByteArray *nbt, short value);
void NBT_ByteArray_add_long_array(ByteArray *nbt, short value);