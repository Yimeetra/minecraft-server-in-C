#include "cJSON/cJSON.h"
#include <stdio.h>
#include "Packet.h"
#include <malloc.h>
#include <assert.h>
#include <string.h>

int main() {
    char *string = "{\"qwerty\": {\"test1\": 12345,\"test2\": 67.89}}";

    cJSON *json = cJSON_Parse(string);

    printf("%f, %f", json->child->child->valueint, json->child->child->valuedouble);
    printf("%f, %f", json->child->child->next->valueint, json->child->child->next->valuedouble);

}