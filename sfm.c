#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <cairo.h>

#define MAX_PATH 1024
#define MAX_EXPENSES 100
#define MAX_DEBTS 50

typedef struct {
    char category[30];
    float amount;
    char date[11];
} Expense;

typedef struct {
    char name[30];
    float amount;
    int isOwedByMe; // 1 if owed by me, 0 if owed to me
} Debt;

Expense expenses[MAX_EXPENSES];
int expenseCount = 0;
Debt debts[MAX_DEBTS];
int debtCount = 0;

void getExecutablePath(char *buffer, size_t size) {
    readlink("/proc/self/exe", buffer, size);
    char *lastSlash = strrchr(buffer, '/');
    if (lastSlash) *lastSlash = '\0';
}

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

void displayMonthlyComparison() {
    printf("Displaying monthly comparison...\n");
}

void createGraph() {
    cairo_surface_t *surface;
    cairo_t *cr;
    int width = 600, height = 400;

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1, 1, 1); // White background
    cairo_paint(cr);

    // Example data setup for graph visualization
    // Assume expensesData and categories are prepared
    // Adjust the logic as needed for actual data

    cairo_surface_write_to_png(surface, "expenses_graph.png");
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

GtkWidget *categoryCombo, *amountEntry, *dateEntry, *expenseList, *totalLabel, *debtLabel;

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
}

void addExpenseFromGUI() {
    const char *category = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(categoryCombo));
    const char *amount_text = gtk_entry_get_text(GTK_ENTRY(amountEntry));
    const char *date = gtk_entry_get_text(GTK_ENTRY(dateEntry));
    float amount = atof(amount_text);

    addExpense(category, amount, date);
    refreshExpenseList();
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
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 500);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    categoryCombo = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(categoryCombo), NULL, "Food");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(categoryCombo), NULL, "Transport");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(categoryCombo), NULL, "Rent");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(categoryCombo), NULL, "Entertainment");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(categoryCombo), NULL, "Shopping");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(categoryCombo), NULL, "Miscellaneous");

    amountEntry = gtk_entry_new();
    dateEntry = gtk_entry_new();
    addButton = gtk_button_new_with_label("Add Expense");

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Category:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), categoryCombo, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Amount:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), amountEntry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Date (YYYY-MM-DD):"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dateEntry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), addButton, 1, 3, 1, 1);

    store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_STRING);
    expenseList = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Category", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(expenseList), col);

    g_signal_connect(addButton, "clicked", G_CALLBACK(addExpenseFromGUI), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    scrollWin = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrollWin), 200);
    gtk_container_add(GTK_CONTAINER(scrollWin), expenseList);
    gtk_grid_attach(GTK_GRID(grid), scrollWin, 0, 4, 2, 1);

    gtk_widget_show_all(window);
    gtk_main();
}

int main() {
    loadExpenses(); // Load stored expenses
    createGUI();    // Launch GUI
    return 0;
}