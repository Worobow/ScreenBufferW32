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
#include <stdio.h>
#include "ScreenBufferW32.h"

//
// This function shows the accumulated to date statistics on the screen
//
//        Args:
//            x        -- Row coordinate of the screen.
//            y        -- Column coordinate of the screen.
//            text    -- prefix 
//            lt        -- length of the prefix
//            stat    -- pointer of the statistis
//

VOID __cdecl ShowStat ( size_t x, size_t y, wchar_t * text, size_t lt, STAT * stat )
{
    static const size_t tempstr_size = 40;
    wchar_t str[tempstr_size];
    size_t lstr=0;
    wchar_t s1[]=L" -";  size_t ls1=sizeof(s1)/sizeof(s1[0])-1;
    wchar_t sC[]=L" C:"; size_t lsC=sizeof(sC)/sizeof(sC[0])-1;
    wchar_t sM[]=L" M:"; size_t lsM=sizeof(sM)/sizeof(sM[0])-1;
    wchar_t sm[]=L" m:"; size_t lsm=sizeof(sm)/sizeof(sm[0])-1;
    wchar_t sA[]=L" A:"; size_t lsA=sizeof(sA)/sizeof(sA[0])-1;
    
    
    INT64 aver;
    
    if ( stat->count )
        aver=stat->summ/(stat->count);
    else
        aver=-1;
    
    SBclrs(x,y,0,80);
    
    SBputsW(x,y,0,text,lt);y+=lt;
    SBputsW(x,y,0,s1,ls1);y+=ls1;
    SBputsW(x,y,0,sC,lsC);y+=lsC;
    _i64tow_s(stat->count,str, tempstr_size,10);lstr=wcslen(str);SBputsW(x,y,0,str,lstr);y+=lstr;
    SBputsW(x,y,0,sM,lsM);y+=lsM;
    _i64tow_s (stat->max,  str, tempstr_size,10);lstr=wcslen(str);SBputsW(x,y,0,str,lstr);y+=lstr;
    SBputsW(x,y,0,sm,lsm);y+=lsm;
    _i64tow_s (stat->min,  str, tempstr_size,10);lstr=wcslen(str);SBputsW(x,y,0,str,lstr);y+=lstr;
    SBputsW(x,y,0,sA,lsA);y+=lsA;
    _i64tow_s (aver,str, tempstr_size,10);lstr=wcslen(str);SBputsW(x,y,0,str,lstr);y+=lstr;
}
                     // InterlockedCompareExchange64
#if (_WIN32_WINNT < 0x0502)

__forceinline LONGLONG  InterlockedCompareExchange64 ( LONGLONG volatile* mem, LONGLONG Exchange, LONGLONG Comperand)//__declspec(noinline)
{
__asm{
        mov        ebx,dword ptr Exchange;low
        mov        ecx,dword ptr Exchange+4;high<- addr

        mov        eax,dword ptr Comperand
        mov        edx,dword ptr Comperand+4

        mov        esi, mem

        lock    cmpxchg8b     [esi]
    }
}

LONGLONG InterlockedExchangeAdd64(  LONGLONG volatile* Addend,  LONGLONG Value ){

    LONGLONG o,n;
    do {
        o=*Addend;
        n=o+Value;
    }while ( InterlockedCompareExchange64(Addend,n,o) != o ) ;
    return  o;
}
#endif

//
// This function is used to accumulate statistics
// 
//        Args:
//            stat    --    place for statistic storage.
//            cur        --    current value of integer variable.
//
VOID __cdecl AccumStats ( STAT * stat, int cur )
{
    int om;
    
    if ( cur > stat->drop )
        return;
    
    InterlockedExchangeAdd((LONG volatile *)&stat->count,1);
    InterlockedExchangeAdd64((LONGLONG*)&stat->summ,cur);
    
    do {
        om=stat->max;
        if ( om >= cur )
            break;
    }while ( (ULONG) InterlockedCompareExchange((LONG volatile *)&stat->max,cur,om) != om );
    do {
        om=stat->min;
        if ( om <= cur )
            break;
    }while ( (ULONG)InterlockedCompareExchange((LONG volatile *)&stat->min,cur,om) != om );
}