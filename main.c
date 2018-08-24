#include <stdio.h>
#include "cmlogic_35.h"

int main()
{
    int i = 0, j = 0, index = 0;
    int sindex[THRESHD] = {-1, -1, -1};
    unsigned char *kjoin[THRESHD], *ptr_join = NULL;
    unsigned char key[] = "123456798abcdefg";
    unsigned char **shares = NULL;
    unsigned char *hex = NULL;
    unsigned char *keyret = NULL;
    
    srand((unsigned) time(NULL));
    for (i = 0; i < strlen(key); i++)
    {
        key[i] = rand()%128;
    }
    printf("input key:%s\n", key);
    shares = key_depart(key);

    for (i=0; i<USERS; i++)
    {
        hex = arr_to_hex_str(shares[i], USER_KEY_LEN);
        printf("user %d:%s\n", i, hex);
        free(hex);
    }

    ptr_join = malloc(THRESHD * USER_KEY_LEN);
    for (i = 0; i < THRESHD; i++)
    {
        while (1)
        {
            index = rand() % USERS;
            if ((index != sindex[0])&&(index != sindex[1])&&(index != sindex[2]))
            {
                sindex[i] = index;
                break;
            }
        }
        printf("select user:%d data:", index);
        kjoin[i] = ptr_join + i * USER_KEY_LEN;
        memset(kjoin[i], '\0', USER_KEY_LEN);
        memcpy(kjoin[i], shares[index], USER_KEY_LEN);
        for (j = 0; j < USER_KEY_LEN; j++)
            printf("%02x", kjoin[i][j]);
        printf("\n");
    }

    keyret = key_join(kjoin);
    if (NULL != keyret)
    {
        printf("key:%s\n", keyret);
        free(keyret);
        keyret = NULL;
    }
    /*由于内存是连续开辟的，因此只需做0位置的释放*/
    if (NULL != ptr_join)
    {
        free(ptr_join);
        ptr_join = NULL;
    }
    
    if (NULL != shares[0])
    {
        free(shares[0]);
        shares[0] = NULL;
    }
    if (NULL != shares)
    {
        free(shares);
        shares = NULL;
    }
}

