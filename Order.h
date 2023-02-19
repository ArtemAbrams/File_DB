#pragma once
#include "Master.h"
#include "Slave.h"
#include "OutputEntities.h"
#define ORDER_DATA "order.fl"
#define ORDER_GARBAGE "order_garbage.txt"
#define ORDER_SIZE sizeof(struct Order)
int updateClient(struct Client client, char* error, int id);
struct Order FindLastAddress(FILE *database, struct Client *client, struct Order *previous)
        {
    for (int i = 0; i < client->orderCount; i++) {
        fread(previous, ORDER_SIZE, 1, database);
        fseek(database, (*previous).nextAddress, SEEK_SET);
    }
    return (*previous);
}
void NextAddress(FILE* database, struct Client *client, struct Order *order)
        {
            fclose(database);
            database = fopen(ORDER_DATA, "r+b");
    struct Order previous;
    fseek(database, client->orderFirstAddress, SEEK_SET);
    previous = FindLastAddress(database, client, &previous);
    previous.nextAddress = order->selfAddress;
    fwrite(&previous, ORDER_SIZE, 1, database);
}
void overwriteGarbageAddress(int garbageCount, FILE* garbageZone, struct Order* record) {
    long * deletedIds = malloc(garbageCount * sizeof(long));
    for (int i = 0; i < garbageCount; i++) {
        fscanf(garbageZone, "%ld", deletedIds + i);
    }

    record->selfAddress = deletedIds[0];
    record->nextAddress = deletedIds[0];

    fclose(garbageZone);
    fopen(ORDER_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount - 1);

    for (int i = 1; i < garbageCount; i++) {
        fprintf(garbageZone, " %ld", deletedIds[i]);
    }

    free(deletedIds);
    fclose(garbageZone);
}
int insertOrder(struct Client client, struct Order order, char * error)
{
    FILE* database = fopen(ORDER_DATA, "a+b");
    FILE* garbageZone = fopen(ORDER_GARBAGE, "a+b");
    int garbageCount = 0;
    fscanf(garbageZone, "%d", &garbageCount);
    order.exists=1;
    struct Order order1;
    fseek(database, 0, SEEK_END);
    if (garbageCount)
    {
        overwriteGarbageAddress(garbageCount, garbageZone, &order);
        fclose(database);
        database = fopen(ORDER_DATA, "r+b");
        fseek(database, order.selfAddress, SEEK_SET);
    }
    else
    {
        if (ftell(database) != 0) {
            fseek(database, -(ORDER_SIZE), SEEK_END);
            fread(&order1, ORDER_SIZE, 1, database);
            order.Id = order1.Id + 1;
        }
        else
        {
            order.Id = 1;
        }
        int dbSize = ftell(database);
        order.selfAddress = dbSize;
        order.nextAddress = dbSize;
    }
    fseek(database, 0, SEEK_END);
    printf("Your order id is %d \n", order.Id);
    fwrite(&order, ORDER_SIZE, 1, database);
    if (!client.orderCount)
    {
        client.orderFirstAddress = order.selfAddress;
    }
    else
    {
        NextAddress(database, &client, &order);
    }
    fclose(database);
    int count = client.orderCount +1;
    client.orderCount=count;
    updateClient(client, error, client.id);
    return 1;
}
int getOrder(struct Client client, struct Order* order, int orderId, char* error) {

    FILE* database = fopen(ORDER_DATA, "rb");
    fseek(database, client.orderFirstAddress, SEEK_SET);
    fread(order, ORDER_SIZE, 1, database);

    for (int i = 0; i < client.orderCount; i++) {
        if ( order->Id== orderId)
        {
            fclose(database);
            return 1;
        }
        fseek(database, order->nextAddress, SEEK_SET);
        fread(order, ORDER_SIZE, 1, database);
    }
    fclose(database);
    return 0;
}
int updateOrder(struct Order order)
        {
    FILE* database = fopen(ORDER_DATA, "r+b");
    fseek(database, order.selfAddress, SEEK_SET);
    fwrite(&order, ORDER_SIZE, 1, database);
    fclose(database);
    return 1;
}
void noteDeletedOrder(long address) {
    FILE* garbageZone = fopen(ORDER_GARBAGE, "rb");
    int garbageCount;
    fscanf(garbageZone, "%d", &garbageCount);
    long* deletedAddresses = malloc(garbageCount * sizeof(long));

    for (int i = 0; i < garbageCount; i++) {
        fscanf(garbageZone, "%ld", deletedAddresses + i);
    }

    fclose(garbageZone);
    garbageZone = fopen(ORDER_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount + 1);

    for (int i = 0; i < garbageCount; i++) {
        fprintf(garbageZone, " %ld", deletedAddresses[i]);
    }

    fprintf(garbageZone, " %ld", address);
    free(deletedAddresses);
    fclose(garbageZone);
}
void relinkAddresses(FILE* database, struct Order previous, struct Order order, struct Client* client) {
    if (order.selfAddress == client->orderFirstAddress) {
        if (order.selfAddress == order.nextAddress) {
            client->orderFirstAddress = -1;
        } else {
            client->orderFirstAddress = order.nextAddress;
        }
    } else {
        if (order.selfAddress == order.nextAddress) {
            previous.nextAddress = previous.selfAddress;
        }
        else {
            previous.nextAddress = order.nextAddress;
        }
        fseek(database, previous.selfAddress, SEEK_SET);
        fwrite(&previous, ORDER_SIZE, 1, database);
    }
}

void deleteOrder(struct Client client, struct Order order, char* error)
        {
    FILE* database = fopen(ORDER_DATA, "r+b");
    struct Order previous;
    fseek(database, client.orderFirstAddress, SEEK_SET);
    do {
        fread(&previous, ORDER_SIZE, 1, database);
        fseek(database, previous.nextAddress, SEEK_SET);
    }
    while (previous.nextAddress != order.selfAddress && order.selfAddress != client.orderFirstAddress);
    relinkAddresses(database, previous, order, &client);
    noteDeletedOrder(order.selfAddress);
    order.exists = 0;

    fseek(database, order.selfAddress, SEEK_SET);
    fwrite(&order, ORDER_SIZE, 1, database);
    fclose(database);

    client.orderCount--;
    updateClient(client, error, client.id);
}
void PrintList(struct Client client)
{
    FILE* ordersDb = fopen(ORDER_DATA, "a+b");
    struct Order order;
    fseek(ordersDb, client.orderFirstAddress, SEEK_SET);
    for (int i = 0; i < client.orderCount; i++) {
        fread(&order, ORDER_SIZE, 1, ordersDb);
        if (order.exists)
        {
            OutputOrder(order, client);
            printf("<====================================>\n");
        }
        fseek(ordersDb, order.nextAddress, SEEK_SET);
    }
    fclose(ordersDb);
}