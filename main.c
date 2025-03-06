/* Personal Expense Tracker - Main File (main.c) */
#include <stdio.h>
#include "expense_tracker.h"
#include "gui.h"

int main() {
    loadExpenses(); // Load stored expenses
    loadDebts();    // Load stored debts
    createGUI();    // Launch GUI
    return 0;
}
