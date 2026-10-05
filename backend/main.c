#include <stdio.h>
#include <string.h>

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
        {
            return i;
        }
    }

    return -1;
}

int findEmpty()
{
    int i;

    for (i = 0; i < MAX_VEHICLES; i++)
    {
        if (active[i] == 0)
        {
            return i;
        }
    }

    return -1;
}

int findSlot(char type[])
{
    int i;

    if (strcmp(type, "Bike") == 0)
    {
        for (i = 0; i < 20; i++)
        {
            int used = 0;
            int j;

            for (j = 0; j < MAX_VEHICLES; j++)
            {
                if (active[j] == 1 && slotNumber[j] == i + 1)
                {
                    used = 1;
                    break;
                }
            }

            if (used == 0)
            {
                return i + 1;
            }
        }
    }
    else
    {
        for (i = 20; i < 30; i++)
        {
            int used = 0;
            int j;

            for (j = 0; j < MAX_VEHICLES; j++)
            {
                if (active[j] == 1 && slotNumber[j] == i + 1)
                {
                    used = 1;
                    break;
                }
            }

            if (used == 0)
            {
                return i + 1;
            }
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
    {
        printf("Could not save data.\n");
        return;
    }

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
    int id;
    char number[20];
    char type[10];
    int slot;
    int index;

    file = fopen("../data/vehicles.txt", "r");

    if (file == NULL)
    {
        return;
    }

    while (fscanf(file, "%d %19s %9s %d", &id, number, type, &slot) == 4)
    {
        index = findEmpty();

        if (index == -1)
        {
            break;
        }

        studentId[index] = id;
        strcpy(vehicleNumber[index], number);
        strcpy(vehicleType[index], type);
        slotNumber[index] = slot;
        active[index] = 1;
    }

    fclose(file);
}

void parkVehicle()
{
    int index;
    int slot;
    int id;
    char number[20];
    char type[10];

    index = findEmpty();

    if (index == -1)
    {
        printf("\nParking is full.\n");
        return;
    }

    printf("\nEnter Student ID: ");
    scanf("%d", &id);

    printf("Enter Vehicle Number: ");
    scanf("%19s", number);

    if (findVehicle(number) != -1)
    {
        printf("This vehicle is already parked.\n");
        return;
    }

    printf("Enter Vehicle Type (Bike/Car): ");
    scanf("%9s", type);

    if (strcmp(type, "Bike") != 0 && strcmp(type, "Car") != 0)
    {
        printf("Invalid vehicle type.\n");
        return;
    }

    slot = findSlot(type);

    if (slot == -1)
    {
        printf("No slot available for this vehicle type.\n");
        return;
    }

    studentId[index] = id;
    strcpy(vehicleNumber[index], number);
    strcpy(vehicleType[index], type);
    slotNumber[index] = slot;
    active[index] = 1;

    saveData();

    printf("\nVehicle parked successfully.\n");
    printf("Slot: ");

    if (slot <= 20)
        printf("A%02d\n", slot);
    else
        printf("B%02d\n", slot - 20);
}

void removeVehicle()
{
    char number[20];
    int index;

    printf("\nEnter Vehicle Number: ");
    scanf("%19s", number);

    index = findVehicle(number);

    if (index == -1)
    {
        printf("Vehicle not found.\n");
        return;
    }

    printf("\nVehicle removed successfully.\n");
    printf("Vehicle: %s\n", vehicleNumber[index]);

    active[index] = 0;
    saveData();
}

void searchVehicle()
{
    char number[20];
    int index;

    printf("\nEnter Vehicle Number: ");
    scanf("%19s", number);

    index = findVehicle(number);

    if (index == -1)
    {
        printf("Vehicle not found.\n");
        return;
    }

    printf("\nVehicle Found\n");
    printf("Student ID: %d\n", studentId[index]);
    printf("Vehicle Number: %s\n", vehicleNumber[index]);
    printf("Vehicle Type: %s\n", vehicleType[index]);
    printf("Slot: ");

    if (slotNumber[index] <= 20)
        printf("A%02d\n", slotNumber[index]);
    else
        printf("B%02d\n", slotNumber[index] - 20);
}

void displayVehicles()
{
    int i;
    int found = 0;

    printf("\n===== CURRENTLY PARKED =====\n");

    printf("%-8s %-15s %-10s %-10s\n",
           "Slot", "Vehicle No.", "Type", "Student ID");

    for (i = 0; i < MAX_VEHICLES; i++)
    {
        if (active[i] == 1)
        {
            found = 1;

            if (slotNumber[i] <= 20)
                printf("A%02d     ", slotNumber[i]);
            else
                printf("B%02d     ", slotNumber[i] - 20);

            printf("%-15s %-10s %-10d\n",
                   vehicleNumber[i],
                   vehicleType[i],
                   studentId[i]);
        }
    }

    if (found == 0)
    {
        printf("No vehicles are currently parked.\n");
    }
}

int main()
{
    int choice;

    initialize();
    loadData();

    do
    {
        printf("\n==============================\n");
        printf("          PARKEASE\n");
        printf(" College Parking Management\n");
        printf("==============================\n");
        printf("1. Park Vehicle\n");
        printf("2. Remove Vehicle\n");
        printf("3. Search Vehicle\n");
        printf("4. View Currently Parked\n");
        printf("5. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            parkVehicle();
            break;

        case 2:
            removeVehicle();
            break;

        case 3:
            searchVehicle();
            break;

        case 4:
            displayVehicles();
            break;

        case 5:
            printf("\nThank you for using ParkEase.\n");
            break;

        default:
            printf("\nInvalid choice.\n");
        }

    } while (choice != 5);

    return 0;
}
