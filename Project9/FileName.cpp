#include "Windows.h"
#include "windowsx.h"
#include "tchar.h"

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void HandleKeyUp(HWND hwnd, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WCHAR className[] = L"Window";

	WNDCLASSEX windowClass;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hIconSm = NULL;
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = className;

	windowClass.lpfnWndProc = WindowProcedure;

	if (!RegisterClassEx(&windowClass)) {
		return 0;
	}

	HWND hWnd = CreateWindowEx(
		0,
		className,
		L"Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		DispatchMessage(&message);
	}

	return message.wParam;
}

int leftClicks = 0;
int rightClicks = 0;
int middleClicks = 0;
_TCHAR messageBuffer[20]{ 0 };

int verticalPos = 0;
int horizontalPos = 0;
int windowSize = 300;

bool isFullScreen = false;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN: {
		wsprintf(messageBuffer, _T("Left Clicks: %d"), leftClicks++);
		SetWindowText(hwnd, messageBuffer);
		break;
	}
	case WM_RBUTTONDOWN: {
		wsprintf(messageBuffer, _T("Right Clicks: %d"), rightClicks++);
		SetWindowText(hwnd, messageBuffer);
		break;
	}
	case WM_MBUTTONDOWN: {
		wsprintf(messageBuffer, _T("Middle Clicks: %d"), middleClicks++);
		SetWindowText(hwnd, messageBuffer);
		break;
	}

	case WM_KEYUP: {
		HandleKeyUp(hwnd, wParam, lParam);
		break;
	}
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	switch (wParam) {
	case VK_UP: {
		if (isFullScreen && horizontalPos != 0)
			SetWindowPos(hwnd, NULL, verticalPos, horizontalPos -= 10, windowSize, windowSize, SWP_NOZORDER);
		break;
	}
	case VK_RIGHT: {
		if (isFullScreen && verticalPos != (GetSystemMetrics(SM_CXSCREEN) - windowSize))
			SetWindowPos(hwnd, NULL, verticalPos += 10, horizontalPos, windowSize, windowSize, SWP_NOZORDER);
		break;
	}
	case VK_DOWN: {
		if (isFullScreen && horizontalPos != (GetSystemMetrics(SM_CYSCREEN) - windowSize) - 40)
			SetWindowPos(hwnd, NULL, verticalPos, horizontalPos += 10, windowSize, windowSize, SWP_NOZORDER);
		break;
	}
	case VK_LEFT: {
		if (isFullScreen && verticalPos != 0)
			SetWindowPos(hwnd, NULL, verticalPos -= 10, horizontalPos, windowSize, windowSize, SWP_NOZORDER);
		break;
	}
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

void HandleKeyUp(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	if (wParam == VK_RETURN) {
		SetWindowPos(hwnd, NULL, 0, 0, windowSize, windowSize, SWP_NOZORDER);
		verticalPos = 0;
		horizontalPos = 0;
		isFullScreen = true;
	}
}
