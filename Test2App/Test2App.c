
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


#include <windows.h>
#include <time.h>
#include <stdio.h>
#include "ScreenBufferW32.h"

int TestCaWithoutScreen ( size_t loop_count )
{
	size_t j = loop_count;
	int Var;

	while ( j-- ){
		// demo for set value
		Var = rand ( );
	}

	return Var;
}

int TestCaWithScreenAndStat ( size_t loop_count )
{
	// string to display
	wchar_t strin[]  = L">>>>>>> X <<<<<<<<";
	wchar_t strout[] = L"<<<<<<< X >>>>>>>>";

	size_t strlength = _countof (strin );

	wchar_t * const casestr[2] = {strin,strout};

	size_t j = loop_count;

	//chars for roll  
	const wchar_t c[] = L"\\|/-";

	int Var;

	wchar_t * str;

	// The place where we gather statistics on the variable 'Var'
	// and zero init all fields.
	STAT mst = { 0 };
	// set cut-off level.
	mst.drop = INT_MAX/5;

	while ( j-- ){
		str = casestr[j%2];
		// demo some display outputs
		str[8] = c[j % 4];
		SBputsW ( 10, 40, RGB ( 127, 100, 100 ), str, strlength );

		// demo for set value
		Var = rand ( );
		// demo statistic accumulate
		AccumStats ( &mst, Var );
		// demo statistic display
		if ( !( j % 100000 ) ){
			ShowStat ( 1, 1, L"rand", 4, &mst );
		}
	}
	
	return Var;
}

int TestCaWithScreenAndWithoutStat ( size_t loop_count )
{
	// string to display
	wchar_t strin[] = L">>>>>>> X <<<<<<<<";
	wchar_t strout[] = L"<<<<<<< X >>>>>>>>";

	size_t strlength = _countof ( strin );

	wchar_t * const casestr[2] = { strin,strout };

	size_t j = loop_count;

	//chars for roll  
	const wchar_t c[] = L"\\|/-";

	int Var;

	wchar_t * str;

	while ( j-- ){
		str = casestr[j % 2];
		// demo some display outputs
		str[8] = c[j % 4];
		SBputsW ( 10, 40, RGB ( 127, 100, 100 ), str, strlength );

		// demo for set value
		Var = rand ( );
	}

	return Var;
}

//#define DEMO_STD_CONSOLE

#ifdef DEMO_STD_CONSOLE
HANDLE hNewScreenBuffer;
HANDLE hStdout;

CHAR_INFO chiBuffer[SCREEN_WIDE];// temp buffer for string
COORD coordBufSize;
COORD coordBufCoord;

SMALL_RECT srctWriteRect;

void InitStdScreen ( void )
{

	hStdout = GetStdHandle ( STD_OUTPUT_HANDLE );

	hNewScreenBuffer = CreateConsoleScreenBuffer (
		GENERIC_READ |           // read/write access 
		GENERIC_WRITE,
		FILE_SHARE_READ |
		FILE_SHARE_WRITE,        // shared 
		NULL,                    // default security attributes 
		CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE 
		NULL );
	
	SetConsoleActiveScreenBuffer ( hNewScreenBuffer );

	for ( size_t i = 0; i < _countof ( chiBuffer ); i++ )
		chiBuffer[i].Attributes = ( FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED );

	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;

	coordBufSize.Y = 1;
}

void CloseStdScreen ( void )
{
	SetConsoleActiveScreenBuffer ( hStdout );
}

void __cdecl StdSBputsW ( size_t x, size_t y, int c, wchar_t * str, size_t ls )
{
	coordBufSize.X = (SHORT)ls;

	srctWriteRect.Top = (SHORT) x;
	srctWriteRect.Left = (SHORT) y;
	srctWriteRect.Bottom = (SHORT) x;
	srctWriteRect.Right = (SHORT) (y+ls);

	for ( size_t i = 0; i < ls; i++ )
		chiBuffer[i].Char.UnicodeChar = str[i];

	WriteConsoleOutput (
		hNewScreenBuffer,
		chiBuffer,
		coordBufSize,
		coordBufCoord,
		&srctWriteRect );
}

int TestCaWithStdScreenAndWithoutStat ( size_t loop_count )
{
	// string to display
	wchar_t strin[] = L">>>>>>> X <<<<<<<<";
	wchar_t strout[] = L"<<<<<<< X >>>>>>>>";

	size_t strlength = _countof ( strin );

	wchar_t * const casestr[2] = { strin,strout };

	size_t j = loop_count;

	//chars for roll  
	const wchar_t c[] = L"\\|/-";

	int Var;

	wchar_t * str;

	while ( j-- ){
		str = casestr[j % 2];
		// demo some display outputs
		str[8] = c[j % 4];
		StdSBputsW ( 10, 40, RGB ( 127, 100, 100 ), str, strlength );

		// demo for set value
		Var = rand ( );
	}

	return Var;
}

#endif //DEMO_STD_CONSOLE

typedef int (*TESTCASE) ( size_t loop_count );

DWORD RunTestCase ( TESTCASE o, int * result, size_t count )
{
	DWORD start;
	DWORD end;

	( *o ) (10);

	start = GetTickCount ( );
	*result = (*o) ( count );
	end = GetTickCount ( );

	return end - start;
}

int main()
{
	// set windows title
	SET_TITLE ( L"Test 2 App programmmm..." );

	// create window without callback 
	SBInit ( 0 );

#ifdef	DEMO_STD_CONSOLE
	InitStdScreen ( );
#endif

	int result;

	size_t count = 10000000;

#ifdef	DEMO_STD_CONSOLE
	count = count / 70; // standart console too slow for 10 billions !!!
#endif

	DWORD durwithall = RunTestCase ( TestCaWithScreenAndStat, &result, count );
	printf ( "Test case with screen and with stat time:%d res:%d\n", durwithall,result);

	DWORD durwithoutstat = RunTestCase ( TestCaWithScreenAndWithoutStat,&result, count );
	printf ( "Test case with screen and without stat time:%d res:%d\n", durwithoutstat, result );

	DWORD durwithoutall = RunTestCase ( TestCaWithoutScreen, &result, count );
	printf ( "Test case without screen and without stat time:%d res:%d\n", durwithoutall, result );

#ifdef	DEMO_STD_CONSOLE
	DWORD durwithstd = RunTestCase ( TestCaWithStdScreenAndWithoutStat, &result, count );
	printf ( "Test case use std screen and without stat time:%d res:%d\n", durwithstd, result );
#endif
	////////////////////

	printf ( "diff: %d %d "
#ifdef	DEMO_STD_CONSOLE
			 "%d"
#endif
			 "\n", durwithall - durwithoutall, durwithoutstat - durwithoutall
#ifdef	DEMO_STD_CONSOLE
			 , durwithstd - durwithoutall 
#endif
	);


#ifdef	DEMO_STD_CONSOLE
	CloseStdScreen ( );
#endif

	// close screen window and free all resources
	SBClose ( );

	printf ( "Press Enter to exit.\n");
	getchar ( );
	
}

