#pragma once
#include "time.h"
struct Client
{
    int id;
    char firstName[10];
    char lastName[10];
    char mobileNumber[20];
    char gender[5];
    char password[20];
    char birthday[10];
    int orderCount;
    long orderFirstAddress;
};
struct Indexer
{
    int id;
    int address;
    int exists;
};
