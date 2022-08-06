#include "pch.h"

void argv_ip(char *argv, u_int8_t *dst)
{
    char *result;
    result = strtok(argv, ".");
    dst[0] = atoi(result);
    for (int i = 1; i <= 3; i++)
    {
        result = strtok(NULL, ".");
        dst[i] = atoi(result);
    }
}
void copy_ip(u_int8_t *src, u_int8_t *dst)
{
    for (int i = 0; i < 4; i++)
    {
        dst[i] = src[i];
    }
}
void print_ip(u_int8_t *ip)
{
    printf("ip addr :");
    for (int i = 0; i < 4; i++)
    {
        printf("%d", ip[i]);
        if (i != 3)
        {
            printf(".");
        }
    }
    printf("\n");
}
void copy_mac(u_int8_t *src, u_int8_t *dst)
{
    for (int i = 0; i < 6; i++)
    {
        dst[i] = src[i];
    }
}
void print_mac(u_int8_t *mac)
{
    printf("mac addr ");
    for (int i = 0; i < 6; i++)
    {
        printf("%02x", mac[i]);
        if (i != 5)
        {
            printf(":");
        }
    }
    printf("\n");
}

bool if_same_mac(u_int8_t *mac1, u_int8_t *mac2)
{
    for (int i = 0; i < 6; i++)
    {
        if (mac1[i] == mac2[i])
            ;
        else
        {
            return false;
        }
    }
    return true;
}

bool if_same_ip(u_int8_t *ip1, u_int8_t *ip2)
{
    for (int i = 0; i < 4; i++)
    {
        if (ip1[i] == ip2[i])
            ;
        else
        {
            return false;
        }
    }
    return true;
}

bool check_http(u_char *payload)
{
    if (payload[0] == 0x17)
    {
        return true;
    }
    else if (payload[0] == 0x47 && payload[1] == 0x45 && payload[2] == 0x54)
    {
        return true;
    }
    else if (payload[0] == 0x50 && payload[1] == 0x4f && payload[2] == 0x53 && payload[3] == 0x54)
    {
        return true;
    }
    else
    {

        return false;
    }
}

void copy_payload(char *src, char *dst)
{
    for (int i = 0; i < strlen(src); i++)
    {
        dst[i] = src[i];
    }
}