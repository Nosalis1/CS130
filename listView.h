//
// Created by Aleksa on 2/24/2024.
//
// Header guards to prevent multiple inclusions of the header file
#ifndef CS130_PZ_C_LISTVIEW_H
#define CS130_PZ_C_LISTVIEW_H

// Define identifiers for list view control and refresh button
// Declare global variables for list view and refresh button handles
HWND list_view;
#define IDC_LISTVIEW 1000
#define IDC_REFRESH_BUTTON 1001
HWND lw_refresh_button;

// Function prototype for refresh button click event handler
void lw_on_refresh();

// Function to initialize the list view control and refresh button
void lw_init() {
    RECT rect;
    GetClientRect(hwnd, &rect);

    const RECT lw_rect = {
            PADDING,
            PADDING,
            rect.right / 2 - PADDING,
            rect.bottom - (PADDING + 30 + PADDING)
    };

    float width = (float) (lw_rect.right - lw_rect.left);

    list_view = CreateWindow(WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | LVS_REPORT,
                             lw_rect.left, lw_rect.top, (long) width, lw_rect.bottom - lw_rect.top,
                             hwnd, (HMENU) IDC_LISTVIEW, hInstance, NULL);

    const float COLUMN_RATIOS[] = {0.3f, 0.3f, 0.2f, 0.1f, 0.2f};

    LVCOLUMN lvColumn;
    lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    lvColumn.pszText = "Name";
    lvColumn.iSubItem = 0;
    lvColumn.cx = (int) (COLUMN_RATIOS[0] * width);
    ListView_InsertColumn(list_view, 0, &lvColumn);

    lvColumn.pszText = "Surname";
    lvColumn.iSubItem = 1;
    lvColumn.cx = (int) (COLUMN_RATIOS[1] * width);
    ListView_InsertColumn(list_view, 1, &lvColumn);

    lvColumn.pszText = "Index";
    lvColumn.iSubItem = 2;
    lvColumn.cx = (int) (COLUMN_RATIOS[2] * width);
    ListView_InsertColumn(list_view, 2, &lvColumn);

    lvColumn.pszText = "Year";
    lvColumn.iSubItem = 3;
    lvColumn.cx = (int) (COLUMN_RATIOS[3] * width);
    ListView_InsertColumn(list_view, 3, &lvColumn);

    lvColumn.pszText = "Average";
    lvColumn.iSubItem = 4;
    lvColumn.cx = (int) (COLUMN_RATIOS[4] * width);
    ListView_InsertColumn(list_view, 4, &lvColumn);

    ListView_SetExtendedListViewStyle(list_view, LVS_EX_FULLROWSELECT);

    const RECT rb_rect = {
            (rect.right / 4) - (COMPONENT_WIDTH / 2),
            rect.bottom - (PADDING + 30),
            (rect.right / 4) + (COMPONENT_WIDTH / 2),
            rect.bottom - PADDING
    };
    lw_refresh_button = gui_create_button(
            (HMENU) IDC_REFRESH_BUTTON, "Refresh", lw_on_refresh,
            rb_rect.left, rb_rect.top, rb_rect.right - rb_rect.left, rb_rect.bottom - rb_rect.top);
}

// Function to clear all items from the list view control
void lw_clear_list() {
    ListView_DeleteAllItems(list_view);
}

// Function to add a student item to the list view at a specified index
void lw_add_item(Student *student, int index) {
    if (!list_view || !student) {
        return;
    }

    LVITEM lv_item;
    lv_item.mask = LVIF_TEXT;
    lv_item.iItem = index;
    lv_item.iSubItem = 0;
    lv_item.pszText = (LPSTR) malloc(VARCHAR_SIZE);

    ListView_InsertItem(list_view, &lv_item);

    ListView_SetItemText(list_view, index, 0, student->name);

    ListView_SetItemText(list_view, index, 1, student->surname);

    ListView_SetItemText(list_view, index, 2, student->index);

    sprintf(lv_item.pszText, "%d", student->year);
    ListView_SetItemText(list_view, index, 3, lv_item.pszText);

    sprintf(lv_item.pszText, "%.2f", student->average);
    ListView_SetItemText(list_view, index, 4, lv_item.pszText);

    free(lv_item.pszText);
}

// Event handler for the refresh button click event
void lw_on_refresh() {
    lw_clear_list();
    for (int i = 0; i < student_count; i++)lw_add_item(student_get_at(i), i);
}

#endif //CS130_PZ_C_LISTVIEW_H