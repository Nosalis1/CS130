//
// Created by Aleksa on 2/24/2024.
//

#ifndef CS130_PZ_C_GUI_H
#define CS130_PZ_C_GUI_H

#include <windows.h>
#include <commctrl.h>

LRESULT CALLBACK window_proc(HWND, UINT, WPARAM, LPARAM);

#define PADDING 10
#define SPACING 10

#define MAX_BUTTON_COUNT 10

#define COMPONENT_WIDTH 100
#define COMPONENT_HEIGHT 17

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

const char TITLE[] = "IMet";
const char CLASS_NAME[] = "Sample Window Class";

HWND hwnd;
HINSTANCE hInstance;

#pragma region BUTTON_HANDLER

typedef void (*ButtonClickHandler)();

typedef struct {
    HMENU index;
    ButtonClickHandler handler;
} ButtonClick;
ButtonClick *buttons_events = NULL;
int buttons_events_size = 0;
#pragma endregion BUTTON_HANDLER

void gui_init_components();

BOOL gui_init() {
    WNDCLASS wc = {};

    wc.lpfnWndProc = window_proc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
            0,
            CLASS_NAME,
            TITLE,
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            0, 0,
            hInstance, 0);

    if (!hwnd) {
        return FALSE;
    }

#ifdef INIT_COMMON_CONTROLS
    INITCOMMONCONTROLSEX i_cex;
    i_cex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    i_cex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&i_cex);
#endif //INIT_COMMON_CONTROLS

    buttons_events = malloc(sizeof(ButtonClick) * MAX_BUTTON_COUNT);
    buttons_events_size = 0;

    gui_init_components();

    return TRUE;
}

#pragma region BUTTON_UTILS

__attribute__((unused)) HWND
gui_create_button(HMENU id, char *text, ButtonClickHandler handler, int x, int y, int width, int height) {
    if (buttons_events_size + 1 >= MAX_BUTTON_COUNT) return NULL;
    buttons_events[buttons_events_size].handler = handler;
    buttons_events[buttons_events_size].index = id;
    buttons_events_size++;
    return CreateWindow("BUTTON", text, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
                        x, y, width, height,
                        hwnd, id, hInstance, NULL);
}

__attribute__((unused)) HWND gui_create_button_default(HMENU id, char *text, ButtonClickHandler handler, int x, int y) {
    if (buttons_events_size + 1 >= MAX_BUTTON_COUNT) return NULL;
    buttons_events[buttons_events_size].handler = handler;
    buttons_events[buttons_events_size].index = id;
    buttons_events_size++;
    return CreateWindow("BUTTON", text, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
                        x, y, COMPONENT_WIDTH, COMPONENT_HEIGHT,
                        hwnd, id, hInstance, NULL);
}

#pragma endregion BUTTON_UTILS

#pragma region LABEL_UTILS

__attribute__((unused)) HWND gui_create_label(char *text, int x, int y, int width, int height) {
    return CreateWindow("STATIC", text, WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOPREFIX,
                        x, y, width, height,
                        hwnd, NULL, hInstance, NULL);
}

__attribute__((unused)) HWND gui_create_label_default(char *text, int x, int y) {
    return CreateWindow("STATIC", text, WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOPREFIX,
                        x, y, COMPONENT_WIDTH, COMPONENT_HEIGHT,
                        hwnd, NULL, hInstance, NULL);
}

#pragma endregion LABEL_UTILS

#pragma region TEXTBOX_UTILS

__attribute__((unused)) HWND gui_create_textbox(char *text, int x, int y, int width, int height) {
    return CreateWindow("EDIT", text, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
                        x, y, width, height,
                        hwnd, NULL, hInstance, NULL);
}

__attribute__((unused)) HWND gui_create_textbox_default(char *text, int x, int y) {
    return CreateWindow("EDIT", text, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
                        x, y, COMPONENT_WIDTH, COMPONENT_HEIGHT,
                        hwnd, NULL, hInstance, NULL);
}

__attribute__((unused)) void gui_set_text(HWND w, char *text) {
    SetWindowText(w, text);
}

char *gui_get_text(HWND w) {
    int size = GetWindowTextLength(w) + 1;
    char *text = malloc(sizeof(char) * size);
    GetWindowText(w, text, size);
    return text;
}

#pragma endregion TEXTBOX_UTILS

#pragma region FILE_DIALOG

BOOL gui_open_file_dialog(HWND parent, LPWSTR fileName, DWORD fileNameSize) {
    OPENFILENAME ofn;
    WCHAR szFile[fileNameSize];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = parent;
    ofn.lpstrFile = (LPSTR) szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = fileNameSize;
    ofn.lpstrFilter = "Data Files\0*.data\0"; // "All Files\0*.*\0Text Files\0*.txt\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_CREATEPROMPT;

    if (GetOpenFileName(&ofn) == TRUE) {
        wcscpy_s(fileName, fileNameSize, (const wchar_t *) ofn.lpstrFile);
        return TRUE;
    } else {
        return FALSE;
    }
}

#pragma endregion FILE_DIALOG

void gui_on_notify(HWND _hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

void gui_on_destroy(HWND _hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT CALLBACK window_proc(HWND _hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_NOTIFY:
            gui_on_notify(_hwnd, msg, wparam, lparam);
            break;
        case WM_COMMAND:
            for (int i = 0; i < buttons_events_size; i++)
                if (buttons_events[i].index == LOWORD(wparam))
                    buttons_events[i].handler();
            break;
        case WM_CLOSE:
            DestroyWindow(_hwnd);
            break;
        case WM_DESTROY:
            gui_on_destroy(_hwnd, msg, wparam, lparam);
            break;
        default:
            return DefWindowProc(_hwnd, msg, wparam, lparam);
    }
    return 0;
}

#endif //CS130_PZ_C_GUI_H