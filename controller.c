#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <pthread.h>

static int pw_found = 0;
static char salt[13], hash[50], correct_password[25];
static pthread_mutex_t lock;

void logo()
{
    printf("                     _             \n");
    printf("                    | |            \n");
    printf("  ___ _ __ __ _  ___| | _____ _ __ \n");
    printf(" / __| '__/ _` |/ __| |/ / _ \\ '__|\n");
    printf("| (__| | | (_| | (__|   <  __/ |   \n");
    printf(" \\___|_|  \\__,_|\\___|_|\\_\\___|_|   \n");
    printf("\n");
}


int found_password()
{return pw_found;}

void print_answer()
{
    printf((found_password()) ? "Found: Password is %s\n" : "Did not find the correct answer\n", correct_password );
}

void split_hash_and_salt(char* arg)
{
    strncpy(hash, arg, 50);
    strncpy(salt, hash, 12);
}

void check(char* password)
{
    struct crypt_data data;
    data.initialized = 0;
    //printf("%s\n", password);
    char* encrypt = crypt_r(password, salt, &data);

    if (strcmp(hash, encrypt) == 0){
        pthread_mutex_lock(&lock);
        strncpy(correct_password, password, 40);
        pw_found = 1;
        pthread_mutex_unlock(&lock);
    }
}