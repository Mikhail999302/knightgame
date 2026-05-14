#include "stdafx.h"
#include "console.h"

HANDLE	hOutput = NULL; 
COORD	coord, cursorCoords;
int		consoleWidth, consoleHeight;
DWORD	cWritten;
WORD	currentColor;
char	text[1024], colors[1024];

void InitConsole( char const * title, int width, int height ) {
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	consoleWidth = width;
	consoleHeight = height;
	SetConsoleSize( width, height );
	SetConsoleTitle( title );
	MoveCursor( 0, 0 );
}

void VisibleCursor( bool visible ) {
	if ( hOutput ) {
		CONSOLE_CURSOR_INFO curs_info;
		curs_info.dwSize = 10;
		curs_info.bVisible = visible;
		SetConsoleCursorInfo( hOutput, &curs_info );
	}
}

void ColorPrint( int x, int y, WORD color, LPCTSTR format, ... ) {
int temp_len;
coord.X = x;
coord.Y = y;
va_list alist;

	if ( hOutput ) {
		va_start( alist, format );
		vsprintf( text, format, alist );

		temp_len = lstrlen( text );
		FillConsoleOutputAttribute(
			hOutput,
			color,	
			temp_len,
			coord,
			&cWritten);

		WriteConsoleOutputCharacter( 
			hOutput,
			text,
			temp_len,
			coord,
			&cWritten );
		va_end( alist );
		MoveCursor( ( x + temp_len ) % consoleWidth, y + ( x + temp_len ) / consoleWidth );
	}
}

void ColorPrint( int x, int y, LPCTSTR format, ... ) {
int temp_len;
coord.X = x;
coord.Y = y;
va_list alist;

	if ( hOutput ) {
		va_start( alist, format );
		vsprintf( text, format, alist );

		temp_len = lstrlen( text );
		FillConsoleOutputAttribute(
			hOutput,
			currentColor,	
			temp_len,
			coord,
			&cWritten);

		WriteConsoleOutputCharacter( 
			hOutput,
			text,
			temp_len,
			coord,
			&cWritten );
		va_end( alist );
		MoveCursor( ( x + temp_len ) % consoleWidth, y + ( x + temp_len ) / consoleWidth );
	}

}

void ColorPrint( WORD color, LPCTSTR format, ... ) {
int temp_len;
va_list alist;

	if ( hOutput ) {
		va_start( alist, format );
		vsprintf( text, format, alist );

		temp_len = lstrlen( text );
		FillConsoleOutputAttribute(
			hOutput,
			color,	
			temp_len,
			cursorCoords,
			&cWritten);

		WriteConsoleOutputCharacter( 
			hOutput,
			text,
			temp_len,
			cursorCoords,
			&cWritten );
		va_end( alist );
		MoveCursor( ( cursorCoords.X + temp_len ) % consoleWidth, 
					cursorCoords.Y + ( cursorCoords.X + temp_len ) / consoleWidth );
	}

}

void ColorPrint( LPCTSTR format, ... ) {
int temp_len;
va_list alist;

	if ( hOutput ) {
		va_start( alist, format );
		vsprintf( text, format, alist );

		temp_len = lstrlen( text );
		FillConsoleOutputAttribute(
			hOutput,
			currentColor,	
			temp_len,
			cursorCoords,
			&cWritten);

		WriteConsoleOutputCharacter( 
			hOutput,
			text,
			temp_len,
			cursorCoords,
			&cWritten );
		va_end( alist );
		MoveCursor( ( cursorCoords.X + temp_len ) % consoleWidth, 
					cursorCoords.Y + ( cursorCoords.X + temp_len ) / consoleWidth );
	}
}


void MoveCursor( int x, int y ) {
	if ( hOutput ) {
		cursorCoords.X = x;
		cursorCoords.Y = y;
		SetConsoleCursorPosition( hOutput, cursorCoords );
	}
}

void SetConsoleSize( int width, int height ) {
	if ( hOutput ) {
		SMALL_RECT winSize = { 0, 0, width - 1, height - 1 };
		coord.X = width;
		coord.Y = height;
		SetConsoleWindowInfo( hOutput, TRUE, &winSize );
		SetConsoleScreenBufferSize( hOutput, coord );
	}
}

void ClearConsole() {
	if ( hOutput ) {
		coord.X = 0;
		coord.Y = 0;

		FillConsoleOutputCharacter(
			hOutput,
			' ',
			consoleWidth * consoleHeight,
			coord,
			&cWritten );

		FillConsoleOutputAttribute(
			hOutput,
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
  			consoleWidth * consoleHeight,
			coord,
			&cWritten );
		MoveCursor( 0, 0 );
	}
}

void SetColor( WORD color ) {
	currentColor = color;
}
