#include "stdafx.h"
#include "console.h"
#include <conio.h>
#include <stdio.h>
#include "R32M.h"

int mainx(int argc, char* argv[])
{
	//init console
	InitConsole( "Game" );
	//ClearConsole();

	rninit( 13666 );

	
	int a = 100;
	char * str = "str";
	double f = 123.947;
	//trying to write different stuff different ways
	ColorPrint( 20, 20, F_L_RED,"A = %d, str = %s gggggggggggggggg",a,str );
	ColorPrint( 20, 21, F_D_WHITE | B_L_WHITE ,"A = %d, str = %s, f = %f gggggggggggggggg",a,str,f );

	//color test
	ColorPrint( 10, 0, F_D_BLUE,"X");
	ColorPrint( 10, 1, F_D_GREEN,"X");
	ColorPrint( 10, 2, F_D_RED,"X");
	ColorPrint( 10, 3, F_D_CYAN,"X");
	ColorPrint( 10, 4, F_D_MAGENTA,"X");
	ColorPrint( 10, 5, F_D_YELLOW,"X");
	ColorPrint( 10, 6, F_D_WHITE,"X");
	ColorPrint( 11, 0, F_L_BLUE,"X");
	ColorPrint( 11, 1, F_L_GREEN,"X");
	ColorPrint( 11, 2, F_L_RED,"X");
	ColorPrint( 11, 3, F_L_CYAN,"X");
	ColorPrint( 11, 4, F_L_MAGENTA,"X");
	ColorPrint( 11, 5, F_L_YELLOW,"X");
	ColorPrint( 11, 6, F_L_WHITE,"X");

	ColorPrint( 10, 7,  B_D_BLUE,"X");
	ColorPrint( 10, 8,  B_D_GREEN,"X");
	ColorPrint( 10, 9,  B_D_RED,"X");
	ColorPrint( 10, 10, B_D_CYAN,"X");
	ColorPrint( 10, 11, B_D_MAGENTA,"X");
	ColorPrint( 10, 12, B_D_YELLOW,"X");
	ColorPrint( 10, 13, B_D_WHITE,"X");
	ColorPrint( 11, 7,  B_L_BLUE,"X");
	ColorPrint( 11, 8,  B_L_GREEN,"X");
	ColorPrint( 11, 9,  B_L_RED,"X");
	ColorPrint( 11, 10, B_L_CYAN,"X");
	ColorPrint( 11, 11, B_L_MAGENTA,"X");
	ColorPrint( 11, 12, B_L_YELLOW,"X");
	ColorPrint( 11, 13, B_L_WHITE,"X");
	
	SetColor( F_L_YELLOW );
	ColorPrint("X");
	ColorPrint(B_L_CYAN, "X");
	ColorPrint("X");
	ColorPrint("X");

	//hide cursor
	VisibleCursor( false );

	_getch();
	
	//ClearConsole();
	VisibleCursor( true );
	MoveCursor( 5, 10 );
	printf("GGG !");

	int i=0, j=0;
	MoveCursor(30, 0 );
	for ( i=30; i<80; i++ ) {
		for ( j=0; j<25; j++ ) {
			MoveCursor( i, j );
			ColorPrint(int( rnunif() * 255 ), " ");
		}
	}
	
	while ( !kbhit() ) {
		MoveCursor( 30 + int( rnunif() * 50 ) , int( rnunif() * 25 ) );
		ColorPrint(int( rnunif() * 255 ), " ");
	}
	
	return 0;
}

