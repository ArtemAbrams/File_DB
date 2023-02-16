#pragma once
#include <stdio.h>
#include <string.h>
#include "Master.h"
#include "time.h"
#include "Slave.h"
void inputClient(struct Client *client)
{
    char firstName[10];
    char lastName[10];
    char mobileNumber[20];
    char gender[5];
    char birthday[10];
    char password[20];

    printf("Enter client's first Name: ");
    scanf("%s", firstName);
    strcpy(client->firstName, firstName);

    printf("Enter client's last Name: ");
    scanf("%s", lastName);
    strcpy(client->lastName, lastName);

    printf("Enter client's mobile Number: ");
    scanf("%s", mobileNumber);
    strcpy(client->mobileNumber, mobileNumber);

    printf("Enter client's gender: ");
    scanf("%s", gender);
    strcpy(client->gender, gender);

    printf("Enter client's birthday: ");
    scanf("%s", birthday);
    strcpy(client->birthday, birthday);

    printf("Enter client's password: ");
    scanf("%s", password);
    strcpy(client->password, password);
}

void InputOrder(struct Order *order)
{
    int countOfProduct;
    int complete;
    char date[12];

    printf("Enter Order's count: ");
    scanf("%d", &countOfProduct);
    order->countOfProduct = countOfProduct;

    printf("Enter complete 's Order: ");
    scanf("%d", &complete);
    order->complete = complete;

    printf("Enter order's date: ");
    scanf("%s", &date);
    strcpy(order->date, date);
}