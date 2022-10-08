#include "Window.h"
#include "EngineTime.h"
#include <exception>

//Window* window = nullptr;

// Calls the events of our window (creation and destroy events)
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//GetWindowLong(hwnd,)
	switch (msg)
	{
	case WM_CREATE:
	{
		// Event fired when the window is created
		// collected here..

		break;
	}

	case WM_SETFOCUS:
	{
		// Event fired when the window get focus
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window) window->onFocus();
		break;
	}

	case WM_KILLFOCUS:
	{
		// Event fired when the window lost focus
		Window* window = (Window*)GetWindowLong(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}

	case WM_DESTROY:
	{
		// Event fired when the window is destroyed
		Window* window = (Window*)GetWindowLong(hwnd, GWLP_USERDATA);
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}
	// recall this function to get another response (recursive)
	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

Window::Window()
{
	// Set appearance for our windows; Setting up WNDCLASSEX object
	// WNDCLASSEX contains the properties of the window
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	// L == (wchar_t*); an optimize typecasting/conversion
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;
	// registers the customized properties we've set on our window
	// If the registration of class will fail, the function will return false
	if (!::RegisterClassEx(&wc))
		throw std::exception("Window not created successfully");

	/*if (!window)
		window = this;*/

		/* 12 Parameters
		 * 1 - WS_EX_OVERLAPPEDWINDOW
		 * 2 - Our lpszClassName we've set
		 * 3 - Title of our window
		 * 4 - Style flags (dw style)
		 * 5 - Position(X) of our window (default)
		 * 6 - Position(Y) of our window (default)
		 * 7 - Width of our window
		 * 8 - Height of our window
		 * 9 - hWndParent reference
		 * 10 - hMenu reference
		 * 11 - hInstance reference
		 * 12 - lpParam reference
		 */
		 //Creation of the window; HWND - "Handle to a Window"
	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectX Application",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
		NULL, NULL, NULL, NULL);

	// if the creation fail return false
	if (!m_hwnd)
		throw std::exception("Window not created successfully");

	// show up the window
	::ShowWindow(m_hwnd, SW_SHOW);
	// Allow us to redraw the content of our window
	::UpdateWindow(m_hwnd);

	// set this flag to true to indicate that the window is initialized and running
	m_is_run = true;
	

}

Window::~Window()
{

}


bool Window::broadcast()
{
	MSG msg;

	// first time to initialize
	if (!m_is_init)
	{
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		this->onCreate();
		m_is_init = true;
		EngineTime::initialize();
	}

	EngineTime::LogFrameStart();
	// renders all the frames of the graphic 'C'
	this->onUpdate();

	// creates message based on the actions done on the windows that will be pass then to the 'WNDPROC'
	// message was received from the queue of the operating system
	while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		// translate msg into string / character messages / value of the window messages macro
		TranslateMessage(&msg);
		// gives the returned translated message to the WNDPROC
		DispatchMessage(&msg);
	}

	// doesn't allow the CPU to throttle due to chunk of processes;
	// allows a minimal pause of '1' Msec so that the CPU can handle the loop
	Sleep(1);
	EngineTime::LogFrameEnd();

	return true;
}

bool Window::isRun()
{
	if (m_is_run)
		broadcast();
	return m_is_run;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

void Window::onCreate()
{
	
}

void Window::onUpdate()
{
	
}

void Window::onDestroy()
{
	m_is_run = false;
}

void Window::onFocus()
{
}

void Window::onKillFocus()
{
}
