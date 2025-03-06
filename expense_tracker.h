#include <stddef.h>

#ifndef EXPENSE_TRACKER_H
#define EXPENSE_TRACKER_H

typedef struct {
    char category[50];
    float amount;
    char date[20];
} Expense;

typedef struct {
    char name[50];
    float amount;
    int isOwedByMe;
} Debt;

#define MAX_EXPENSES 100
#define MAX_DEBTS 50

extern Expense expenses[MAX_EXPENSES];  // Declare as external
extern int expenseCount;

extern Debt debts[MAX_DEBTS];
extern int debtCount;

void addExpense(const char *category, float amount, const char *date);
void saveExpenses();
void loadExpenses();
void analyzeExpenses();
void comparePreviousMonth();
float getPreviousMonthTotal();
float getCurrentMonthTotal();

void addDebt(const char *name, float amount, int isOwedByMe);
void saveDebts();
void loadDebts();
float getTotalOwedByMe();
float getTotalOwedToMe();

void getExecutablePath(char *buffer, size_t size);

#endif
