#pragma once
#include "Master.h"

void OutputClient(struct Client client)
{
    printf("Client's first name: %s\n", client.firstName);
    printf("Client's last name: %s\n", client.lastName);
    printf("Client's birthday: %s\n", client.birthday);
    printf("Client's mobile number: %s\n", client.mobileNumber);
    printf("Client's password: %s\n", client.password);
    printf("Client's sex: %s\n", client.gender);
}

void OutputOrder(struct Order order, struct Client client)
{
    printf("The client id %d \n", client.id);
    printf("Client info first name and last name: %s, %s \n", client.firstName, client.lastName);
    printf("Order count of product: %d\n", order.countOfProduct);
    printf("Order complete: %d\n", order.complete);
    printf("Order recovery time: %s\n", order.date);
}
