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

//
// windows update callback function declaration 
//
typedef void (*SCREENBUFFER_CALLBACK) ( void );

//
// Init function 
//
//		Args:
//			updater -- This function is called after the screen has been updated.
//				       This argument may be zero, if you don't want to receive this notification.
//
#ifdef __cplusplus
extern "C" 
#endif
int  __cdecl SBInit ( SCREENBUFFER_CALLBACK updater );

//
// Detach function. 
//
#ifdef __cplusplus
extern "C" 
#endif
void __cdecl SBClose ( void );

//
// Output wide string to screen in specified coords
//
//		Args:
//			x	-- Row coordinate of the screen.
//			y	-- Column coordinate of the screen.
//			c	-- color for all string, but now not use.
//			ls	-- string length
//
#ifdef __cplusplus
extern "C" 
#endif
void __cdecl SBputsW ( size_t x, size_t y, int c, wchar_t * str, size_t ls );

// Clear frame of screen 
// 
//		Args:
//			x	-- Row coordinate of the screen.
//			y	-- Column coordinate of the screen.
//			c	-- Color (not used).
//			str	-- String for out.
//			ls	-- Length of the string.

#ifdef __cplusplus
extern "C" 
#endif
void __cdecl SBclrs ( size_t x, size_t y, int c, size_t ls );

//
//macro for display constant string at x,y coords 
//
#define CONSTR(x,y,s) {wchar_t str[]=s;SBputsW (x,y,0,str,sizeof(str)/sizeof(str[0])-1);}
//
// macro for display constant string with printf like format
//
inline void CONFSTR ( size_t x, size_t y, wchar_t * s, ... )
{
    va_list a;
    wchar_t str[256];
    int i;
    va_start(a,s);

#ifdef _NTDDK_
    i=vswprintf(str,s,a);
#else
    i=vswprintf(str,sizeof(str),s,a);
#endif

    SBputsW (x,y,0,str,i);
}

#ifdef __cplusplus
extern "C"
#endif
wchar_t * SCBWTitle;

#define SET_TITLE(a) SCBWTitle=a

typedef struct {
    int	volatile min;
    int	volatile max;
    size_t	volatile count;
    long long	volatile summ;
    int	volatile drop;
}STAT;


#ifdef __cplusplus
extern "C" 
#endif
VOID  __cdecl ShowStat ( size_t x, size_t y, wchar_t * text, size_t lt, STAT * stat );
#ifdef __cplusplus
extern "C" 
#endif
VOID  __cdecl AccumStats ( STAT * stat, int cur );

#define SCREEN_HIGHT 35
#define SCREEN_WIDE 127
