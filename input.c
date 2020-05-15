
#include "ytree.h"
#include "tilde.h"
#include "xmalloc.h"


/***************************************************************************
 * InputStr                                                                *
 * Liest eine Zeichenkette an Position (y,x) mit der max. Laenge length    *
 * Vorschlagswert fuer die Eingabe ist s selbst                            *
 * Zurueckgegeben wird das Zeichen, mit dem die Eingabe beendet wurde      *
 ***************************************************************************/


char *StrLeft(const char *str, size_t count)
{
#ifdef WITH_UTF8
  mbstate_t state;
#endif
  char *rez, *p, *tmp;
  size_t len, i;

#ifdef WITH_UTF8
  memset(&state, 0, sizeof(state));
#endif
  if (count == 0) return(Strdup(""));
  len = StrVisualLength(str);
  if (count >= len) return(Strdup(str));
  
  len = 0;

  tmp = Strdup(str);
  p = tmp; 
  for (i = 0; i < count; i++) {
#ifdef WITH_UTF8
    len += mbrlen(p, 4, &state);
#else
    len++;
#endif
    p = tmp + len;
  }
  free(tmp);
  
  rez = Strndup(str, len);
  rez[len] = '\0';
  return(rez);
}

char *StrRight(const char *str, size_t count)
{
#ifdef WITH_UTF8
  mbstate_t state;
#endif
  char *rez, *p, *tmp;
  size_t byte_len, char_len, tmp_len, i;

#ifdef WITH_UTF8
  memset(&state, 0, sizeof(state));
#endif

  if (count == 0) return(Strdup(""));

  byte_len = strlen(str);
  char_len = StrVisualLength(str);
  
  if (count > char_len) count = char_len;
  
  tmp = Strdup(str);
  p = tmp;
  i = 0;
  rez = NULL;
  while ( (p - tmp) < byte_len ) {
    if (i == (char_len - count) ) {
      rez = Strdup(p);
    }
#ifdef WITH_UTF8    
    tmp_len = mbrlen(p, 4, &state);
#else
    tmp_len = 1;
#endif
    p += tmp_len;
    i++;
  }

  free(tmp);
  return(rez);
}

int StrVisualLength(const char *str)
{
#ifdef WITH_UTF8
  mbstate_t state;
  int len = 0;
  
  memset(&state, '\0', sizeof(state));
  len = mbsrtowcs(NULL, &str, strlen(str), &state);
  if(len < 0) {
    /* Invalid multibyte sequence */
    len = strlen(str);
  }
#else
  return(strlen(str));
#endif
}


