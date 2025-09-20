#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //how does this even work

struct node {
    char text[1024];
    char date[20];
    struct node* next;
    struct node* prev;
};

struct node* head = NULL;
struct node* tail = NULL;

void insert_tail(char *timeStr, char *entryText) {
    struct node* ptr = (struct node*)malloc(sizeof(struct node));
    strcpy(ptr->date, timeStr);
    strcpy(ptr->text, entryText);
    ptr->next = NULL;
    ptr->prev = tail;

    if (tail != NULL)
        tail->next = ptr;
    else
        head = ptr;

    tail = ptr;
}

void display_entries() {
    struct node* temp = head;
    if (!temp) {
        printf("\nNo entries in diary.\n");
        return;
    }

    printf("\n------ Diary Entries (Oldest → Newest) ------\n");
    while (temp) {
        printf("Date: %s\n", temp->date);
        printf("Entry: %s\n", temp->text);
        printf("--------------------------------------------\n");
        temp = temp->next;
    }
}

void display_reverse() {
    struct node* temp = tail;
    if (!temp) {
        printf("\nNo entries in diary.\n");
        return;
    }

    printf("\n------ Diary Entries (Newest → Oldest) ------\n");
    while (temp) {
        printf("Date: %s\n", temp->date);
        printf("Entry: %s\n", temp->text);
        printf("--------------------------------------------\n");
        temp = temp->prev;
    }
}

void search_by_date(char *searchDate) {
    struct node* temp = head;
    int found = 0;

    printf("\n------ Entries for %s ------\n", searchDate);
    while (temp) {
        if (strcmp(temp->date, searchDate) == 0) {
            printf("Entry: %s\n", temp->text);
            printf("--------------------------------\n");
            found = 1;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("No entries found for this date.\n");
    }
}
//csv functions shamelessly stolen from the internet :D
void save_to_csv() {
    FILE *file = fopen("diary.csv", "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }
    struct node* temp = head;
    while (temp) {
        fprintf(file, "%s,\"%s\"\n", temp->date, temp->text);
        temp = temp->next;
    }
    fclose(file);
}

void load_from_csv() {
    FILE *file = fopen("diary.csv", "r");
    if (!file) return;

    char line[2048];
    while (fgets(line, sizeof(line), file)) {
        char *comma = strchr(line, ',');
        if (!comma) continue;

        *comma = 0;
        char *date = line;
        char *text = comma + 1;

        text[strcspn(text, "\n")] = 0;
        if (text[0] == '"' && text[strlen(text)-1] == '"') {
            text[strlen(text)-1] = 0;
            text++;
        }

        insert_tail(date, text);
    }
    fclose(file);
}

int main() {
    int choice;
    char buffer[1024];
    char timeStr[20];
    char searchDate[20];

    load_from_csv();

    while (1) {
        printf("\n==== Personal Diary ====\n");
        printf("1. Write Entry\n");
        printf("2. View Entries (Oldest → Newest)\n");
        printf("3. View Entries (Newest → Oldest)\n");
        printf("4. Search Entries by Date\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                time_t lt = time(NULL);
                struct tm *currentTime = localtime(&lt);
                sprintf(timeStr, "%04d-%02d-%02d",
                        currentTime->tm_year + 1900,
                        currentTime->tm_mon + 1,
                        currentTime->tm_mday);

                printf("Writing at date: %s\n", timeStr);
                printf("Enter your diary text:\n");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;

                insert_tail(timeStr, buffer);
                save_to_csv();
                break;
            }

            case 2:
                display_entries();
                break;

            case 3:
                display_reverse();
                break;

            case 4:
                printf("Enter date to search (YYYY-MM-DD): ");
                scanf("%s", searchDate);
                search_by_date(searchDate);
                break;

            case 5:
                save_to_csv();
                printf("Exiting diary. Goodbye!\n");
                exit(0);

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}