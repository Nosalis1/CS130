#include <stdio.h>
// #define INIT_COMMON_CONTROLS // Optional (Not required on Windows 10)
#include "gui.h"
#include "student.h"
#include "listView.h"
#include "editor.h"
#include "files.h"

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    hInstance = _hInstance;
    if (!gui_init()) {
        printf("Failed to initialize GUI\n");
        return 1;
    }
    ShowWindow(hwnd, nShowCmd);
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

void gui_init_components() {
    student_init_array();
    lw_init();
    files_init();
    editor_init();
}

void gui_on_notify(HWND _hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (((NMHDR *) lparam)->code == LVN_ITEMCHANGED) {
        NMLISTVIEW *p_nvm_l = (NMLISTVIEW *) lparam;
        if ((p_nvm_l->uChanged & LVIF_STATE) &&
            ((p_nvm_l->uNewState & LVIS_SELECTED) != (p_nvm_l->uOldState & LVIS_SELECTED))) {
            student_selection_change_callback(p_nvm_l->iItem);
        }
    }
}

void gui_on_destroy(HWND _hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    student_free_array();
    free(buttons_events);
    printf("Application closed\n");
    PostQuitMessage(0);
}