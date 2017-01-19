
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

typedef int (*TESTCASE) ( size_t loop_count );

//TESTCASE  TestCases[] = {};

DWORD RunTestCase ( TESTCASE o, int * result, size_t count )
{
	DWORD start;
	DWORD end;

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

	int result;
	size_t count = 100000000;

	RunTestCase ( TestCaWithScreenAndStat, &result, 10 );
	DWORD durwithall = RunTestCase ( TestCaWithScreenAndStat, &result, count );
	printf ( "Test case with screen and with stat time:%d res:%d\n", durwithall,result);

	RunTestCase ( TestCaWithScreenAndWithoutStat, &result, 10 );
	DWORD durwithoutstat = RunTestCase ( TestCaWithScreenAndWithoutStat,&result, count );
	printf ( "Test case with screen and without stat time:%d res:%d\n", durwithoutstat, result );

	RunTestCase ( TestCaWithoutScreen, &result, 10 );
	DWORD durwithoutall = RunTestCase ( TestCaWithoutScreen, &result, count );
	printf ( "Test case without screen and without stat time:%d res:%d\n", durwithoutall, result );

	////////////////////

	printf ( "diff: %d %d\n", durwithall - durwithoutall, durwithoutstat - durwithoutall );

	// close screen window and free all resources
	SBClose ( );

	printf ( "Press Enter to exit.\n");
	getchar ( );
	
}

