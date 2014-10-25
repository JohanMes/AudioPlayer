#include <windows.h>
#include <stdio.h>
#include "resource.h"

HWND PrevBtn = NULL;
HWND PlayBtn = NULL;
HWND PauseBtn = NULL;
HWND NextBtn = NULL;

HBRUSH redbrush = NULL;
HBRUSH greenbrush = NULL;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_CREATE: {
			
			// Create an audio player which manages its own GUI parts
			audioplayer = new AudioPlayer(hwnd); // invalidates bg of hwnd when needed
			
			// Temporary list of fancy audio
			audioplayer->AddToPlaylist("Data\\001.mp3");
			audioplayer->AddToPlaylist("Data\\002.mp3");
			audioplayer->AddToPlaylist("Data\\02 - Eruption.mp3");
			audioplayer->AddToPlaylist("Data\\Windows Battery Critical.wav");
			
			// Temporary buttons that will send the message for us
			PrevBtn = CreateWindow(
				"BUTTON","Prev",WS_CHILD|WS_VISIBLE,5,20,80,30,hwnd,(HMENU)ID_PREV,GetModuleHandle(NULL),NULL);			
			PlayBtn = CreateWindow(
				"BUTTON","Play",WS_CHILD|WS_VISIBLE,90,5,80,30,hwnd,(HMENU)ID_PLAY,GetModuleHandle(NULL),NULL);
			PauseBtn = CreateWindow(
				"BUTTON","Pause",WS_CHILD|WS_VISIBLE,90,40,80,30,hwnd,(HMENU)ID_PAUSE,GetModuleHandle(NULL),NULL);
			NextBtn = CreateWindow(
				"BUTTON","Next",WS_CHILD|WS_VISIBLE,175,20,80,30,hwnd,(HMENU)ID_NEXT,GetModuleHandle(NULL),NULL);
				
			// Key colors
			redbrush = CreateSolidBrush(RGB(255, 0, 0));
			greenbrush = CreateSolidBrush(RGB(0, 255, 0));
			break;
		}
		case WM_DESTROY: {
			DeleteObject(redbrush);
			DeleteObject(greenbrush);
			PostQuitMessage(0);
			break;
		}
		case WM_COMMAND: {
			switch(LOWORD(wParam)) {
				case ID_PREV: {
					audioplayer->Prev();
					break;
				}
				case ID_PLAY: {
					audioplayer->Play();
					break;
				}
				case ID_PAUSE: {
					audioplayer->Pause();
					break;
				}
				case ID_NEXT: {
					audioplayer->Next();
					break;
				}
			}
			break;
		}
		case WM_ERASEBKGND: {
			if(audioplayer) {
				HDC hdc = (HDC)wParam;
				RECT clientrect;
				GetClientRect(hwnd,&clientrect);
				if(audioplayer->IsPlaying()) {
					FillRect(hdc,&clientrect,greenbrush);
				} else {
					FillRect(hdc,&clientrect,redbrush);
				}
				
				// Returning 1 means we're erasing by ourselves
				return 1;
			}
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/*
	- Global key 1, pause/play toggle
	- Global key 2, delete playlist
	- Achtergrond rood als niet spelen, groen spelen
	- Geen border misschien, vaag gedoe
*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* insert window procedure function here */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* use "A" as icon name when you want to use the project icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* as above */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	// Set up a window with this many usable pixels
	RECT result = {0,0,260,75};
	DWORD style = WS_VISIBLE|WS_POPUP;
	AdjustWindowRect(&result,style,false);

	hwnd = CreateWindow("WindowClass","Caption",style,
		100, // x
		100, // y
		result.right - result.left, // width
		result.bottom - result.top, // height
		NULL,NULL,hInstance,NULL);
	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	while(GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
