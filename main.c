#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <json.h>
#include <json_object.h>

#include "main.h"
#include "iopcdefine.h"
#include "iopcops_cfg_json.h"

unsigned long parse_paramegers(int argc, char** argv, char* cfg_file, char* cfg_key)
{
    unsigned long func = 0x0;
    int cmd_opt = 0;
    while(1) {
        cmd_opt = getopt(argc, argv, "f:k:alvts");

        if(cmd_opt == -1) {
            return func;
        }

        switch(cmd_opt) {
        case 'k': // cfg key
            strcpy(cfg_key, optarg);
        break;
        case 'f': // cfg file
            strcpy(cfg_file, optarg);
        break;
        case 'a': // show all cfgs
            func |= SHOW_ALL;
        break;
        case 'l': // show array len
            func |= SHOW_ARRAY_LEN;
        break;
        case 's': // show array string
            func |= SHOW_ARRAY_STRING;
        break;
        case 'v': // show value
            func |= SHOW_VALUE;
        break;
        case 't': // show type
            func |= SHOW_TYPE;
        break;
        }
    }

    return func;
}

static void show_usage()
{
    printf("-f: input cfg file\n");
    printf("-k: cfg key string\n");
    printf("-a: show all elements of file\n");
    printf("-v: show value of element\n");
    printf("-t: show type of element\n");
    printf("-l: show length of array element\n");
    printf("-s: show array strings\n");
    printf("example : \n");
    printf("    utils -f /tmp/cfgs.db -a\n");
    printf("    utils -f /tmp/cfgs.db -k type -v\n");
}

int main(int argc, char** argv)
{
    //char* path;
    uint8_t err_code = 0;
    struct json_object *json_obj;
    char cfg_file[STR_LEN];
    unsigned long func = 0x0;
    //int array_len = 0;
    int isError = 0;
    uint8_t cfg_key[STR_LEN];
    uint8_t cfg_val[STR_LEN];
    struct cfg_json_t cfg_json;

    if(argc < 2) {
        show_usage();
        return 1;
    }

    func = parse_paramegers(argc, argv, &cfg_file[0], &cfg_key[0]);
    //json_obj = json_object_from_file(json_file);
    //query_json_obj.isFound = 0;
    GET_INSTANCE(ops_cfg_json)->open(cfg_file, &cfg_json);

    switch(func) {
    case SHOW_ALL:
        GET_INSTANCE(ops_cfg_json)->show_all(&cfg_json);
    break;
    case SHOW_TYPE:
        json_obj = GET_INSTANCE(ops_cfg_json)->get_raw(&cfg_json, cfg_key);
        switch(json_object_get_type(json_obj)){
        case json_type_null:
            printf("%s", "NULL");
        break;
        case json_type_boolean:
            printf("%s", "BOOLEAN");
        break;
        case json_type_double:
            printf("%s", "DOUBLE");
        break;
        case json_type_int:
            printf("%s", "INTEGER");
        break;
        case json_type_string:
            printf("%s", "STRING");
        break;
        case json_type_object:
            printf("%s", "OBJECT");
        break;
        case json_type_array:
            printf("%s", "ARRAY");
        break;
        default:
            isError = 1;
        break;
        }
    break;
    case SHOW_ARRAY_LEN:
        printf("%d", GET_INSTANCE(ops_cfg_json)->get_array_size(&err_code, &cfg_json, cfg_key));
#if 0
        get_json_obj_by_query_obj(&query_json_obj, json_obj);
        isError = 1;
        if(query_json_obj.type == json_type_array) {
            printf("%d", json_object_array_length(query_json_obj.obj));
            isError = 0;
        }
#endif
    break;
    case SHOW_ARRAY_STRING:
#if 0
        get_json_obj_by_query_obj(&query_json_obj, json_obj);
        isError = 1;
        if(query_json_obj.type == json_type_array) {
            isError = 0;
        }
#endif
    break;
    case SHOW_VALUE:
	json_obj = GET_INSTANCE(ops_cfg_json)->get_raw(&cfg_json, cfg_key);
        switch(json_object_get_type(json_obj)){
        case json_type_null:
            printf("%s", "NULL");
        break;
        case json_type_boolean:
            printf("%d", GET_INSTANCE(ops_cfg_json)->get_boolean(&err_code, &cfg_json, cfg_key));
        break;
        case json_type_double:
            printf("%f", GET_INSTANCE(ops_cfg_json)->get_double(&err_code, &cfg_json, cfg_key));
        break;
        case json_type_int:
            printf("%d", GET_INSTANCE(ops_cfg_json)->get_int(&err_code, &cfg_json, cfg_key));
        break;
        case json_type_string:
            GET_INSTANCE(ops_cfg_json)->get_string(&err_code, &cfg_json, cfg_key, &cfg_val[0]);
	    printf("%s", cfg_val);
        break;
        case json_type_object:
        case json_type_array:
        default:
            isError = 1;
        break;
        }
    break;
    }

    if(isError)
        printf("%s", ERROR_STRING);

    return 0;
}

