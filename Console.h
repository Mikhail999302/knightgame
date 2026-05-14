#include <windows.h> 

/** 
 *	Foreground and background colors :
 *		F and B means foreground and background correspondingly,
 *		D and L means dark and ligh
 **/
enum COLORS {
	F_D_BLUE	= FOREGROUND_BLUE,
	F_D_GREEN	= FOREGROUND_GREEN,
	F_D_RED		= FOREGROUND_RED,
	F_D_CYAN	= FOREGROUND_BLUE		| FOREGROUND_GREEN,
	F_D_MAGENTA	= FOREGROUND_BLUE		| FOREGROUND_RED,
	F_D_YELLOW	= FOREGROUND_GREEN		| FOREGROUND_RED,
	F_D_WHITE	= FOREGROUND_BLUE		| FOREGROUND_GREEN	| FOREGROUND_RED,
	F_L_BLUE	= FOREGROUND_INTENSITY	| FOREGROUND_BLUE,
	F_L_GREEN	= FOREGROUND_INTENSITY	| FOREGROUND_GREEN,
	F_L_RED		= FOREGROUND_INTENSITY	| FOREGROUND_RED,
	F_L_CYAN	= FOREGROUND_INTENSITY	| FOREGROUND_BLUE	| FOREGROUND_GREEN,
	F_L_MAGENTA	= FOREGROUND_INTENSITY	| FOREGROUND_BLUE	| FOREGROUND_RED,
	F_L_YELLOW	= FOREGROUND_INTENSITY	| FOREGROUND_GREEN	| FOREGROUND_RED,
	F_L_WHITE	= FOREGROUND_INTENSITY	| FOREGROUND_BLUE	| FOREGROUND_GREEN | FOREGROUND_RED,

	B_D_BLUE	= BACKGROUND_BLUE,
	B_D_GREEN	= BACKGROUND_GREEN,
	B_D_RED		= BACKGROUND_RED,
	B_D_CYAN	= BACKGROUND_BLUE		| BACKGROUND_GREEN,
	B_D_MAGENTA	= BACKGROUND_BLUE		| BACKGROUND_RED,
	B_D_YELLOW	= BACKGROUND_GREEN		| BACKGROUND_RED,
	B_D_WHITE	= BACKGROUND_BLUE		| BACKGROUND_GREEN	| BACKGROUND_RED,
	B_L_BLUE	= BACKGROUND_INTENSITY	| BACKGROUND_BLUE,
	B_L_GREEN	= BACKGROUND_INTENSITY	| BACKGROUND_GREEN,
	B_L_RED		= BACKGROUND_INTENSITY	| BACKGROUND_RED,
	B_L_CYAN	= BACKGROUND_INTENSITY	| BACKGROUND_BLUE	| BACKGROUND_GREEN,
	B_L_MAGENTA	= BACKGROUND_INTENSITY	| BACKGROUND_BLUE	| BACKGROUND_RED,
	B_L_YELLOW	= BACKGROUND_INTENSITY	| BACKGROUND_GREEN	| BACKGROUND_RED,
	B_L_WHITE	= BACKGROUND_INTENSITY	| BACKGROUND_BLUE	| BACKGROUND_GREEN | BACKGROUND_RED,

	BLACK		= 0
};

/** 
 *	Console initialization :
 *		title  - title of our window
 *		width  - window width, default value is 80
 *		height - window height, default value is 25
 **/
void InitConsole( char const * title, int width = 80, int height = 25);

/**
 *	Sets visibility state for text cursor :
 *		true  - show cursor
 *		false - hide cursor
 **/		
void VisibleCursor( bool visible );

/**
 *	Stuff for printing, similsr to printf :
 *		x		- vertical coordinate for printing starting
 *		y		- horizontal coordinate for printing starting
 *		color	- color of text output; see COLORS above
 *		format	- string containing format; see help on printf for more information
 *		...		- list of variables according to format; see help on printf for more information
 **/
void ColorPrint( int x, int y, WORD color, LPCTSTR format, ... );

// Uses current color stated by SetColor()
void ColorPrint( int x, int y, LPCTSTR format, ... );

// Uses current cursor coordinates;
void ColorPrint( WORD color, LPCTSTR format, ... );

// Uses both current color and cursor coordinates
void ColorPrint( LPCTSTR format, ... );

/**
 *	Sets text cursor to ( x, y ) position in window
 **/
void MoveCursor( int x, int y );

/**
 *	Resizes our console window :
 *		width	- new window width
 *		height	- new window height
 **/
void SetConsoleSize( int width, int height );

/**
 *	Clears all text and attribute information in window
 **/
void ClearConsole();

/**
 *	Sets current color for ColorPrint's
 **/
void SetColor( WORD color );

///////////////////
// Best regards, //
//	Anton.       //
///////////////////