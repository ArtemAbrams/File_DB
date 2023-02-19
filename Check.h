#pragma once


#include "Master.h"
#include "Client.h"
#include "Check.h"

#define INDEXER_SIZE sizeof(struct Indexer)
#define Client_DATA "client.fl"
#define Client_SIZE sizeof(struct Client)
#define ORDER_DATA "order.fl"
#define ORDER_SIZE sizeof(struct Order)
#define Client_IND "client.ind"

int getClient(struct Client* client, int id, char* error);
int checkFileExists(FILE* indexTable, FILE* database, char* error)
{
    if (indexTable == NULL || database == NULL) {
        strcpy(error, "DB file don`t exits");
        return 0;
    }
    return 1;
}
int checkIndexExists(FILE* indexTable, char* error, int id) {
    fseek(indexTable, 0, SEEK_END);
    long indexTableSize = ftell(indexTable);
    if (indexTableSize == 0 || id * INDEXER_SIZE > indexTableSize) {
        strcpy(error, "no this ID in the table");
        return 0;
    }
    return 1;
}
int checkRecordExists(struct Indexer indexer, char* error) {
    if (indexer.exists == 0 ) {
        strcpy(error, "the record was deleted");
        return 0;
    }
    return 1;
}
int checkIfRecordExist(struct Client client, int orderId, char *error ) {
    FILE* ordersDb = fopen(ORDER_DATA, "a+b");
    struct Order order;
    fseek(ordersDb, client.orderFirstAddress, SEEK_SET);
    for (int i = 0; i < client.orderCount; i++) {
        fread(&order, ORDER_SIZE, 1, ordersDb);
        fclose(ordersDb);
        if (order.Id == orderId)
        {
            if(order.exists)
                return 1;
            else
            {
                strcpy(error, "Order was deleted");
                return 0;
            }
        }
        ordersDb = fopen(ORDER_DATA, "r+b");
        fseek(ordersDb, order.nextAddress, SEEK_SET);
    }
    strcpy(error, "The client do not have this order, the order has been deleted or does not exist");
    fclose(ordersDb);
    return 0;
}
void info() {
    FILE* indexTable = fopen(Client_IND, "rb");
    if (indexTable == NULL) {
        printf("Error: database files do not exist\n");
        return;
    }
    int clientCount = 0;
    int orderCount = 0;
    fseek(indexTable, 0, SEEK_END);
    int indAmount = ftell(indexTable) / sizeof(struct Indexer);
    struct Client client;
    char error[51];
    for (int i = 1; i <= indAmount; i++)
    {
        if (getClient(&client, i, error))
        {
            clientCount++;
            orderCount += client.orderCount;
            printf("Train #%d has %d crews\n", i, client.orderCount);
        }
    }
    fclose(indexTable);
    printf("Total trains: %d\n", clientCount);
    printf("Total crews: %d\n", orderCount);
}




