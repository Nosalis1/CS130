//
// Created by Aleksa on 2/24/2024.
//
// Header guards to prevent multiple inclusions of the header file
#ifndef CS130_PZ_C_EDITOR_H
#define CS130_PZ_C_EDITOR_H

// Define identifiers for various GUI elements
HWND editor_name_textbox, editor_surname_textbox, editor_index_textbox, editor_year_textbox, editor_average_textbox;

// Declare global variables for GUI element handles
#define IDC_ADD_STUDENT_BUTTON 1005
HWND editor_add_student_button;

#define IDC_EDIT_STUDENT_BUTTON 1006
HWND editor_edit_student_button;

#define IDC_REMOVE_STUDENT_BUTTON 1007
HWND editor_remove_student_button;

// Function prototypes for button click event handlers
void editor_on_add_student_button();

void editor_on_edit_student_button();

void editor_on_remove_student_button();

void editor_notify_selection_changed(int previous_index, int current_index);

// Function to add spacing between GUI elements
static void editor_add_spacing(int *y) {
    *y += PADDING + SPACING + COMPONENT_HEIGHT;
}

// Function to initialize the editor GUI
void editor_init() {
    RECT rect;
    GetClientRect(hwnd, &rect);

    const int X = rect.right / 2 + SPACING / 2;
    const int X_0 = X + SPACING + COMPONENT_WIDTH;

    int y = PADDING + COMPONENT_HEIGHT + SPACING * 2;

    gui_create_label_default("Name: ", X, y);
    editor_name_textbox = gui_create_textbox_default("Name here", X_0, y);
    editor_add_spacing(&y);

    gui_create_label_default("Surname: ", X, y);
    editor_surname_textbox = gui_create_textbox_default("Surname here", X_0, y);
    editor_add_spacing(&y);

    gui_create_label_default("Index: ", X, y);
    editor_index_textbox = gui_create_textbox_default("Index here", X_0, y);
    editor_add_spacing(&y);

    gui_create_label_default("Year: ", X, y);
    editor_year_textbox = gui_create_textbox_default("Year here", X_0, y);
    editor_add_spacing(&y);

    gui_create_label_default("Average Grade: ", X, y);
    editor_average_textbox = gui_create_textbox_default("Average here", X_0, y);
    editor_add_spacing(&y);

    editor_add_student_button = gui_create_button_default((HMENU) IDC_ADD_STUDENT_BUTTON, "Add Student",
                                                          editor_on_add_student_button,
                                                          X, y);
    editor_add_spacing(&y);

    editor_edit_student_button = gui_create_button_default((HMENU) IDC_EDIT_STUDENT_BUTTON, "Edit Student",
                                                           editor_on_edit_student_button,
                                                           X, y);
    EnableWindow(editor_edit_student_button, FALSE);
    editor_add_spacing(&y);

    editor_remove_student_button = gui_create_button_default((HMENU) IDC_REMOVE_STUDENT_BUTTON, "Remove Student",
                                                             editor_on_remove_student_button,
                                                             X, y);
    EnableWindow(editor_remove_student_button, FALSE);

    student_selection_change = editor_notify_selection_changed;
}

// Function to clear input fields in the editor
void editor_clear_input() {
    gui_set_text(editor_name_textbox, "");
    gui_set_text(editor_surname_textbox, "");
    gui_set_text(editor_index_textbox, "");
    gui_set_text(editor_year_textbox, "");
    gui_set_text(editor_average_textbox, "");
}

int editor_search_by_index(const void *student_ptr, const void *key_ptr) {
    const Student *student = (const Student *) student_ptr;
    const char *key_index = (const char *) key_ptr;
    return strcmp(student->index, key_index) == 0;
}

// Button click event handler for adding a student
void editor_on_add_student_button() {
    char *name = gui_get_text(editor_name_textbox);
    char *surname = gui_get_text(editor_surname_textbox);
    char *index = gui_get_text(editor_index_textbox);
    char *year = gui_get_text(editor_year_textbox);
    char *average = gui_get_text(editor_average_textbox);

    if (name && surname && index && year && average) {
        if (student_search(editor_search_by_index, index) != -1) {
            MessageBox(hwnd, "Index already exists", "Error", MB_ICONERROR);
        } else {
            student_add_new(name, surname, index, strtol(year, NULL, 10), strtof(average, NULL));
            editor_clear_input();
        }
    }

    free(name);
    free(surname);
    free(index);
    free(year);
    free(average);

    lw_on_refresh();
}

// Button click event handler for editing a student
void editor_on_edit_student_button() {
    char *name = gui_get_text(editor_name_textbox);
    char *surname = gui_get_text(editor_surname_textbox);
    char *index = gui_get_text(editor_index_textbox);
    char *year = gui_get_text(editor_year_textbox);
    char *average = gui_get_text(editor_average_textbox);

    if (name && surname && index && year && average) {
        Student *selected = student_get_selected();
        if (!selected)return;

        strcpy(selected->name, name);
        strcpy(selected->surname, surname);
        if (student_search(editor_search_by_index, index) != -1)
            MessageBox(hwnd, "Index already exists", "Error", MB_ICONERROR);
        else
            strcpy(selected->index, index);
        selected->year = strtol(year, NULL, 10);
        selected->average = strtof(average, NULL);
    }

    free(name);
    free(surname);
    free(index);
    free(year);
    free(average);

    lw_on_refresh();
}

// Button click event handler for removing a student
void editor_on_remove_student_button() {
    EnableWindow(editor_edit_student_button, FALSE);
    EnableWindow(editor_remove_student_button, FALSE);
    Student *selected = student_get_selected();
    if (selected == NULL)return;
    editor_clear_input();
    student_remove_at(student_selected_index);
    lw_on_refresh();
}

// Function to notify selection changes and update GUI accordingly
void editor_notify_selection_changed(int previous_index, int current_index) {
    const WINBOOL ENABLE = current_index != -1;
    EnableWindow(editor_edit_student_button, ENABLE);
    EnableWindow(editor_remove_student_button, ENABLE);

    if (!ENABLE) {
        editor_clear_input();
        return;
    }
    Student *selected = student_get_selected();

    gui_set_text(editor_name_textbox, selected->name);
    gui_set_text(editor_surname_textbox, selected->surname);
    gui_set_text(editor_index_textbox, selected->index);
    char buffer[10];
    sprintf(buffer, "%d", selected->year);
    gui_set_text(editor_year_textbox, buffer);
    sprintf(buffer, "%.2f", selected->average);
    gui_set_text(editor_average_textbox, buffer);
}

#endif //CS130_PZ_C_EDITOR_H