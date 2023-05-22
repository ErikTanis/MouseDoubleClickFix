#include <Windows.h>

HHOOK g_mouse_hook = NULL;
bool g_second_click = false;
DWORD g_last_click_time = 0;
int minMs = 70;

LRESULT CALLBACK mouse_hook_callback(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && wParam == WM_LBUTTONDOWN) {
        DWORD current_time = GetTickCount();
        if (g_second_click && current_time - g_last_click_time < minMs) {
            // Second click within minMs ms, cancel it
            g_second_click = false;
            g_last_click_time = 0;
            return 1;
        }
        else {
            // First click or after minMs ms, handle normally
            g_second_click = true;
            g_last_click_time = current_time;
        }
    }
    return CallNextHookEx(g_mouse_hook, nCode, wParam, lParam);
}

int main() {
    // Install mouse hook
    HINSTANCE hInstance = GetModuleHandle(NULL);
    g_mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, mouse_hook_callback, hInstance, 0);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Uninstall mouse hook
    UnhookWindowsHookEx(g_mouse_hook);
    return 0;
}
