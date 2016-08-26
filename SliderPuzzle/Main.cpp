#include <Windows.h>
#include <windowsx.h>
#include <memory>
#include <assert.h>
#include "Game.h"

static HWND WindowInit(HINSTANCE instance, const wchar_t* class_name, int32_t width, int32_t height);
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

static MouseState mouseState{};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    HWND window = WindowInit(instance, L"SliderPuzzle", 1280, 720);
    if (!window)
    {
        return -1;
    }

    std::shared_ptr<Game> game(new Game());
    if (!game->Init(window))
    {
        return -2;
    }

    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);



    MSG msg{};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if (!game->Update(mouseState))
            {
                break;
            }
            mouseState.clicked = false;
            game->Draw();
        }
    }

    DestroyWindow(window);
    return 0;
}

HWND WindowInit(HINSTANCE instance, const wchar_t* class_name, int32_t width, int32_t height)
{
    WNDCLASSEX wcx{};
    wcx.cbSize = sizeof(wcx);
    wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcx.hInstance = instance;
    wcx.lpfnWndProc = WindowProc;
    wcx.lpszClassName = class_name;
    if (RegisterClassEx(&wcx) == INVALID_ATOM)
    {
        assert(false);
        return nullptr;
    }

    DWORD style = WS_OVERLAPPEDWINDOW;

    RECT rc{};
    rc.right = width;
    rc.bottom = height;
    AdjustWindowRect(&rc, style, FALSE);

    HWND hwnd = CreateWindow(wcx.lpszClassName, wcx.lpszClassName, style, CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, instance, nullptr);
    if (!hwnd)
    {
        assert(false);
        return nullptr;
    }

    return hwnd;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
        }
        break;

    case WM_LBUTTONUP:
        mouseState.clicked = true;
        mouseState.x = GET_X_LPARAM(lParam);
        mouseState.y = GET_Y_LPARAM(lParam);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