int InputString(char *s, int y, int x, int cursor_pos, int length, char *term)
                               /* Ein- und Ausgabestring              */
                               /* Position auf Bildschirm             */
                               /* max. Laenge                         */
                               /* Menge von Terminierungszeichen      */
{
  int p;                       /* Aktuelle Position                   */
  int c1;                      /* Gelesenes Zeichen                   */
  int i;                       /* Laufvariable                        */
  char *pp;
  BOOL len_flag = FALSE;
  char path[PATH_LENGTH + 1];
  char buf[2], sbuf[20], *ls, *rs;
  static BOOL insert_flag = TRUE;

  buf[1] = '\0';
  strcpy(sbuf, "");

  /* Feld gefuellt ausgeben */
  /*------------------------*/
  print_time = FALSE;
  curs_set(1);
  MvAddStr( y, x, s );
  leaveok(stdscr, FALSE);
  
  for(i=strlen(s); i < length; i++)
    addch( '_' );

  p = cursor_pos;
  
  MvAddStr( y, x, s );

  nodelay( stdscr, TRUE );
  do {
    c1 = wgetch(stdscr);
    
    if ( c1 != ERR ) {
    
      if( c1 >= ' ' && c1 < 0xff && c1 != 127 ) {
        if ( len_flag == TRUE) beep();
        else {
          buf[0] = (char)c1;
	  strcat(sbuf, buf);
	}
      } else {
        /* Control symbols */
        switch( c1 )
        {
        case 'C' & 0x1f     : c1 = 27; 
			      break;

        case KEY_LEFT       : if( p > 0 )
                                p--;
                              else
                                beep();
                              break;
        case KEY_RIGHT      : if( p < StrVisualLength(s) )
                                p++;
                              else
                                beep();
                              break;
	case KEY_UP         : 
	                      nodelay(stdscr, FALSE);
			      pp = GetHistory();
			      nodelay(stdscr, TRUE);
                             if (pp == NULL) break;
                             if(*pp)
                             {
			       ls = StrLeft(pp, length);
			       strcpy(s, ls);
			       free(ls);
			       p = StrVisualLength(s);
                               MvAddStr( y, x, s );
                               for(i=p; i < length; i++)
                                 addch( '_' );
                               RefreshWindow( stdscr );
                               doupdate();
                             }
                             break;

        case KEY_HOME       : p = 0;
                              break;
        case KEY_END        : p = StrVisualLength( s );
                              break;
        case KEY_DC         : if( p < StrVisualLength(s) )
                              {
			        ls = StrLeft(s, p);
				rs = StrRight(s, StrVisualLength(s) - p - 1);
				strcpy(s, ls);
				strcat(s, rs);
				free(ls);
				free(rs);
				MvAddStr( y, x, s );
                                addch( '_' );
                              }
                              break;
	case 0x08           :
        case 0x7F           :
        case KEY_BACKSPACE  : if( p > 0 )
                              {
			        ls = StrLeft(s, p - 1);
				rs = StrRight(s, StrVisualLength(s) - p);
				strcpy(s, ls);
				strcat(s, rs);
				free(ls);
				free(rs);
				MvAddStr( y, x, s );
                                addch( '_' );
                                p--;
                              }
                              else
                                beep();
                              break;
        case KEY_DL         : for(i=0; i < StrVisualLength(s) - p; i++) addch( '_' );
			      ls = StrLeft(s, p);
			      strcpy(s, ls);
			      free(ls);
                              break;
	case KEY_EIC        :
        case KEY_IC         : insert_flag ^= TRUE; 
                              break;
	case '\t'           : if(( pp = GetMatches(s)) == NULL) {
			       break;
			     }
                             if(*pp)
                             {
			       ls = StrLeft(pp, length);
			       strcpy(s, ls);
			       free(ls);
			       p = StrVisualLength(s);
			       free(pp);
                               MvAddStr( y, x, s );
                               for(i=p; i < length; i++) addch( '_' );
                               RefreshWindow( stdscr );
                               doupdate();
                             }
                             break;
#ifdef KEY_F
        case KEY_F(2)       : 
#endif
        case 'F' & 0x1f     : if(KeyF2Get( statistic.tree,
                                           statistic.disp_begin_pos,
                                           statistic.cursor_pos, path)) 
                              {
			        /* beep(); */
				break;
			      }
                              if(*path)
                              {
			        ls = StrLeft(path, length);
				strcpy(s, ls);
				free(ls);
			        p = StrVisualLength(s);
                                MvAddStr( y, x, s );
                                for(i=p; i < length; i++) addch( '_' );
                                RefreshWindow( stdscr );
                                doupdate();
                              }
                              break;

        default             : if( c1 == LF ) c1 = CR;
                              break;
        } /* switch */
      } /* else control symbols */
    } else {
     
      if (strlen(sbuf) > 0) {
       if ( insert_flag ) {
  	    /* append symbol */
	    if ( p >= StrVisualLength(s)) strcat(s, sbuf);
	    else {
	      /* insert symbol at cursor position */
	      if ( p > 0 ) ls = StrLeft(s, p);
	      else ls = Strdup("");
	      rs = StrRight(s, StrVisualLength(s) - p);
	      strcpy(s, ls);
	      strcat(s, sbuf);
	      strcat(s, rs);
	      free(ls);
	      free(rs);
	    }
          } else {
            /* owerwrite symbol at cursor position */
	    if ( p > StrVisualLength(s) - 1) strcat(s, sbuf);
	    else {
  	      if (p > 0) ls = StrLeft(s, p);
	      else ls = Strdup("");
	      rs = StrRight(s, StrVisualLength(s) - p - 1);
	      strcpy(s, ls);
	      strcat(s, sbuf);
	      strcat(s, rs);
	      free(ls);
	      free(rs);
	    }
	  }

	strcpy(sbuf, "");
	p++;
      }

      if (StrVisualLength(s) >= length) len_flag = TRUE;
      else len_flag = FALSE;

      MvAddStr( y, x, s );
      wmove(stdscr, y, x + p);
    }
  } while( c1 !=  27 && c1 != CR );

  nodelay( stdscr, FALSE );

  p = strlen( s );
  move( y, x + p );

  for(i=0; i < length - p; i++ )
   addch( ' ' );

  move( y, x );
  leaveok( stdscr, TRUE);
  curs_set(0);
  print_time = TRUE;
  InsHistory( s );
#ifdef READLINE_SUPPORT
  pp = tilde_expand(s);
#else
  pp = Strdup(s);
#endif

  strncpy( s, pp, length - 1);
  s[length]='\0';
  xfree(pp);
  return( c1 );
}





