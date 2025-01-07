#pragma once


#include "../WindowInterface.hpp"

#include <windows.h>

namespace ExhibitEngine{

    class Win32Window : public WindowInterface {
    public:
        Win32Window(EventDispatcher& eventDispatcher, int32 width, int32 height);
        ~Win32Window();

        void createWindow(int32 width, int32 height) override;

        bool processEvents() override;

        void closeWindow() override;
    private:
        HWND hwnd;
        HINSTANCE hInstance;

        static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        
        LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    };


}