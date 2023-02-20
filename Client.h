#pragma once
#include <malloc.h>
#include "Master.h"
#include "Input.h"
#include "Check.h"
#include "OutputEntities.h"
#include "Slave.h"
#include "Order.h"

#define Client_IND "client.ind"
#define Client_DATA "client.fl"
#define Client_GARBAGE "client_garbage.txt"
#define INDEXER_SIZE sizeof(struct Indexer)
#define Client_SIZE sizeof(struct Client)
#define ORDER_SIZE sizeof(struct Order)
#define ORDER_DATA "order.fl"
int PrintListOfClient(char *error);
void overwriteGarbageClientElement(int garbageCount, FILE* garbageZone, struct Client* client)
        {
    int* deleteIdsFile = malloc(garbageCount * sizeof(int));
    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbageZone, "%d", deleteIdsFile + i);
    }
    fclose(garbageZone);
    client->id = deleteIdsFile[0];
    fopen(Client_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount - 1);
    for (int i = 1; i < garbageCount; i++)
    {
        fprintf(garbageZone, " %d", deleteIdsFile[i]);
    }
    fclose(garbageZone);
    free(deleteIdsFile);
}
int insertClient(struct Client client){
    FILE* garbageZone = fopen(Client_GARBAGE, "a+b");
    FILE* indexTable = fopen(Client_IND, "a+b");
    FILE* database = fopen(Client_DATA, "a+b");
    int garbageCount = 0;
    fscanf(garbageZone, "%d", &garbageCount);
    struct Indexer indexer;
    if (garbageCount !=0 )
    {
        overwriteGarbageClientElement(garbageCount, garbageZone, &client);
        fclose(database);
        fclose(indexTable);
        indexTable = fopen(Client_IND, "r+b");
        database = fopen(Client_DATA, "r+b");
        fseek(indexTable, (client.id - 1) * INDEXER_SIZE, SEEK_SET);
        fread(&indexer, INDEXER_SIZE, 1, indexTable);
        fseek(database, indexer.address, SEEK_SET);
    }
    else {
        fseek(indexTable, 0, SEEK_END);
        if (ftell(indexTable) != 0) {
            fseek(indexTable, -(INDEXER_SIZE), SEEK_END);
            fread(&indexer, INDEXER_SIZE, 1, indexTable);
            client.id = indexer.id + 1;
        } else {
            client.id = 1;
        }
    }
    client.orderFirstAddress = -1;
    client.orderCount = 0;
    fwrite(&client, Client_SIZE, 1, database);
    indexer.id = client.id;
    indexer.exists = 1;
    indexer.address = (client.id - 1) * Client_SIZE;
    printf("Your client id is %d\n", client.id);
    fseek(indexTable, (client.id - 1) * INDEXER_SIZE, SEEK_SET);
    fwrite(&indexer, INDEXER_SIZE, 1, indexTable);
    fclose(database);
    fclose(indexTable);
    return 1;
}
int getClient(struct Client* client, int id, char* error)
        {
    FILE* indexTable = fopen(Client_IND, "rb");
    FILE* database = fopen(Client_DATA, "rb");
    if(checkFileExists(indexTable, database, error) == 0)
        return 0;
    struct Indexer indexer;
    if(checkIndexExists(indexTable, error, id) ==0)
        return 0;
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if (checkRecordExists(indexer, error)==0)
        return 0;
    fseek(database, indexer.address, SEEK_SET);
    fread(client, Client_SIZE, 1, database);
    fclose(indexTable);
    fclose(database);
    return 1;
}

int updateClient(struct Client client, char* error, int id) {
    FILE* indexTable = fopen(Client_IND, "r+b");
    FILE* database = fopen(Client_DATA, "r+b");
    struct Indexer indexer;
    if (checkFileExists(indexTable, database, error)==0) {
        return 0;
    }
    if (checkIndexExists(indexTable, error, id)==0) {
        return 0;
    }
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if(checkRecordExists(indexer, error)==0)
        return 0;
    client.id = id;
    fseek(database, indexer.address, SEEK_SET);
    fwrite(&client, Client_SIZE, 1, database);
    fclose(indexTable);
    fclose(database);
    return 1;
}
void listOfDeletedClient(int id)
{
    FILE* garbage = fopen(Client_GARBAGE, "rb");
    if (garbage == NULL)
    {
        printf("Error occured while opening file \n");
        return;
    }
    int garbageCount;
    fscanf(garbage, "%d", &garbageCount);
    int* deleteIds = malloc(garbageCount * sizeof(int));

    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbage, "%d", deleteIds + i);
    }
    fclose(garbage);
    garbage = fopen(Client_GARBAGE, "wb");
    fprintf(garbage, "%d", garbageCount + 1);
    for (int i = 0; i < garbageCount; i++) {
        fprintf(garbage, " %d", deleteIds[i]);
    }
    fprintf(garbage, " %d", id);
    free(deleteIds);
    fclose(garbage);
}
int deleteClient(int id, char* error) {
    FILE* indexTable = fopen(Client_IND, "r+b");
    struct Indexer indexer;
    if (indexTable == NULL) {
        strcpy(error, "database files are not created yet");
        return 0;
    }
    if (checkIndexExists(indexTable, error, id)==0) {
        return 0;
    }
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if(checkRecordExists(indexer, error)==0)
        return 0;
    struct Client client;
    getClient(&client, id, error);
    indexer.exists = 0;
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fwrite(&indexer, INDEXER_SIZE, 1, indexTable);
    fclose(indexTable);
    listOfDeletedClient(id);
    if (client.orderCount != 0) {
        FILE* orderDb = fopen(ORDER_DATA, "r+b");
        struct Order order;
        fseek(orderDb, client.orderFirstAddress, SEEK_SET);
        for (int i = 0; i < client.orderCount; i++)
        {
            fread(&order, ORDER_SIZE, 1, orderDb);
            fclose(orderDb);
            deleteOrder(client, order, error);
            orderDb = fopen(ORDER_DATA, "r+b");
            fseek(orderDb, order.nextAddress, SEEK_SET);
        }
        fclose(orderDb);
    }
    return 1;
}
 int PrintListOfClient(char *error)
 {
     FILE* database = fopen(Client_DATA, "rb");
     FILE* indexTable = fopen(Client_IND, "rb");
     struct Client client;
     struct Indexer indexer;
     if (checkFileExists(indexTable, database, error)==0)
     {
         return 0;
     }
     fseek(indexTable, -INDEXER_SIZE, SEEK_END);
     fread(&indexer, INDEXER_SIZE, 1, indexTable);
     rewind(indexTable);
     int endIndex =0;
     getClient(&client, indexer.id, error);
     endIndex = indexer.id;
     int id = 0;
     int count =0;
     while(id < endIndex)
     {
         fseek(indexTable, id *INDEXER_SIZE, SEEK_SET);
         fread(&indexer, INDEXER_SIZE, 1, indexTable);
         if(indexer.exists)
         {
             printf("<=============================>\n");
             getClient(&client, indexer.id, error);
             printf("Id %d\n", client.id);
             OutputClient(client);
             count++;
         }
         id++;
     }
     if(count == 0)
     {
         printf("No clients\n");
     }
     return 1;
 }


