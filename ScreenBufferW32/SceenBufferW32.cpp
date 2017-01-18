/***********************************************************************************\
 *                                                                                 *
 * ScreenBuffer is extremely simple screen library.                                *
 *                                                                                 *
 * Copyright (C) 2003  Andrew.W Worobow                                            *
 *                            email:worobow@hotmail.com                            *
 *                                                                                 *
 * This program is free software; you can redistribute it and / or modify          *
 * it under the terms of the GNU General Public License as published by            *
 * the Free Software Foundation; either version 3 of the License, or               *
 * any later version.                                                              *
 *                                                                                 *
 * This program is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                  *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the                     *
 * GNU General Public License for more details.                                    *
 *                                                                                 *
 * You should have received a copy of the GNU General Public License               *
 * along with this program; if not, write to the Free Software Foundation,         *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301  USA             *
 *                                                                                 *
 *                                                                                 *
 * This library is designed to display information on the screen                   *
 * of the console applications with minimal overhead.                              *
 *                                                                                 *
\***********************************************************************************/


#include <Windows.h>

#include <wchar.h>
#include <stdio.h>
#include <conio.h>

typedef void (*SCREENBUFFER_CALLBACK) ( void );

extern "C" int  __cdecl SBInit  ( SCREENBUFFER_CALLBACK Updater );
extern "C" void __cdecl SBClose ( void );
extern "C" void __cdecl SBputsW ( size_t x, size_t y, int c, wchar_t * str, size_t ls );
extern "C" void __cdecl SBclrs  ( size_t x, size_t y, int c, size_t ls );

ULONG SCBW32init = 0;
UINT_PTR SCBtmr;

TEXTMETRIC SCBtm;
HFONT SCBhfnt;

DWORD SCBthId;

HDC SCBmdc;
HBITMAP SCBbmp;
HWND SCBwnd; 
WNDCLASSEX SCBwcx;

ULONG SCBW32ch=1;

RECT SCBW32rect={0,0,0,0};

wchar_t SCBW32b[25][80]; //bytes
DWORD  SCBW32c[25][80];//col

extern "C" wchar_t * SCBWTitle = L"sample";

HBRUSH WHhbr;

SCREENBUFFER_CALLBACK lpUpDater=NULL;

VOID CALLBACK SCBTimerProc ( HWND hwnd,  UINT uMsg, UINT_PTR idEvent,  DWORD dwTime )
{
    int s;

    RECT r;
    
    if ( !SCBW32ch )
        return;

    for(s=0;s<25;s++)
        TextOut(SCBmdc,0,s*SCBtm.tmHeight,(LPCWSTR)&SCBW32b[s][0],80);

    r.top=SCBW32rect.top*SCBtm.tmHeight;
    r.bottom=SCBW32rect.bottom*SCBtm.tmHeight;
    r.left=SCBW32rect.left*(SCBtm.tmAveCharWidth+1);
    r.right=SCBW32rect.right*(SCBtm.tmAveCharWidth+1);
    
    FrameRect(  SCBmdc,&r,WHhbr);
    InvalidateRect(SCBwnd,&r,0);

    SCBW32ch=0;
    SCBW32rect.left=1000;
    SCBW32rect.top=1000;
    SCBW32rect.right=0;
    SCBW32rect.bottom=0;

    if ( lpUpDater ) 
        (*lpUpDater)();
}

LRESULT CALLBACK MainWndProc ( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch (uMsg){ 

        case WM_COMMAND: 
            switch(LOWORD(wParam)){ 
                case 99: 
                    
                    break; 
                case 98: 
                    
                    break; 
                default: 
                    break; 
            } 
            return 0; 
        case WM_PAINT:
            hdc=BeginPaint(hwnd,&ps);
            
            BitBlt( hdc,0,0,(SCBtm.tmAveCharWidth+1)*80,SCBtm.tmHeight*25, SCBmdc, 0,0,SRCCOPY );////MERGECOPY
            EndPaint(hwnd,&ps);
            return NULL; 
        default: 
            return DefWindowProc(hwnd, uMsg, wParam, lParam); 
    } 
    return NULL; 

}

