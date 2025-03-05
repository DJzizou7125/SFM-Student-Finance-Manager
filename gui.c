/* GUI Functions (gui.c) */
#include "gui.h"
#include "expense_tracker.h"
#include <cairo.h>
#include <gtk/gtk.h>

void createGUI() {
  GtkWidget *window, *drawingArea, *label;
  char expenseText[200];
  float prevMonthTotal = getPreviousMonthTotal();
  float currMonthTotal = getCurrentMonthTotal();
  float totalOwedByMe = getTotalOwedByMe();
  float totalOwedToMe = getTotalOwedToMe();

  gtk_init(NULL, NULL);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Expense Tracker");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  drawingArea = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawingArea, 400, 300);

  sprintf(expenseText,
          "Prev Month: %.2f | Curr Month: %.2f\nOwed by me: %.2f | Owed to me: "
          "%.2f",
          prevMonthTotal, currMonthTotal, totalOwedByMe, totalOwedToMe);
  label = gtk_label_new(expenseText);

  GtkWidget *box = gtk_vbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), drawingArea, TRUE, TRUE, 0);
  gtk_container_add(GTK_CONTAINER(window), box);

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window);
  gtk_main();
}