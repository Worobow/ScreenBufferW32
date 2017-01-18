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

int main ( int q, char * a[] ) 
{ 
    // string to display
    wchar_t str[]=L">>>>>>> X <<<<<<<<";
    // value of loop count. 
    int j=1000000;

    //chars for roll  
    wchar_t c[]=L"\\|/-";

    // set windows title
    SET_TITLE (L"Main programmmm...");

    // create window without callback 
    SBInit ( 0 );

    // Variable for stat 
    int Var = 0;

    // The place where we gather statistics on the variable 'Var'
    // and zero init all fields.
    STAT mst = {0};
    // set cut-off level.
    mst.drop = INT_MAX;

    while ( j-- ) {
        // demo some display outputs
        str[8]=c[j%4];
        SBputsW ( 10, 40, RGB(127,100,100), str, _countof(str) );

        // demo for set value
        Var = rand ( );
        // demo statistic accumulate
        AccumStats ( &mst, Var);
        // demo statistic display
        if ( !( clock ( ) % (CLOCKS_PER_SEC/10) ) ){
            ShowStat ( 1, 1, L"rand", 4, &mst );
        }
    }
    // wait for while you watching.
    system("cmd.exe /K echo when done type 'exit'.");
    // close screen window and free all resources
    SBClose ( );
}