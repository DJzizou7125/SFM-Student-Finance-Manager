/* GUI Functions (gui.c) */
#include "gui.h"
#include "expense_tracker.h"
#include <gtk/gtk.h>
#include <stdlib.h>

// Global GUI elements
GtkWidget *categoryEntry, *amountEntry, *dateEntry, *expenseList, *totalLabel, *debtLabel;

void refreshExpenseList() {
    gtk_list_store_clear(GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(expenseList))));

    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(expenseList)));
    GtkTreeIter iter;
    float total = 0;

    for (int i = 0; i < expenseCount; i++) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, expenses[i].category, 1, expenses[i].amount, 2, expenses[i].date, -1);
        total += expenses[i].amount;
    }

    char totalText[50];
    sprintf(totalText, "Total Expenses: $%.2f", total);
    gtk_label_set_text(GTK_LABEL(totalLabel), totalText);

    float totalOwedByMe = getTotalOwedByMe();
    float totalOwedToMe = getTotalOwedToMe();
    char debtText[100];
    sprintf(debtText, "Owed by me: $%.2f | Owed to me: $%.2f", totalOwedByMe, totalOwedToMe);
    gtk_label_set_text(GTK_LABEL(debtLabel), debtText);
}

void addExpenseFromGUI(GtkWidget *widget, gpointer data) {
    const char *category = gtk_entry_get_text(GTK_ENTRY(categoryEntry));
    const char *amountText = gtk_entry_get_text(GTK_ENTRY(amountEntry));
    const char *date = gtk_entry_get_text(GTK_ENTRY(dateEntry));

    float amount = atof(amountText);
    if (amount > 0 && category[0] != '\0' && date[0] != '\0') {
        addExpense(category, amount, date);
        gtk_entry_set_text(GTK_ENTRY(categoryEntry), "");
        gtk_entry_set_text(GTK_ENTRY(amountEntry), "");
        gtk_entry_set_text(GTK_ENTRY(dateEntry), "");
        refreshExpenseList();
    }
}

void createGUI() {
    GtkWidget *window, *grid, *addButton, *scrollWin;
    GtkListStore *store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *col;

    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Expense Tracker");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    categoryEntry = gtk_entry_new();
    amountEntry = gtk_entry_new();
    dateEntry = gtk_entry_new();
    addButton = gtk_button_new_with_label("Add Expense");

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Category:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), categoryEntry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Amount:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), amountEntry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Date (YYYY-MM-DD):"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dateEntry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), addButton, 1, 3, 1, 1);

    // Expense List
    store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_STRING);
    expenseList = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Category", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(expenseList), col);

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Amount", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(expenseList), col);

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Date", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(expenseList), col);

    scrollWin = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrollWin, 400, 150);
    gtk_container_add(GTK_CONTAINER(scrollWin), expenseList);
    gtk_grid_attach(GTK_GRID(grid), scrollWin, 0, 4, 2, 1);

    totalLabel = gtk_label_new("Total Expenses: $0.00");
    gtk_grid_attach(GTK_GRID(grid), totalLabel, 0, 5, 2, 1);

    debtLabel = gtk_label_new("Owed by me: $0.00 | Owed to me: $0.00");
    gtk_grid_attach(GTK_GRID(grid), debtLabel, 0, 6, 2, 1);

    g_signal_connect(addButton, "clicked", G_CALLBACK(addExpenseFromGUI), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    refreshExpenseList();

    gtk_widget_show_all(window);
    gtk_main();
}
