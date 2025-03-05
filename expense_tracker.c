
/* Expense Tracker Functions (expense_tracker.c) */
#include "expense_tracker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPENSES 100
#define MAX_DEBTS 50

Expense expenses[MAX_EXPENSES];
int expenseCount = 0;
Debt debts[MAX_DEBTS];
int debtCount = 0;

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
    FILE *file = fopen("expenses.csv", "w");
    fprintf(file, "Category,Amount,Date\n");
    for (int i = 0; i < expenseCount; i++) {
        fprintf(file, "%s,%.2f,%s\n", expenses[i].category, expenses[i].amount, expenses[i].date);
    }
    fclose(file);
}

void loadExpenses() {
    FILE *file = fopen("expenses.csv", "r");
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
    FILE *file = fopen("debts.csv", "w");
    fprintf(file, "Name,Amount,OwedByMe\n");
    for (int i = 0; i < debtCount; i++) {
        fprintf(file, "%s,%.2f,%d\n", debts[i].name, debts[i].amount, debts[i].isOwedByMe);
    }
    fclose(file);
}

void loadDebts() {
    FILE *file = fopen("debts.csv", "r");
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
