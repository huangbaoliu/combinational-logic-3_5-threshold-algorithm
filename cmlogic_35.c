#include "cmlogic_35.h"


void index_cal(unsigned char person[USERS][PERSON_KEY_PARTS])
{
    int i = 0, j = 0;
    int r0= 0, r1 = 0, r2;
    int flag = 0;
    int person_index[USERS] = {0, 0, 0, 0, 0};

    while(1)
    {
        flag = 0;
        for (i=0; i<USERS; i++)
        {
            person_index[i] = 0;
        }
        for (i=0; i<KEY_PARTS; i++)
        {
            r0 = rand()%USERS;
            person[r0][person_index[r0]] = i;
            person_index[r0]++;

            while (1)
            {
                r1 = rand()%USERS;
                if (r1 != r0)
                    break;
            }
            person[r1][person_index[r1]] = i;
            person_index[r1]++;

            while (1)
            {
                r2 = rand()%USERS;
                if ((r2 != r0) && (r2 != r1))
                    break;
            }
            person[r2][person_index[r2]] = i;
            person_index[r2]++;
        }

        for (i=0; i<USERS; i++)
        {
            if (PERSON_KEY_PARTS != person_index[i]) 
            {
                flag = 1;
            }
        }

/*flag 为0分配成功*/
        if (0 == flag)
        {
#if 1
            for (i=0; i<USERS; i++)
            {
                printf("person %d:\t", i);
                for (j=0; j<6; j++)
                {
                    printf("%d\t", person[i][j]);
                }
                printf("\n");
            }
#endif
            printf("depart ok\n");
            return;
        }
        //printf("reconstruct\n");
    }

    return ;
}

unsigned char miner_data(unsigned char data1, unsigned char data2)
{
    unsigned char miner = data1;
    if (miner > data2)
        miner = data2;
    return miner;
}

unsigned char **key_depart(unsigned char *key)
{
    int ret = 0;
    int i = 0, j = 0;
    unsigned char index = 0;
    unsigned char person[USERS][PERSON_KEY_PARTS];
    unsigned char *ptr = NULL, *data_ptr = NULL;
    unsigned char **shares = malloc(USERS * sizeof(unsigned char *));

    memset(person, 0, sizeof(person));
    index_cal(person);

    data_ptr = (unsigned char *)malloc(USER_KEY_LEN * USERS);
    for (i=0; i<USERS; i++)
    {
        shares[i] = data_ptr + i*USER_KEY_LEN;//malloc(18 * sizeof(unsigned char));
        ptr = shares[i];
        for (j=0; j<PERSON_KEY_PARTS; j++)
        {
            index = person[i][j];
            *(ptr + j*3) = index;
            if (index < PERSON_KEY_PARTS)
            {
                memcpy(ptr + (j*3+1), key + 2*index, 2);
            }
            else
            {   
                memcpy(ptr + (j*3+1), key + (2*6+(index-6)), 1);
            }
        }
    }
    return shares;
}

unsigned char *key_join(unsigned char **shares_ptr)
{
    int i = 0;
    int j = 0;
    unsigned char miner;
    unsigned char *key = NULL;;
    unsigned char indexs[THRESHD] = {0, 0, 0};
    unsigned int len = 0;

    key = (unsigned char *)malloc(KEY_SIZE);
    if (NULL == key)
    {
        printf("malloc memory for key error\n");
        return NULL;
    }
    memset(key, '\0', KEY_SIZE);

    while(len < KEY_SIZE)
    {
        if (indexs[0] >= USER_KEY_LEN )
            miner = miner_data(*shares_ptr[2], *shares_ptr[1]);
        else if (indexs[1] >= USER_KEY_LEN )
            miner = miner_data(*shares_ptr[2], *shares_ptr[0]);
        else if (indexs[2] >= USER_KEY_LEN )
            miner = miner_data(*shares_ptr[0], *shares_ptr[1]);
        else
            miner = miner_data(miner_data(*shares_ptr[0], *shares_ptr[1]), *shares_ptr[2]);
        
        printf("len :%d, miner:%d\n", len, miner);
        for (i = 0; i < THRESHD; i++)
        {
            if (*shares_ptr[i] == miner)
            {
                if (miner < PERSON_KEY_PARTS)
                {
                    memcpy(key + len, shares_ptr[i]+1, 2);
                    len += 2;
                }
                else
                {
                    memcpy(key + len, shares_ptr[i]+1, 1);
                    len += 1;
                }
                indexs[i] += 3;
                if (indexs[i] < USER_KEY_LEN)
                {
                    shares_ptr[i] += 3;
                }
                for (j = 0; j < 3; j++)
                {
                    if (j == i)
                        continue;
                    while (*shares_ptr[j] <= miner)
                    {
                        indexs[j] += 3;
                        if (indexs[j] < USER_KEY_LEN)
                        {
                            shares_ptr[j] += 3;
                        }
                    }
                }
                break;
            }
        }
    }

    return key;
}

char *arr_to_hex_str(unsigned char *arr, int arr_size) 
{
    char *out = malloc(2 * arr_size);
    for (int pos = 0; pos < arr_size; pos++) 
    {
        sprintf(out + 2*pos, "%02x", arr[pos]);
    }
    //out[2 * arr_size + 1] = '\0';
    return out;
}

unsigned char * hex_str_to_arr(const char *s) 
{
    // / 2 ?
    unsigned char *res = malloc(strlen(s) * sizeof(unsigned char));
    char buff[3] = {0x00, 0x00, 0x00};
    for (int pos = 0; pos < strlen(s); pos++) 
    {
        strncpy(buff, s + pos*2, 2);
        res[pos] = strtoul(buff, NULL, 16);
    }
    return res;
}

