#pragma once
#include "Master.h"
#define INDEXER_SIZE sizeof(struct Indexer)
#define Client_DATA "client.fl"
#define Client_SIZE sizeof(struct Client)
#define ORDER_DATA "order.fl"
#define ORDER_SIZE sizeof(struct Order)
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

