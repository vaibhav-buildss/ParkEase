#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_VEHICLES 30

int studentId[MAX_VEHICLES];
char vehicleNumber[MAX_VEHICLES][20];
char vehicleType[MAX_VEHICLES][10];
int slotNumber[MAX_VEHICLES];
int active[MAX_VEHICLES];

void initialize()
{
    int i;

    for (i = 0; i < MAX_VEHICLES; i++)
    {
        studentId[i] = 0;
        vehicleNumber[i][0] = '\0';
        vehicleType[i][0] = '\0';
        slotNumber[i] = 0;
        active[i] = 0;
    }
}

int findVehicle(char number[])
{
    int i;

    for (i = 0; i < MAX_VEHICLES; i++)
    {
        if (active[i] == 1 && strcmp(vehicleNumber[i], number) == 0)
            return i;
    }

    return -1;
}

int findEmpty()
{
    int i;

    for (i = 0; i < MAX_VEHICLES; i++)
    {
        if (active[i] == 0)
            return i;
    }

    return -1;
}

int findSlot(char type[])
{
    int i, j, used;

    if (strcmp(type, "Bike") == 0)
    {
        for (i = 0; i < 20; i++)
        {
            used = 0;

            for (j = 0; j < MAX_VEHICLES; j++)
            {
                if (active[j] == 1 && slotNumber[j] == i + 1)
                {
                    used = 1;
                    break;
                }
            }

            if (used == 0)
                return i + 1;
        }
    }
    else
    {
        for (i = 20; i < 30; i++)
        {
            used = 0;

            for (j = 0; j < MAX_VEHICLES; j++)
            {
                if (active[j] == 1 && slotNumber[j] == i + 1)
                {
                    used = 1;
                    break;
                }
            }

            if (used == 0)
                return i + 1;
        }
    }

    return -1;
}

void saveData()
{
    FILE *file;
    int i;

    file = fopen("../data/vehicles.txt", "w");

    if (file == NULL)
        return;

    for (i = 0; i < MAX_VEHICLES; i++)
    {
        if (active[i] == 1)
        {
            fprintf(file, "%d %s %s %d\n",
                    studentId[i],
                    vehicleNumber[i],
                    vehicleType[i],
                    slotNumber[i]);
        }
    }

    fclose(file);
}

void loadData()
{
    FILE *file;
    int id, slot, index;
    char number[20], type[10];

    file = fopen("../data/vehicles.txt", "r");

    if (file == NULL)
        return;

    while (fscanf(file, "%d %19s %9s %d", &id, number, type, &slot) == 4)
    {
        index = findEmpty();

        if (index == -1)
            break;

        studentId[index] = id;
        strcpy(vehicleNumber[index], number);
        strcpy(vehicleType[index], type);
        slotNumber[index] = slot;
        active[index] = 1;
    }

    fclose(file);
}

void printSlot(int slot)
{
    if (slot <= 20)
        printf("A%02d", slot);
    else
        printf("B%02d", slot - 20);
}

void state()
{
    int i;
    int occupied = 0;
    int bikes = 0;
    int cars = 0;
    int first = 1;

    printf("{\"ok\":true,\"occupied\":");

    for (i = 0; i < MAX_VEHICLES; i++)
    {
        if (active[i] == 1)
        {
            occupied++;

            if (strcmp(vehicleType[i], "Bike") == 0)
                bikes++;
            else
                cars++;
        }
    }

    printf("%d,\"available\":%d,\"bikes\":%d,\"cars\":%d,\"vehicles\":[",
           occupied, MAX_VEHICLES - occupied, bikes, cars);

    for (i = 0; i < MAX_VEHICLES; i++)
    {
        if (active[i] == 1)
        {
            if (!first)
                printf(",");

            printf("{\"studentId\":%d,\"number\":\"%s\",\"type\":\"%s\",\"slot\":\"",
                   studentId[i], vehicleNumber[i], vehicleType[i]);

            printSlot(slotNumber[i]);

            printf("\"}");

            first = 0;
        }
    }

    printf("]}");
}

void park(char number[], char type[], int id)
{
    int index;
    int slot;

    if (findVehicle(number) != -1)
    {
        printf("{\"ok\":false,\"message\":\"Vehicle is already parked.\"}");
        return;
    }

    index = findEmpty();

    if (index == -1)
    {
        printf("{\"ok\":false,\"message\":\"Parking is full.\"}");
        return;
    }

    if (strcmp(type, "Bike") != 0 && strcmp(type, "Car") != 0)
    {
        printf("{\"ok\":false,\"message\":\"Invalid vehicle type.\"}");
        return;
    }

    slot = findSlot(type);

    if (slot == -1)
    {
        printf("{\"ok\":false,\"message\":\"No slot available.\"}");
        return;
    }

    studentId[index] = id;
    strcpy(vehicleNumber[index], number);
    strcpy(vehicleType[index], type);
    slotNumber[index] = slot;
    active[index] = 1;

    saveData();

    printf("{\"ok\":true,\"message\":\"Vehicle parked successfully.\",\"slot\":\"");

    printSlot(slot);

    printf("\"}");
}

void removeVehicle(char number[])
{
    int index;

    index = findVehicle(number);

    if (index == -1)
    {
        printf("{\"ok\":false,\"message\":\"Vehicle not found.\"}");
        return;
    }

    active[index] = 0;
    saveData();

    printf("{\"ok\":true,\"message\":\"Vehicle removed successfully.\"}");
}

void search(char number[])
{
    int index;

    index = findVehicle(number);

    if (index == -1)
    {
        printf("{\"ok\":true,\"found\":false}");
        return;
    }

    printf("{\"ok\":true,\"found\":true,\"studentId\":%d,\"number\":\"%s\",\"type\":\"%s\",\"slot\":\"",
           studentId[index], vehicleNumber[index], vehicleType[index]);

    printSlot(slotNumber[index]);

    printf("\"}");
}

int main(int argc, char *argv[])
{
    initialize();
    loadData();

    if (argc < 2)
    {
        printf("{\"ok\":false,\"message\":\"No command.\"}");
        return 0;
    }

    if (strcmp(argv[1], "state") == 0)
    {
        state();
    }
    else if (strcmp(argv[1], "park") == 0 && argc >= 5)
    {
        park(argv[3], argv[4], atoi(argv[2]));
    }
    else if (strcmp(argv[1], "remove") == 0 && argc >= 3)
    {
        removeVehicle(argv[2]);
    }
    else if (strcmp(argv[1], "search") == 0 && argc >= 3)
    {
        search(argv[2]);
    }
    else
    {
        printf("{\"ok\":false,\"message\":\"Invalid command.\"}");
    }

    return 0;
}
