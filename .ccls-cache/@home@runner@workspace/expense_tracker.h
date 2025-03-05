/* Expense Tracker Header (expense_tracker.h) */
#ifndef EXPENSE_TRACKER_H
#define EXPENSE_TRACKER_H

typedef struct {
    char category[50];
    float amount;
    char date[15];
} Expense;

typedef struct {
    char name[50];
    float amount;
    int isOwedByMe; // 1 if I owe, 0 if someone owes me
} Debt;

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

#endif
