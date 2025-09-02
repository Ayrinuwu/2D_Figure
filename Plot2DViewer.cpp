#include <windows.h>
#include <windowsx.h>
#include "Data.h"
#include "Scene2D.h"

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);            // �������� ������� ���������
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)    // �������� ���������
{
    // ������ ������������ ����� �������� ��������� - �������� ����: ������� ����������� ������� ����� wc, ����� �������� ���� hWnd
    WNDCLASS wc;
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;            // ��� ������� ���������, ������������ �� ������ �������
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;            // ������������� ����������, ���������� ����� ������ ���������� ��� ������� ������
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(6);
    wc.lpszMenuName = 0;              // ���� � ������� ������ �����������
    wc.lpszClassName = L"MainWindowClass";  // ��� �������� ������, ������������ ��� �������� ����������� ����
    RegisterClass(&wc);                // ����������� ������ wc

    HWND hWnd = CreateWindow(            // hWnd - ����������, ���������������� ����; ������� �������� ���� ��������� ���������� hWnd ��������� ���������
        L"MainWindowClass",          // ��� �������� ������
        L"Plot2D Viewer",          // ��������� ����
        WS_OVERLAPPEDWINDOW,            // ����� ����
        200, 200, 400, 400,              // ���������� �� ������ ������ �������� ���� ����, ��� ������ � ������
        nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // ������ ������������ ����� �������� ��������� - �������� ���� ��������� ��������� ���������, ������� ������� ��������� � ��������� �� ��������������� �����
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))        // ������� GetMessage �������� �� ������� ��������� � ������� ��� � ��������� msg
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);            // ������� DispatchMessage ��������� ������� � ������������� ������ ������� ���������
    }

    return 0;
}

Scene2D scene(X0, Y0, px, py);
double V[12] = {
      1,  0,  -1,  0,
      0,  0.25,  0,  -0.25,
      1,  1,  1,  1
};
int AM[16] = {
0,  1,  0,  1,
0,  0,  1,  0,
0,  0,  0,  1,
1,  0,  1,  0
};
Matrix<double> vertices(3, 4, V);
Matrix<int> adjacencyMatrix(4, 4, AM);


Model2D model(vertices, adjacencyMatrix);
char state = 't';
LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)    // ������� ��������� ��������� � ������������ ��� ���������, ������������ ����
{
    switch (msg)
    {
    case WM_PAINT:
    {
        HDC dc = GetDC(hWnd);

        scene.Clear(dc);
        scene.addModel(model);
        scene.Render(dc);
        //scene.Clear(dc);        // ����� �������������� � ������ Camera2D ������, ����������� �� ������� ������� ������� ���� hWnd
        ///*scene.Plot(dc, Sinusoid);*/    // ����� �������������� � ������ Scene2D ������, ����������� �� ��������� ������� ���������
        //scene.PolarPlot(dc, TestPolar1);
        ///*scene.PolarPlot(dc, TestPolar2);*/
        ReleaseDC(hWnd, dc);
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    case WM_SIZE:
    {
        HDC dc = GetDC(hWnd);
        scene.SetResolution(dc);
        ReleaseDC(hWnd, dc);
        InvalidateRect(hWnd, nullptr, false);
        return 0;
    }case WM_DESTROY:
{
    PostQuitMessage(0);
    return 0;
}
case WM_LBUTTONUP:
{
    scene.EndDrag();
    return 0;
}
case WM_LBUTTONDOWN:
{
    scene.BeginDrag(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    return 0;
}
case WM_MOUSEMOVE:
{
    if (scene.IsDrag())
    {
        scene.Drag(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        InvalidateRect(hWnd, nullptr, false);
    }
    return 0;
}
case WM_KEYDOWN:
    if (state == 'm')
    {
        switch (wParam) {
        case VK_LEFT:
            model.Apply(translate(-0.05, 0));
            break;
        case VK_UP:
            model.Apply(translate(0, -0.05));
            break;
        case VK_RIGHT:
            model.Apply(translate(0.05, 0));
            break;
        case VK_DOWN:
            model.Apply(translate(0, 0.05));
            break;
        default: return 0;
        }
    }
    if (state == 't')
    {
        switch (wParam) {
        case VK_LEFT:
            model.Apply(rotate(-1));
            break;

        case VK_RIGHT:
            model.Apply(rotate(1));
            break;
        default: return 0;
        }
    }
    if (state == 's')
    {
        switch (wParam) {
        case VK_UP:
            model.Apply(scale(1, 1.25));
            break;
        case VK_LEFT:
            model.Apply(scale(0.75, 1));
            break;
        case VK_RIGHT:
            model.Apply(scale(1.25, 1));
            break;
        case VK_DOWN:
            model.Apply(scale(1, 0.75));
            break;
        default: return 0;
        }
    }
    if (state == 'r')
    {
        switch (wParam) {
        case VK_UP:
            model.Apply(scale(1, 1));
            break;
        case VK_LEFT:
            model.Apply(scale(-1, 1));
            break;
        case VK_RIGHT:
            model.Apply(scale(1, -1));
            break;
        case VK_DOWN:
            model.Apply(scale(-1, -1));
            break;
        default: return 0;
        }
    }
    scene.addModel(model);
    InvalidateRect(hWnd, nullptr, false);
    return 0;
case WM_CHAR:
{
    switch (wParam)
    {
        //turn = �������
    case 't':
    {
        state = 't';
        break;
    }

    //scaling = ��������������� ������������ ��� �������
    case 's':
    {
        state = 's';
        break;
    }
    //moving = �����������
    case 'm':
    {
        state = 'm';
        break;
    }

    //reflection = ���������
    case 'r':
    {
        state = 'r';
        break;
    }
    default: return 0;
    }
    return 0;
}

case WM_MOUSEWHEEL:
{
    POINT p;
    p.x = GET_X_LPARAM(lParam);
    p.y = GET_Y_LPARAM(lParam);
    ScreenToClient(hWnd, &p);
    scene.Scale(GET_WHEEL_DELTA_WPARAM(wParam), p);
    InvalidateRect(hWnd, nullptr, false);
    return 0;
}


default:
{
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
  }
  return 0;
}