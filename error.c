/***************************************************************************
 *
 * $Header: /usr/local/cvsroot/utils/ytree/error.c,v 1.12 2003/06/06 19:47:26 werner Exp $
 *
 * Ausgabe von Fehlermeldungen
 *
 ***************************************************************************/


#include "ytree.h"




static void MapErrorWindow(char *header);
static void MapNoticeWindow(char *header);
static void UnmapErrorWindow(void);
static void PrintErrorLine(int y, char *str);
static void DisplayErrorMessage(char *msg);
static int  PrintErrorMessage(char *msg);



void Message(char *msg)
{
  MapErrorWindow( "E R R O R" );
  (void) PrintErrorMessage( msg );
}


void Notice(char *msg)
{
  MapNoticeWindow( "N O T I C E" );
  DisplayErrorMessage( msg );
  RefreshWindow( error_window );
  refresh();
}


void Warning(char *msg)
{
  MapErrorWindow( "W A R N I N G" );
  (void) PrintErrorMessage( msg );
}


void Error(char *msg, char *module, int line)
{
  char buffer[MESSAGE_LENGTH + 1];

  MapErrorWindow( "INTERNAL ERROR" );
  (void) sprintf( buffer, "%s*In Module \"%s\"*Line %d", 
		  msg, module, line
		);
  (void) PrintErrorMessage( buffer );
}





static void MapErrorWindow(char *header)
{
   werase( error_window );
   box( error_window, 0, 0 );

   PrintSpecialString( error_window, 
		       ERROR_WINDOW_HEIGHT - 3, 
		       0,
		       "6--------------------------------------7",
		       WINERR_COLOR
		     );
   wattrset( error_window, A_REVERSE | A_BLINK ); 
   MvWAddStr( error_window,
	      ERROR_WINDOW_HEIGHT - 2,
	      1,
	      "             PRESS ENTER              "
	    );
   wattrset( error_window, 0 );
   PrintErrorLine( 1, header );
}


static void MapNoticeWindow(char *header)
{
   werase( error_window );
   box( error_window, 0, 0 );

   PrintSpecialString( error_window, 
		       ERROR_WINDOW_HEIGHT - 3, 
		       0,
		       "6--------------------------------------7",
		       WINERR_COLOR
		     );
   wattrset( error_window, A_REVERSE | A_BLINK ); 
   MvWAddStr( error_window,
	      ERROR_WINDOW_HEIGHT - 2,
	      1,
	      "             PLEASE WAIT              "
	    );
   wattrset( error_window, 0 );
   PrintErrorLine( 1, header );
}


static void UnmapErrorWindow(void)
{
   werase( error_window );
   touchwin( stdscr );
   doupdate();
}


void UnmapNoticeWindow(void)
{
   werase( error_window );
   touchwin( stdscr );
   doupdate();
}



static void PrintErrorLine(int y, char *str)
{
  int l;

  l = strlen( str );

  MvWAddStr( error_window, y, (ERROR_WINDOW_WIDTH - l) >> 1, str );
}




static void DisplayErrorMessage(char *msg)
{
  int  y, i, j, count;
  char buffer[ERROR_WINDOW_WIDTH - 2 + 1];

  for(i=0, count=0; msg[i]; i++)
    if( msg[i] == '*' ) count++;

  if( count > 3 )      y = 2;
  else if( count > 1 ) y = 3;
  else                 y = 4;


  for( i=0,j=0; msg[i]; i++ )
  {
    if( msg[i] == '*' )
    {
      buffer[j] = '\0';
      PrintErrorLine( y++, buffer );
      j=0;
    }
    else
    {
      if( j < (int)((sizeof( buffer) - 1)) ) buffer[j++] = msg[i];
    }
  } 
  buffer[j] = '\0';
  PrintErrorLine( y, buffer );
}



static int PrintErrorMessage(char *msg)
{
  int c;
  
  DisplayErrorMessage( msg );
  beep();
  RefreshWindow( error_window );
  doupdate();
  c = wgetch(error_window);
  UnmapErrorWindow();
  touchwin( dir_window );
  return( c );
}

