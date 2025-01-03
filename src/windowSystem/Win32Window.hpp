#pragma once

#include "WindowInterface.hpp"
#include <windows.h>

class Win32Window : public WindowInterface {
public:
    Win32Window(EventDispatcher& eventDispatcher, int width, int height):WindowInterface(eventDispatcher, width, height){
        createWindow();
    }
    void createWindow() override {
        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = "Win32WindowClass";
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // Load a default arrow cursor

        RegisterClass(&wc);

        hwnd = CreateWindowEx(
            0,                      // Optional window styles
            "Win32WindowClass",     // Window class
            title,                  // Window title
            WS_OVERLAPPEDWINDOW,    // Window style
            CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            NULL,                   // Parent window    
            NULL,                   // Menu
            GetModuleHandle(NULL),  // Instance handle
            NULL                    // Additional application data
        );

        ShowWindow(hwnd, SW_SHOW);
    }

    bool handleEvents() override {
        bool running = true;
        MSG msg = {};
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //this can probably be handled better but i only want to send 1 event per game loop
        if((eventResize.newWidth!= width) || (eventResize.newHeight!=height)){
            width = eventResize.newWidth;
            height = eventResize.newHeight;
            eventDispatcher.enqueue(&eventResize);
        }
        return running;
    }

    void closeWindow() override {
        DestroyWindow(hwnd);
    }
private:
    HWND hwnd;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            eventResize.newWidth = LOWORD(lParam);
            eventResize.newHeight = HIWORD(lParam);
            break;
        default: 
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        return 0;
    }
};


