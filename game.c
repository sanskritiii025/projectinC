#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INVENTORY_SIZE 10
#define MAX_HEALTH 100


struct Player {
    char name[50];
    int health;
    int score;
    char inventory[INVENTORY_SIZE][50];
    int inventoryCount;
    int day; 
};


void initializePlayer(struct Player *player) {
    printf("Enter your name, adventurer: ");
    fgets(player->name, 50, stdin);
    player->name[strcspn(player->name, "\n")] = '\0'; 

    player->health = MAX_HEALTH;
    player->score = 0;
    player->inventoryCount = 0;
    player->day = 1;

    printf("\nWelcome, %s! Your adventure begins...\n", player->name);
}


void displayStatus(struct Player *player) {
    printf("\n--- Status ---\n");
    printf("Name: %s\n", player->name);
    printf("Health: %d/%d\n", player->health, MAX_HEALTH);
    printf("Score: %d\n", player->score);
    printf("Day: %d\n", player->day);
    printf("Inventory: ");
    if (player->inventoryCount == 0) {
        printf("Empty\n");
    } else {
        for (int i = 0; i < player->inventoryCount; i++) {
            printf("%s, ", player->inventory[i]);
        }
        printf("\n");
    }
    printf("---------------\n");
}


void addItem(struct Player *player, char item[]) {
    if (player->inventoryCount >= INVENTORY_SIZE) {
        printf("Your inventory is full! You cannot carry %s.\n", item);
        return;
    }

    strcpy(player->inventory[player->inventoryCount], item);
    player->inventoryCount++;
    printf("You found: %s\n", item);
}


void combat(struct Player *player) {
    int enemyHealth = 30;
    printf("\nA wild beast attacks you!\n");

    while (player->health > 0 && enemyHealth > 0) {
        int choice;
        printf("\n1. Attack\n");
        printf("2. Defend\n");
        printf("3. Use Item\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: 
                printf("You attack the beast!\n");
                enemyHealth -= 10;
                printf("Beast's health: %d\n", enemyHealth);
                break;
            case 2: 
                printf("You defend yourself. The beast's attack is weakened!\n");
                break;
            case 3: 
                if (player->inventoryCount > 0) {
                    printf("You use %s to heal yourself!\n", player->inventory[0]);
                    player->health += 20;
                    if (player->health > MAX_HEALTH) player->health = MAX_HEALTH;
                    player->inventoryCount--;
                } else {
                    printf("You have no items to use!\n");
                }
                break;
            default:
                printf("Invalid choice! You hesitate and lose your turn.\n");
        }

        
        if (enemyHealth > 0) {
            printf("The beast attacks you!\n");
            player->health -= 15;
            printf("Your health: %d\n", player->health);
        }
    }

    if (player->health > 0) {
        printf("You defeated the beast! +20 points\n");
        player->score += 20;
    } else {
        printf("You were defeated by the beast...\n");
    }
}


void randomEvent(struct Player *player) {
    srand(time(0));
    int event = rand() % 3;

    switch (event) {
        case 0:
            printf("\nYou stumble upon a hidden treasure chest!\n");
            addItem(player, "Gold Coin");
            player->score += 10;
            break;
        case 1:
            printf("\nA friendly traveler gives you a healing potion.\n");
            addItem(player, "Healing Potion");
            break;
        case 2:
            printf("\nYou step into a trap and lose 10 health!\n");
            player->health -= 10;
            break;
    }
}

void simulateDay(struct Player *player) {
    printf("\n--- Day %d ---\n", player->day);
    printf("The sun rises, and a new day begins.\n");

    int choice;
    printf("\n1. Explore the forest\n");
    printf("2. Rest and recover\n");
    printf("3. Check status\n");
    printf("4. Save and quit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("\nYou venture into the forest...\n");
            randomEvent(player);
            combat(player);
            break;
        case 2:
            printf("\nYou rest and recover 20 health.\n");
            player->health += 20;
            if (player->health > MAX_HEALTH) player->health = MAX_HEALTH;
            break;
        case 3:
            displayStatus(player);
            break;
        case 4:
            return; 
        default:
            printf("\nInvalid choice! The day passes uneventfully.\n");
    }

    player->day++;
}

void saveGame(struct Player *player) {
    FILE *file = fopen("savegame.txt", "w");
    if (file == NULL) {
        printf("Error saving game!\n");
        return;
    }

    fprintf(file, "%s\n", player->name);
    fprintf(file, "%d\n", player->health);
    fprintf(file, "%d\n", player->score);
    fprintf(file, "%d\n", player->inventoryCount);
    for (int i = 0; i < player->inventoryCount; i++) {
        fprintf(file, "%s\n", player->inventory[i]);
    }
    fprintf(file, "%d\n", player->day);

    fclose(file);
    printf("Game saved!\n");
}


void loadGame(struct Player *player) {
    FILE *file = fopen("savegame.txt", "r");
    if (file == NULL) {
        printf("No saved game found!\n");
        return;
    }

    fgets(player->name, 50, file);
    player->name[strcspn(player->name, "\n")] = '\0'; 
    fscanf(file, "%d\n", &player->health);
    fscanf(file, "%d\n", &player->score);
    fscanf(file, "%d\n", &player->inventoryCount);
    for (int i = 0; i < player->inventoryCount; i++) {
        fgets(player->inventory[i], 50, file);
        player->inventory[i][strcspn(player->inventory[i], "\n")] = '\0'; 
    }
    fscanf(file, "%d\n", &player->day);

    fclose(file);
    printf("Game loaded!\n");
}

int main() {
    struct Player player;
    int choice;

    printf("1. New Game\n");
    printf("2. Load Game\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar(); 

    if (choice == 1) {
        initializePlayer(&player);
    } else if (choice == 2) {
        loadGame(&player);
    } else {
        printf("Invalid choice! Exiting...\n");
        return 0;
    }

    while (player.health > 0) {
        simulateDay(&player);

        printf("\nDo you want to save and quit? (1 = Yes, 0 = No): ");
        scanf("%d", &choice);
        if (choice == 1) {
            saveGame(&player);
            break;
        }
    }

    if (player.health <= 0) {
        printf("\nGame Over! Your final score: %d\n", player.score); 
    } else {
        printf("\nThanks for playing! Your adventure continues...\n");
    }

    return 0;
}