int InputChoise(char *msg, char *term)
{
  int  c;

  ClearHelp();

  curs_set(1);
  leaveok(stdscr, FALSE);
  mvprintw( LINES - 2, 1, msg );
  RefreshWindow( stdscr );
  doupdate();
  do
  {
    c = Getch();
    if(c >= 0)
      if( islower( c ) ) c = toupper( c );
  } while( c != -1 && !strchr( term, c ) );
  
  if(c >= 0)
    echochar( c );

  move( LINES - 2, 1 ); clrtoeol();
  leaveok(stdscr, TRUE);
  curs_set(0);

  return( c );
}





int GetTapeDeviceName( void )
{
  int  result;
  char path[PATH_LENGTH * 2 +1];

  result = -1;

  ClearHelp();

  (void) strcpy( path, statistic.tape_name );

  MvAddStr( LINES - 2, 1, "Tape-Device:" );
  if( InputString( path, LINES - 2, 14, 0, COLS - 15, "\r\033" ) == CR )
  {
    result = 0;
    (void) strcpy( statistic.tape_name, path );
  }

  move( LINES - 2, 1 ); clrtoeol();

  return( result );
}


void HitReturnToContinue(void)
{
#ifndef XCURSES
  curs_set(1);
  vidattr( A_REVERSE );
  putp( "[Hit return to continue]" );
  vidattr( 0 );
  (void) fflush( stdout );
  (void) Getch();
#endif /* XCURSES */
  curs_set(0);
  doupdate();
}



BOOL KeyPressed()
{
  BOOL pressed = FALSE;

#if !defined( linux ) || !defined( TERMCAP )
  nodelay( stdscr, TRUE );
  if( wgetch( stdscr ) != ERR ) pressed = TRUE;
  nodelay( stdscr, FALSE );
#endif /* linux/TERMCAP */

  return( pressed );
}


BOOL EscapeKeyPressed()
{
  BOOL pressed = FALSE;
  int  c;

#if !defined( linux ) || !defined( TERMCAP )
  nodelay( stdscr, TRUE );
  if( ( c = wgetch( stdscr ) ) != ERR ) pressed = TRUE;
  nodelay( stdscr, FALSE );
#endif /* linux/TERMCAP */

  return( ( pressed && c == ESC ) ? TRUE : FALSE );
}




#ifdef VI_KEYS

int ViKey( int ch )
{
  switch( ch )
  {
    case VI_KEY_UP:    ch = KEY_UP;    break;
    case VI_KEY_DOWN:  ch = KEY_DOWN;  break;
    case VI_KEY_RIGHT: ch = KEY_RIGHT; break;
    case VI_KEY_LEFT:  ch = KEY_LEFT;  break;
    case VI_KEY_PPAGE: ch = KEY_PPAGE; break;
    case VI_KEY_NPAGE: ch = KEY_NPAGE; break;
  }
  return(ch);
}

#endif /* VI_KEYS */

  
#ifdef _IBMR2
#undef wgetch

int AixWgetch( WINDOW *w )
{
  int c;
  
  if( ( c = wgetch( w ) ) == KEY_ENTER ) c = LF;

  return( c );
}

#endif


int Getch()
{
  int c;

  c = getch();

#ifdef KEY_RESIZE
  if(c == KEY_RESIZE) {
    resize_request = TRUE;
    c = -1;
  }
#endif

  return(c);
}

