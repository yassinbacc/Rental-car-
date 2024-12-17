#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARS 100
#define MAX_STRING_LENGTH 50

// Car structure definition
typedef struct {
    int id;
    char make[MAX_STRING_LENGTH];
    char model[MAX_STRING_LENGTH];
    int year;
    float daily_rate;
    int is_available;
} Car;

// Global array to store cars
Car cars[MAX_CARS];
int total_cars = 0;

// Function prototypes
void load_cars_from_file();
void save_cars_to_file();
void add_car();
void display_all_cars();
void rent_car();
void return_car();
void delete_car();
int find_car_by_id(int id);

// Load cars from file at program start
void load_cars_from_file() {
    FILE *file = fopen("carsdb.txt", "r");
    if (file == NULL) {
        printf("No existing car database found. Creating new database.\n");
        return;
    }

    total_cars = 0;
    while (fscanf(file, "%d %49s %49s %d %f %d\n", 
        &cars[total_cars].id, 
        cars[total_cars].make, 
        cars[total_cars].model, 
        &cars[total_cars].year, 
        &cars[total_cars].daily_rate, 
        &cars[total_cars].is_available) == 6) {
        total_cars++;
        if (total_cars >= MAX_CARS) break;
    }

    fclose(file);
    printf("Loaded %d cars from database.\n", total_cars);
}

// Save cars to file
void save_cars_to_file() {
    FILE *file = fopen("carsdb.txt", "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    for (int i = 0; i < total_cars; i++) {
        fprintf(file, "%d %s %s %d %.2f %d\n", 
            cars[i].id, 
            cars[i].make, 
            cars[i].model, 
            cars[i].year, 
            cars[i].daily_rate, 
            cars[i].is_available);
    }

    fclose(file);
    printf("Cars saved to database successfully.\n");
}

// Add a new car
void add_car() {
    if (total_cars >= MAX_CARS) {
        printf("Database is full. Cannot add more cars.\n");
        return;
    }

    Car new_car;
    printf("Enter Car ID: ");
    scanf("%d", &new_car.id);

    // Check if car ID already exists
    if (find_car_by_id(new_car.id) != -1) {
        printf("Car with ID %d already exists.\n", new_car.id);
        return;
    }

    printf("Enter Make: ");
    scanf("%49s", new_car.make);

    printf("Enter Model: ");
    scanf("%49s", new_car.model);

    printf("Enter Year: ");
    scanf("%d", &new_car.year);

    printf("Enter Daily Rate: ");
    scanf("%f", &new_car.daily_rate);

    new_car.is_available = 1;  // New cars are available by default

    cars[total_cars++] = new_car;
    save_cars_to_file();
    printf("Car added successfully.\n");
}

// Display all cars
void display_all_cars() {
    load_cars_from_file();
    if (total_cars == 0) {
        printf("No cars in the database.\n");
        return;
    }

    printf("\n--- Car Inventory ---\n");
    printf("ID\tMake\tModel\tYear\tDaily Rate\tAvailability\n");
    printf("---------------------------------------------------\n");

    for (int i = 0; i < total_cars; i++) {
        printf("%d\t%s\t%s\t%d\t$%.2f\t\t%s\n", 
            cars[i].id, 
            cars[i].make, 
            cars[i].model, 
            cars[i].year, 
            cars[i].daily_rate, 
            cars[i].is_available ? "Available" : "Rented");
    }
}

// Find car index by ID
int find_car_by_id(int id) {
    for (int i = 0; i < total_cars; i++) {
        if (cars[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Rent a car
void rent_car() {
    int car_id, index;

    display_all_cars();
    printf("\nEnter Car ID to rent: ");
    scanf("%d", &car_id);

    index = find_car_by_id(car_id);
    if (index == -1) {
        printf("Car with ID %d not found.\n", car_id);
        return;
    }

    if (!cars[index].is_available) {
        printf("Car is already rented.\n");
        return;
    }

    cars[index].is_available = 0;
    save_cars_to_file();
    printf("Car rented successfully. Total cost per day: $%.2f\n", cars[index].daily_rate);
}

// Return a car
void return_car() {
    int car_id, index;

    printf("Enter Car ID to return: ");
    scanf("%d", &car_id);

    index = find_car_by_id(car_id);
    if (index == -1) {
        printf("Car with ID %d not found.\n", car_id);
        return;
    }

    if (cars[index].is_available) {
        printf("Car is already in the inventory.\n");
        return;
    }

    cars[index].is_available = 1;
    save_cars_to_file();
    printf("Car returned successfully.\n");
}

// Delete a car from the database
void delete_car() {
    int car_id, index;

    printf("Enter Car ID to delete: ");
    scanf("%d", &car_id);

    index = find_car_by_id(car_id);
    if (index == -1) {
        printf("Car with ID %d not found.\n", car_id);
        return;
    }

    // Remove the car by shifting array elements
    for (int i = index; i < total_cars - 1; i++) {
        cars[i] = cars[i + 1];
    }
    total_cars--;

    save_cars_to_file();
    printf("Car deleted successfully.\n");
}

// Main menu
void display_menu() {
    printf("\n--- Rental Car Management System ---\n");
    printf("1. Add a Car\n");
    printf("2. Display All Cars\n");
    printf("3. Rent a Car\n");
    printf("4. Return a Car\n");
    printf("5. Delete a Car\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    int choice;

    load_cars_from_file();

    while (1) {
        display_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_car();
                break;
            case 2:
                display_all_cars();
                break;
            case 3:
                rent_car();
                break;
            case 4:
                return_car();
                break;
            case 5:
                delete_car();
                break;
            case 6:
                save_cars_to_file();
                printf("Exiting the program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}