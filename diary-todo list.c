#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //how does this even work ;-;

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
//CSV code stolen from stackoverflow :D
void save_to_csv() {
    FILE *file = fopen("diary.csv", "w");
    if (!file) {
        printf("Error opening diary.csv for writing.\n");
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

struct todo_node {
    char task[1024];
    char date[20];
    char status[16]; 
    struct todo_node* next;
    struct todo_node* prev;
};

struct todo_node* todo_head = NULL;
struct todo_node* todo_tail = NULL;

void todo_insert_tail(char *dateStr, char *taskText, char *statusStr) {
    struct todo_node* ptr = (struct todo_node*)malloc(sizeof(struct todo_node));
    strcpy(ptr->date, dateStr);
    strcpy(ptr->task, taskText);
    strcpy(ptr->status, statusStr);
    ptr->next = NULL;
    ptr->prev = todo_tail;

    if (todo_tail != NULL)
        todo_tail->next = ptr;
    else
        todo_head = ptr;

    todo_tail = ptr;
}

void display_todo() {
    struct todo_node* temp = todo_head;
    if (!temp) {
        printf("\nNo tasks in To-Do list.\n");
        return;
    }
    int idx = 1;
    printf("\n------ To-Do Tasks ------\n");
    while (temp) {
        printf("[%d] Date: %s | Status: %s\n", idx, temp->date, temp->status);
        printf("     Task: %s\n", temp->task);
        printf("--------------------------------\n");
        temp = temp->next;
        idx++;
    }
}

int todo_count() {
    struct todo_node* temp = todo_head;
    int c = 0;
    while (temp) {
        c++;
        temp = temp->next;
    }
    return c;
}

void mark_task_done(int index) {
    if (index <= 0) {
        printf("Invalid index.\n");
        return;
    }
    struct todo_node* temp = todo_head;
    int i = 1;
    while (temp && i < index) {
        temp = temp->next;
        i++;
    }
    if (!temp) {
        printf("No task at that index.\n");
        return;
    }
    strcpy(temp->status, "Done");
    printf("Task %d marked as Done.\n", index);
}

void delete_task(int index) {
    if (index <= 0) {
        printf("Invalid index.\n");
        return;
    }
    struct todo_node* temp = todo_head;
    int i = 1;
    while (temp && i < index) {
        temp = temp->next;
        i++;
    }
    if (!temp) {
        printf("No task at that index.\n");
        return;
    }
    if (temp->prev) temp->prev->next = temp->next;
    else todo_head = temp->next;
    if (temp->next) temp->next->prev = temp->prev;
    else todo_tail = temp->prev;
    free(temp);
    printf("Task %d deleted.\n", index);
}

void save_todo_to_csv() {
    FILE *file = fopen("todo.csv", "w");
    if (!file) {
        printf("Error opening todo.csv for writing.\n");
        return;
    }
    struct todo_node* temp = todo_head;
    while (temp) {
        fprintf(file, "%s,\"%s\",%s\n", temp->date, temp->task, temp->status);
        temp = temp->next;
    }
    fclose(file);
}

void load_todo_from_csv() {
    FILE *file = fopen("todo.csv", "r");
    if (!file) return;

    char line[4096];
    while (fgets(line, sizeof(line), file)) {
        char *first_comma = strchr(line, ',');
        if (!first_comma) continue;
        *first_comma = 0;
        char *date = line;
        char *rest = first_comma + 1;

        char *last_comma = strrchr(rest, ',');
        if (!last_comma) continue;
        *last_comma = 0;
        char *task = rest;
        char *status = last_comma + 1;

        task[strcspn(task, "\n")] = 0;
        status[strcspn(status, "\n")] = 0;
        if (task[0] == '"' && task[strlen(task)-1] == '"') {
            task[strlen(task)-1] = 0;
            task++;
        }
        if (status[0] == '"' && status[strlen(status)-1] == '"') {
            status[strlen(status)-1] = 0;
            status++;
        }
        todo_insert_tail(date, task, status);
    }
    fclose(file);
}

void diary_menu() {
    int choice;
    char buffer[1024];
    char timeStr[20];
    char searchDate[20];

    while (1) {
        printf("\n==== Diary ====\n");
        printf("1. Write Entry\n");
        printf("2. View Entries (Oldest → Newest)\n");
        printf("3. View Entries (Newest → Oldest)\n");
        printf("4. Search Entries by Date\n");
        printf("5. Back to Main Menu\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { getchar(); continue; }
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
                if (!fgets(buffer, sizeof(buffer), stdin)) buffer[0] = 0;
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
                if (scanf("%19s", searchDate) != 1) { getchar(); break; }
                getchar();
                search_by_date(searchDate);
                break;
            case 5:
                return;
            default:
                printf("Invalid choice!\n");
        }
    }
}

void todo_menu() {
    int choice;
    char buffer[1024];
    char dateStr[20];
    int idx;

    while (1) {
        printf("\n==== To-Do List ====\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Mark Task as Done\n");
        printf("4. Delete Task\n");
        printf("5. Back to Main Menu\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { getchar(); continue; }
        getchar();

        switch (choice) {
            case 1: {
                time_t lt = time(NULL);
                struct tm *currentTime = localtime(&lt);
                sprintf(dateStr, "%04d-%02d-%02d",
                        currentTime->tm_year + 1900,
                        currentTime->tm_mon + 1,
                        currentTime->tm_mday);

                printf("Task date (default %s). Press enter to use default or type YYYY-MM-DD: ", dateStr);
                if (!fgets(buffer, sizeof(buffer), stdin)) buffer[0] = 0;
                buffer[strcspn(buffer, "\n")] = 0;
                if (strlen(buffer) > 0) strncpy(dateStr, buffer, sizeof(dateStr));
                printf("Enter task description:\n");
                if (!fgets(buffer, sizeof(buffer), stdin)) buffer[0] = 0;
                buffer[strcspn(buffer, "\n")] = 0;

                todo_insert_tail(dateStr, buffer, "Pending");
                save_todo_to_csv();
                break;
            }
            case 2:
                display_todo();
                break;
            case 3:
                display_todo();
                if (todo_count() == 0) break;
                printf("Enter task index to mark as Done: ");
                if (scanf("%d", &idx) != 1) { getchar(); break; }
                getchar();
                mark_task_done(idx);
                save_todo_to_csv();
                break;
            case 4:
                display_todo();
                if (todo_count() == 0) break;
                printf("Enter task index to delete: ");
                if (scanf("%d", &idx) != 1) { getchar(); break; }
                getchar();
                delete_task(idx);
                save_todo_to_csv();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice!\n");
        }
    }
}

int main() {
    int choice;

    load_from_csv();
    load_todo_from_csv();

    while (1) {
        printf("\n==== Main Menu ====\n");
        printf("1. Diary\n");
        printf("2. To-Do List\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { getchar(); continue; }
        getchar();

        switch (choice) {
            case 1:
                diary_menu();
                break;
            case 2:
                todo_menu();
                break;
            case 3:
                save_to_csv();
                save_todo_to_csv();
                printf("Exiting. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
