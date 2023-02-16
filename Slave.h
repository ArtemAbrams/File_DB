#pragma once
struct Order
{
    int Id;
    int ClientId;
    int countOfProduct;
    int complete;
    char date[10];
    int exists;
    long selfAddress;
    long nextAddress;
};
