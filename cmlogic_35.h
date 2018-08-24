#ifndef __COMBINATORICS_H__
#define __COMBINATORICS_H__
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>


#define KEY_PARTS (10)
#define PERSON_KEY_PARTS (6)
#define USER_KEY_LEN (18)
#define USERS (5)
#define THRESHD (3)
#define KEY_SIZE (16)

unsigned char **key_depart(unsigned char *key);
unsigned char *key_join(unsigned char **shares);
char *arr_to_hex_str(unsigned char *arr, int arr_size);
unsigned char * hex_str_to_arr(const char *s);
#endif

