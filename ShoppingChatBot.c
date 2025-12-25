#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char name[20];
    int price;
    char category[20]; // Tagging each item by category
} Product;

typedef struct {
    char name[20];
    int price;
    int quantity;
} Item;

// Global Data - Added category tags to every item
Product inventory[] = {
    {"Oranges", 2, "Fruits"}, {"Apples", 3, "Fruits"}, {"Bananas", 1, "Fruits"},
    {"Carrots", 1, "Vegetables"}, {"Broccoli", 4, "Vegetables"}, {"Spinach", 2, "Vegetables"},
    {"Milk", 5, "Groceries"}, {"Cheese", 7, "Groceries"}, {"Bread", 3, "Groceries"}, {"Meat", 10, "Groceries"},
    {"Soda", 2, "Drinks"}, {"Water", 1, "Drinks"}, {"Juice", 4, "Drinks"}, {"Energy Drinks", 6, "Drinks"}
};

// Automatically calculate size based on the array above
int invSize = sizeof(inventory) / sizeof(inventory[0]);

Item cart[10]; 
int cartCount = 0;
char currentUser[30];

// Function Prototypes
void clearBuffer();
void showMenu();
void addToCart(char *name, int price);
void removeItems();
int checkOut();
void searchInventory();
void displayByCategory(char *catName);

int main() {
    printf("--- WELCOME TO Cos-SHOP ---\n");
    printf("Register Username: ");
    fgets(currentUser, sizeof(currentUser), stdin);
    currentUser[strcspn(currentUser, "\n")] = '\0'; 

    showMenu();
    return 0;
}

void showMenu() {
    int choice;
    while (1) {
        printf("\nWelcome, %s! Select an option:\n", currentUser);
        printf("1. Fruits\n2. Vegetables\n3. Groceries\n4. Drinks\n5. SEARCH ITEMS\n");
        if (cartCount > 0) {
            printf("6. Remove Items\n7. Check Out\n");
        }
        printf("0. Exit\nChoice: ");
        scanf("%d", &choice);
        clearBuffer();

        if (choice == 1) displayByCategory("Fruits");
        else if (choice == 2) displayByCategory("Vegetables");
        else if (choice == 3) displayByCategory("Groceries");
        else if (choice == 4) displayByCategory("Drinks");
        else if (choice == 5) searchInventory();
        else if (choice == 6 && cartCount > 0) removeItems();
        else if (choice == 7 && cartCount > 0) {
            if (checkOut()) {
            break;
        }
    }
        else if (choice == 0) break;
        else printf("Invalid option!\n");
    }
}

// This function does searching for matching categories
void displayByCategory(char *catName) {
    int map[20]; // Maps the 1,2,3 choice back to the original inventory index
    int localCount = 0;

    printf("\n--- %s ---\n", catName);
    for (int i = 0; i < invSize; i++) {
        if (strcmp(inventory[i].category, catName) == 0) {
            printf("%d. %s ($%d)\n", localCount + 1, inventory[i].name, inventory[i].price);
            map[localCount] = i; 
            localCount++;
        }
    }

    printf("Choice (0 to go back): ");
    int itemChoice;
    scanf("%d", &itemChoice);
    if (itemChoice > 0 && itemChoice <= localCount) {
        int actualIdx = map[itemChoice - 1];
        addToCart(inventory[actualIdx].name, inventory[actualIdx].price);
    }
}

void searchInventory() {
    char query[20];
    int found = 0;
    printf("Enter item name to search: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';

    printf("\nSearch Results for '%s':\n", query);
    for (int i = 0; i < invSize; i++) {
        if (strstr(inventory[i].name, query) != NULL) {
            printf("[%d] %s - $%d (%s)\n", i, inventory[i].name, inventory[i].price, inventory[i].category);
            found = 1;
        }
    }

    if (!found) {
        printf("No items match your search.\n");
    } else {
        printf("Enter item ID to add (or -1 to cancel): ");
        int choice;
        scanf("%d", &choice);
        if (choice >= 0 && choice < invSize) {
            addToCart(inventory[choice].name, inventory[choice].price);
        }
    }
}

void addToCart(char *name, int price) {
    for (int i = 0; i < cartCount; i++) {
        if (strcmp(cart[i].name, name) == 0) {
            int qty;
            printf("%s is already in cart. Add how many more? ", name);
            scanf("%d", &qty);
            cart[i].quantity += qty;
            return;
        }
    }

    if (cartCount >= 10) {
        printf("Cart is full!\n");
        return;
    }

    int qty;
    printf("How many %s? ", name);
    scanf("%d", &qty);
    strcpy(cart[cartCount].name, name);
    cart[cartCount].price = price;
    cart[cartCount].quantity = qty;
    cartCount++;
    printf("Added to cart!\n");
}

void removeItems() {
    printf("\n--- YOUR CART ---\n");
    for (int i = 0; i < cartCount; i++) {
        printf("%d. %s (Qty: %d) - $%d total\n", i + 1, cart[i].name, cart[i].quantity, cart[i].price * cart[i].quantity);
    }
    printf("Select number to remove (0 to cancel): ");
    int index;
    scanf("%d", &index);
    if (index > 0 && index <= cartCount) {
        for (int i = index - 1; i < cartCount - 1; i++) {
            cart[i] = cart[i + 1];
        }
        cartCount--;
        printf("Item removed.\n");
    }
}

int checkOut() {
    int total = 0;
    printf("\n--- FINAL RECEIPT FOR %s ---\n", currentUser);
    for (int i = 0; i < cartCount; i++) {
        int subtotal = cart[i].price * cart[i].quantity;
        printf("%-15s x%d: $%d\n", cart[i].name, cart[i].quantity, subtotal);
        total += subtotal;
    }
    printf("--------------------\n");
    printf("TOTAL FEES: $%d\n", total);
    printf("Confirm purchase? (1-Yes, 0-No): ");
    
    int confirm;
    scanf("%d", &confirm);
    clearBuffer(); // Keep the buffer clean

    if (confirm) {
        printf("Receipt Printed. Thank you!\n");
        return 1; // Return 1 to indicate success/exit
    } else {
        printf("Checkout cancelled. Returning to menu...\n");
        return 0; // Return 0 to stay in the menu
    }
}

void clearBuffer() { // Used to prevent readings of leftover characters
    while (getchar() != '\n'); 
}
