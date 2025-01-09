#include "../WindowInterface.hpp"

namespace ExhibitEngine{


    Window::Window(EventDispatcher& eventDispatcher, int32 width, int32 height):
        WindowInterface(eventDispatcher),
        hwnd(nullptr),
        hInstance(GetModuleHandle(nullptr)){
        createWindow(width, height);
    }

    Window::~Window(){
        DestroyWindow(hwnd);
    }

    void Window::createWindow(int32 width, int32 height){
        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProcStatic;
        wc.hInstance = hInstance;
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
            hInstance,              // Instance handle
            this                    // Additional application data
        );

        ShowWindow(hwnd, SW_SHOW);
    }

    bool Window::processEvents(){
            MSG msg = {};
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg); // Generate additional messages like WM_CHAR
                DispatchMessage(&msg);  // Dispatch to the window procedure

                if (msg.message == WM_QUIT) { // Check for application close
                    running = false;
                }
            }
            //this can probably be handled better but i only want to send 1 event per game loop
            if(beenResized){
                beenResized = false;

                RECT rect;
                GetClientRect(hwnd, &rect);
                eventDispatcher.enqueue(EventResize(rect.right - rect.left, rect.bottom - rect.top));
            }
            return running;
    }

    void Window::closeWindow(){
        DestroyWindow(hwnd);
    }

    LRESULT CALLBACK Window::WindowProcStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
            Window* pThis = nullptr;

            if (uMsg == WM_NCCREATE) {
                CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
                pThis = static_cast<Window*>(pCreate->lpCreateParams);
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

                pThis->hwnd = hwnd;
            } else {
                pThis = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            }

            if (pThis) {
                return pThis->WindowProc(uMsg, wParam, lParam);
            }

            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }


    LRESULT Window::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam){
        switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            beenResized = true;
            break;
        default: 
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        return 0;
    }

}