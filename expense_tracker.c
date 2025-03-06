/* Expense Tracker Functions (expense_tracker.c) */
#include "expense_tracker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>


#define MAX_PATH 1024
#define MAX_EXPENSES 100
#define MAX_DEBTS 50

Expense expenses[MAX_EXPENSES];
int expenseCount = 0;
Debt debts[MAX_DEBTS];
int debtCount = 0;

void getExecutablePath(char *buffer, size_t size);

void addExpense(const char *category, float amount, const char *date) {
    if (expenseCount < MAX_EXPENSES) {
        strcpy(expenses[expenseCount].category, category);
        expenses[expenseCount].amount = amount;
        strcpy(expenses[expenseCount].date, date);
        expenseCount++;
        saveExpenses();
    }
}

void saveExpenses() {
    char path[MAX_PATH];
    getExecutablePath(path, sizeof(path));
    strcat(path, "/expenses.csv");

    FILE *file = fopen(path, "w");
    fprintf(file, "Category,Amount,Date\n");
    for (int i = 0; i < expenseCount; i++) {
        fprintf(file, "%s,%.2f,%s\n", expenses[i].category, expenses[i].amount, expenses[i].date);
    }
    fclose(file);
}

void loadExpenses() {
    char path[MAX_PATH];
    getExecutablePath(path, sizeof(path));
    strcat(path, "/expenses.csv");

    FILE *file = fopen(path, "r");
    if (file) {
        char line[100];
        fgets(line, sizeof(line), file); // Skip header line
        while (fscanf(file, "%[^,],%f,%s", expenses[expenseCount].category, &expenses[expenseCount].amount, expenses[expenseCount].date) == 3) {
            expenseCount++;
        }
        fclose(file);
    }
}

void addDebt(const char *name, float amount, int isOwedByMe) {
    if (debtCount < MAX_DEBTS) {
        strcpy(debts[debtCount].name, name);
        debts[debtCount].amount = amount;
        debts[debtCount].isOwedByMe = isOwedByMe;
        debtCount++;
        saveDebts();
    }
}

void saveDebts() {
    char path[MAX_PATH];
    getExecutablePath(path, sizeof(path));
    strcat(path, "/debts.csv");

    FILE *file = fopen(path, "w");
    fprintf(file, "Name,Amount,OwedByMe\n");
    for (int i = 0; i < debtCount; i++) {
        fprintf(file, "%s,%.2f,%d\n", debts[i].name, debts[i].amount, debts[i].isOwedByMe);
    }
    fclose(file);
}

void loadDebts() {
    char path[MAX_PATH];
    getExecutablePath(path, sizeof(path));
    strcat(path, "/debts.csv");

    FILE *file = fopen(path, "r");
    if (file) {
        char line[100];
        fgets(line, sizeof(line), file); // Skip header line
        while (fscanf(file, "%[^,],%f,%d", debts[debtCount].name, &debts[debtCount].amount, &debts[debtCount].isOwedByMe) == 3) {
            debtCount++;
        }
        fclose(file);
    }
}

float getTotalOwedByMe() {
    float total = 0;
    for (int i = 0; i < debtCount; i++) {
        if (debts[i].isOwedByMe) {
            total += debts[i].amount;
        }
    }
    return total;
}

float getTotalOwedToMe() {
    float total = 0;
    for (int i = 0; i < debtCount; i++) {
        if (!debts[i].isOwedByMe) {
            total += debts[i].amount;
        }
    }
    return total;
}


void getExecutablePath(char *buffer, size_t size) {
    ssize_t len = readlink("/proc/self/exe", buffer, size - 1);
    if (len != -1) {
        buffer[len] = '\0';
        dirname(buffer);  // Get the directory of the executable
    } else {
        strcpy(buffer, ".");  // Default to current directory if failed
    }
}