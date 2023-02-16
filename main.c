#include <stdio.h>
#include "Master.h"
#include "Input.h"
#include "OutputEntities.h"
#include "Slave.h"
#include "Client.h"
#include "Order.h"

#define Client_IND "client.ind"
#define INDEXER_SIZE sizeof(struct Indexer)
int main()
{
    struct Client client;
    struct Order order;
    while (1)
    {
        int choice;
        int id;
        char error[20];
        printf("Acceptable Options:\n0 Insert Client\n1 Get Client\n2 Update Client\n3 Delete Client\n4 Print List of Clients\n5 Insert Order\n6 Get Order\n7 Update Order\n8 Delete Order \n9 Output List of Orders\n10 Exit\n");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                inputClient(&client);
                insertClient(client);
                break;
            case 1:
                printf("Enter ID: \n");
                scanf("%d", &id);
                if(getClient(&client, id, error) != 0)
                {
                    OutputClient(client);
                }
                else
                {
                    printf("Error: %s\n", error);
                }
                break;
            case 2:
                printf("Enter ID: \n");
                scanf("%d", &id);
                inputClient(&client);
                if(updateClient(client, error, id)==0)
                    printf("%s", error);
                else
                    printf("Update was successfully\n");
                break;
            case 3:
                printf("Enter ID: \n");
                scanf("%d", &id);
                if(deleteClient(id, error) != 0)
                   printf("Deleted successfully\n");
                else
                    printf("Error: %s\n", error);
                break;
            case 4:
                PrintListOfClient(error);
                break;
            case 5:
                printf("Enter the client id\n");
                scanf("%d", &id);
                if(getClient(&client, id, error) != 0)
                {
                    InputOrder(&order);
                    insertOrder(client, order, error);
                    printf("Insert was successfully \n");
                }
                else
                {
                    printf("Error %s\n", error);
                }
                break;
            case 6:
                printf("Enter the client id \n");
                scanf("%d", &id);
                if(getClient(&client, id, error))
                {
                    printf("Enter the id of order \n");
                    scanf("%d", &id);
                    if(checkIfRecordExist(client, id,error))
                    {
                        getOrder(client, &order, id, error);
                        OutputOrder(order, client);
                    }
                    else
                    {
                        printf("Error %s\n", error);
                    }
                }
                else
                {
                    printf("Error %s\n", error);
                }
                  break;
            case 7:
                printf("Enter the client id \n");
                scanf("%d", &id);
                if(getClient(&client, id, error))
                {
                    printf("Enter the id of order\n ");
                    scanf("%d", &id);
                    if(checkIfRecordExist(client, id,error))
                    {
                        getOrder(client, &order, id, error);
                        InputOrder(&order);
                        updateOrder(order);
                        printf("Updating was successfully \n ");
                    }
                    else
                    {
                        printf("Error %s\n", error);
                    }
                }
                else
                {
                    printf("Error %s \n", error);
                }

                break;
            case 8:
                printf("Enter the client id \n");
                scanf("%d", &id);
                if(getClient(&client, id, error))
                {
                    printf("Enter the id of order \n");
                    scanf("%d", &id);
                    if(checkIfRecordExist(client, id,error))
                    {
                        deleteOrder(client,order, error);
                        printf("Deleted was successfully \n");
                    }
                    else
                    {
                        printf("Error %s \n ", error);
                    }
                }
                else
                {
                    printf("Error %s \n ", error);
                }
                break;
            case 9:
                printf("Enter the client id \n");
                scanf("%d", &id);
                if(getClient(&client, id, error))
                {
                   if(client.orderCount != 0)
                   {
                       PrintList(client);
                   }
                   else
                   {
                       printf("This client has no orders\n");
                   }
                }
                else
                {
                    printf("Error %s \n ", error);
                }
                break;
            case 10:
                return 0;
            default:
                printf("No command :( \n");
        }
        printf("\n");
    }
}
