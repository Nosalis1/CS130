//
// Created by Aleksa on 2/24/2024.
//

// Header guards to prevent multiple inclusions of the header file
#ifndef CS130_PZ_C_FILES_H
#define CS130_PZ_C_FILES_H

// Define identifiers for various file-related GUI elements
// Declare global variables for file path and GUI element handles
#define IDC_OPEN_BUTTON 1002
HWND files_open_button;
#define IDC_SAVE_BUTTON 1003
HWND files_save_button;
#define IDC_SAVE_AS_BUTTON 1004
HWND files_save_as_button;

WCHAR files_file_path[MAX_PATH] = L"";

// Function prototypes for file-related button click event handlers
// Button click event handler for opening a file
void files_on_file_open();

// Button click event handler for saving a file
void files_on_file_save();

// Button click event handler for saving a file with a new name
void files_on_file_save_as();

// Function to initialize file-related GUI elements
void files_init() {
    RECT rect;
    GetClientRect(hwnd, &rect);

    const int padding = 3;

    const RECT ob_rect = {
            rect.right / 2 + padding,
            padding,
            rect.right / 2 + padding + COMPONENT_WIDTH,
            padding + 30
    };
    const RECT sb_rect = {
            ob_rect.right + padding,
            padding,
            ob_rect.right + padding + COMPONENT_WIDTH,
            padding + 30
    };
    const RECT sbs_rect = {
            sb_rect.right + padding,
            padding,
            sb_rect.right + padding + COMPONENT_WIDTH,
            padding + 30
    };

    files_open_button = gui_create_button((HMENU) IDC_OPEN_BUTTON, "Open", files_on_file_open,
                                          ob_rect.left, ob_rect.top,
                                          ob_rect.right - ob_rect.left,
                                          ob_rect.bottom - ob_rect.top);
    files_save_button = gui_create_button((HMENU) IDC_SAVE_BUTTON, "Save", files_on_file_save,
                                          sb_rect.left, sb_rect.top,
                                          sb_rect.right - sb_rect.left,
                                          sb_rect.bottom - sb_rect.top);
    EnableWindow(files_save_button, FALSE);
    files_save_as_button = gui_create_button((HMENU) IDC_SAVE_AS_BUTTON, "Save as", files_on_file_save_as,
                                             sbs_rect.left, sbs_rect.top,
                                             sbs_rect.right - sbs_rect.left,
                                             sbs_rect.bottom - sbs_rect.top);
    EnableWindow(files_save_as_button, FALSE);
}

// Function to handle file opening
void files_on_file_open() {
    if (!gui_open_file_dialog(hwnd, files_file_path, MAX_PATH)) return;

    FILE *file = fopen((const char *) files_file_path, "rb");
    if (file == NULL) {
        MessageBox(hwnd, "Failed to open file", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    student_init_array();

    size_t length = fread(students, sizeof(Student), MAX_STUDENT_COUNT, file);
    student_count = (int) length;

    fclose(file);

    EnableWindow(files_save_button, TRUE);
    EnableWindow(files_save_as_button, TRUE);

    lw_on_refresh();
}

// Function to handle file saving
void files_on_file_save() {
    FILE *file = fopen((const char *) files_file_path, "wb");
    if (file == NULL) {
        MessageBox(hwnd, "Failed to open file", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    fwrite(students, sizeof(Student), student_count, file);
    fclose(file);
}

// Function to handle file saving with a new name
void files_on_file_save_as() {
    if (!gui_open_file_dialog(hwnd, files_file_path, MAX_PATH)) return;
    FILE *file = fopen((const char *) files_file_path, "wb");
    if (file == NULL) {
        MessageBox(hwnd, "Failed to open file", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    fwrite(students, sizeof(Student), student_count, file);
    fclose(file);
}

#endif //CS130_PZ_C_FILES_H