DWORD WINAPI SCBthr (  LPVOID _lpUpDater )
{
    MSG msg;
    HDC hdc;
    int br;

     // Fill in the window class structure with parameters 
    // that describe the main window. 
 
    SCBwcx.cbSize = sizeof(SCBwcx);          // size of structure 
    SCBwcx.style = CS_HREDRAW | CS_VREDRAW;                    // redraw if size changes 
    SCBwcx.lpfnWndProc = MainWndProc;     // points to window procedure 
    SCBwcx.cbClsExtra = 0;                // no extra class memory 
    SCBwcx.cbWndExtra = 0;                // no extra window memory 
    SCBwcx.hInstance = GetModuleHandle(0);// hinstance;         // handle to instance 

//	HICON jjj=LoadCursorFromFile(L"SCBW32.ico");

    SCBwcx.hIcon = (HICON)LoadImage(  0,L"SCBW32.ico",IMAGE_ICON,0,0,LR_LOADFROMFILE|LR_LOADTRANSPARENT);              // predefined app. icon 
    SCBwcx.hCursor = (HCURSOR)LoadImage(  0,L"SCBW32.cur",IMAGE_CURSOR,0,0,LR_LOADFROMFILE|LR_LOADTRANSPARENT);                    // predefined arrow 
    SCBwcx.hbrBackground = (HBRUSH)GetStockObject(   BLACK_BRUSH);                  // white background brush 
    SCBwcx.lpszMenuName = 0;// L"MainMenu";    // name of menu resource 
    SCBwcx.lpszClassName = L"SCBW32Class";  // name of window class 
    SCBwcx.hIconSm = (HICON)LoadImage(  0,L"SCBW32s.ico",IMAGE_ICON,0,0,LR_LOADFROMFILE|LR_LOADTRANSPARENT);// small class icon 
 
    RegisterClassEx(&SCBwcx); 

    // Save the application-instance handle. 
  
    // Create the main window. 

 
    SCBwnd = CreateWindow( 
        L"SCBW32Class",      // name of window class 
        //L"Sample",           // title-bar string 
        SCBWTitle,
        WS_OVERLAPPEDWINDOW, // top-level window 
        CW_USEDEFAULT,       // default horizontal position 
        CW_USEDEFAULT,       // default vertical position 
        CW_USEDEFAULT,       // default width 
        CW_USEDEFAULT,       // default height 
        (HWND) NULL,         // no owner window 
        (HMENU) NULL,        // use class menu 
         SCBwcx.hInstance,      // handle to application instance 
        (LPVOID) NULL);      // no window-creation data 
 
    if (!SCBwnd) 
        return FALSE; 
 
    // Show the window and send a WM_PAINT message to the window 
    // procedure. 
 
    ShowWindow(SCBwnd, SW_HIDE); 
    hdc=GetDC(SCBwnd);
    SCBmdc = CreateCompatibleDC(hdc); 

    SCBhfnt = (HFONT)GetStockObject(SYSTEM_FIXED_FONT); 
    GetTextMetrics(SCBmdc,&SCBtm);
    
    SCBbmp = CreateCompatibleBitmap(hdc,(SCBtm.tmAveCharWidth+1)*80, SCBtm.tmHeight*25 ); 
    
    DeleteDC(hdc);

    
    SelectObject(SCBmdc, SCBhfnt);
    
    SelectObject ( SCBmdc, SCBbmp );

    SetTextColor(SCBmdc,RGB(127,127,127));
    SetBkColor(SCBmdc,RGB(15,15,15));

    lpUpDater = (SCREENBUFFER_CALLBACK) _lpUpDater;
    
    SCBtmr=SetTimer(0,0,1000/60, (TIMERPROC) SCBTimerProc);

    wmemset(&SCBW32b[0][0],L'.',sizeof(SCBW32b)/sizeof(SCBW32b[0][0]));

    WHhbr=(HBRUSH)GetStockObject(WHITE_BRUSH);
    
    SCBTimerProc ( SCBwnd, 0, 0, 0 );

    MoveWindow( SCBwnd, 0,   0,    (SCBtm.tmAveCharWidth+1)*80+5,    (SCBtm.tmHeight+1)*25+5,  FALSE );
    ShowWindow(SCBwnd, SW_SHOW); 
    UpdateWindow(SCBwnd); 

    while( ( br = GetMessage( &msg, 0, 0, 0 )) != 0){ 
        SCBW32init=1;
        if ( br == -1 ){
            break;//error
        }
        else{
            TranslateMessage(&msg); 
            DispatchMessage(&msg); 
        }
    } 
    KillTimer(0,SCBtmr);
    DeleteObject(SCBhfnt);
    DeleteObject(SCBbmp);
    DeleteObject(WHhbr);
    DeleteDC(SCBmdc);
    UnregisterClass(L"SCBW32Class",0);
    DestroyWindow(SCBwnd);

    SCBW32init=0;

    return 0;
}


int __cdecl SBInit ( SCREENBUFFER_CALLBACK updater )
{
    HANDLE th;
    th=CreateThread(0,0,SCBthr,updater,0,&SCBthId);//
    CloseHandle(th);
    while ( !SCBW32init )
        Sleep(1);
    
    return 0;

}
void __cdecl SBClose ( void )
{
    PostThreadMessage(SCBthId, WM_QUIT,0,0);
    while ( SCBW32init )
        Sleep(1);
}


#define CAS(m,n,o) (InterlockedCompareExchange(m,n,o) == o )

void callmaxs ( LONG x, LONG y, LONG mx, LONG my )
{
    LONG o;
    
    while(1){
        o=SCBW32rect.right;
        if ( o < my ){
            if ( CAS(&SCBW32rect.right,my,o) )
                break;
        }
        break;
    }
    while ( 1 ){
        o=SCBW32rect.bottom;
        if ( o < mx)
            if ( CAS(&SCBW32rect.bottom,mx,o) )
                break;
        break;
    }
    while ( 1 ){
        o=SCBW32rect.left;
        if ( o > y)
            if ( CAS(&SCBW32rect.left,y,o) )
                break;
        break;
    }
    while ( 1 ){
        o=SCBW32rect.top;
        if ( o > x )
            if ( CAS(&SCBW32rect.top,x,o) )
                break;
        break;
    }
}

void __cdecl SBputsW ( size_t x, size_t y, int c, wchar_t * str, size_t ls )
{
    LONG cx,cy,mx,my;

    wmemcpy(&SCBW32b[x][y],str,ls);

    cx = (LONG)x;
    cy = (LONG)y;
    mx=cx+1;
    my=cy+(LONG)ls;
    
    callmaxs(cx,cy,mx,my);

    SCBW32ch=1;
}
void __cdecl SBclrs ( size_t x, size_t y, int c, size_t ls )
{
    int cx,cy,mx,my;

    wmemset(&SCBW32b[x][y],' ',ls);

    cx = (LONG)x;
    cy = (LONG)y;
    mx=cx+1;
    my=cy+(LONG)ls;
    
    callmaxs(cx,cy,mx,my);

    SCBW32ch=1;
}




