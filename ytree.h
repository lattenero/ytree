/***************************************************************************
 *
 * $Header: /usr/local/cvsroot/utils/ytree/ytree.h,v 1.47 2016/09/04 14:41:12 werner Exp $
 *
 * Header-Datei fuer YTREE
 *
 ***************************************************************************/


#define _LARGEFILE64_SOURCE 1
#define _FILE_OFFSET_BITS 64                                                                   

#include <stdio.h>
#include <ctype.h>
#include <math.h>

#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__APPLE__)
#include <locale.h>
#endif

#ifdef XCURSES
#include <xcurses.h>
#define HAVE_CURSES 1
#endif

#ifdef _IBMR2
#define NLS
#endif /* _IBMR2 */

#ifdef ultrix
#ifndef HAVE_CURSES
#include <cursesX.h>
#endif
#else
#ifdef __FreeBSD__
#ifndef HAVE_CURSES
#include <ncurses.h>
#endif
#else
#ifndef HAVE_CURSES
#include <curses.h>
#endif
#endif /* __FreeBSD__ */
#endif /* ultrix */

#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#if defined(linux) || defined(__GNU__)
#include <locale.h>
#include <sys/wait.h>
#include <sys/time.h>	/* needed vor RedHed5 (thanks to Robert Spier) */
#endif


#if __STDC__ || defined( _IBMR2 )
#include <stdlib.h>
#endif /* __STDC__ || _IBMR2 */

#ifdef __NeXT__
extern char *getcwd();
#include <sys/dir.h>
#define dirent direct
#else
#include <unistd.h>
#include <dirent.h>
#endif /* __NeXT__ */

#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#ifndef __QNX__
#include <memory.h>
#endif
#include <pwd.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>

#if defined( TERMCAP ) && !defined( __NeXT__ )
#include <termcap.h>
#endif

#ifdef WITH_UTF8
#include <wchar.h>
#endif

#ifdef __OpenBSD__
#define STATFS(a, b, c, d )     statfs( a, b )
#define LONGLONG		long long
#define HAS_LONGLONG		1
#else
#ifdef __NetBSD__
#define STATFS(a, b, c, d )     statvfs( a, b )
#define LONGLONG		long long
#define HAS_LONGLONG		1
#define typeahead( file )
#define vidattr( attr )
#define putp( str )             tputs( str, 1, putchar )
#else
#ifdef __APPLE__
#define STATFS(a, b, c, d )     statfs( a, b )
#define LONGLONG		long long
#define HAS_LONGLONG		1
#else
#ifdef __FreeBSD__
#define STATFS(a, b, c, d )     statfs( a, b )
#define LONGLONG		long long
#define HAS_LONGLONG		1
#else
#ifdef OSF1
#define STATFS(a, b, c, d )     statvfs( a, b )
#define echochar( ch )          { addch( ch ); refresh(); }
#define LONGLONG		unsigned long
#else
#ifdef SVR4
#define STATFS(a, b, c, d )     statvfs( a, b )
#define LONGLONG		unsigned long
#else
#ifdef SVR3
#define LONGLONG		unsigned long
#define STATFS(a, b, c, d )     statfs( a, b, c, d )
#define LONGLONG		unsigned long
#else
#ifdef _IBMR2
#define STATFS(a, b, c, d )     statfs( a, b )
#define LONGLONG		long long
#define HAS_LONGLONG		1
#else
#if defined(linux)
#define STATFS(a, b, c, d )     statfs( a, b )
#define LONGLONG               long long
#define HAS_LONGLONG           1
#else
#if defined(__GNU__)
#define STATFS(a, b, c, d )     statfs( a, b )
#define LONGLONG		long long
#define HAS_LONGLONG		1
#else
#ifdef hpux
#define STATFS(a, b, c, d )     statfs( a, b )
#define echochar( ch )          { addch( ch ); refresh(); }
#define LONGLONG		long long
#define HAS_LONGLONG		1
#else
#ifdef ultrix
#define STATFS(a, b, c, d )	statfs( a, b )
#define echochar( ch )          { addch( ch ); refresh(); }
#define LONGLONG		unsigned long
#else
#ifdef __QNX__
#define STATFS(a, b, c, d )	statfs( a, b )
#define LONGLONG		unsigned long
#else
#define STATFS(a, b, c, d )     statfs( a, b, c, d )
#define LONGLONG		long long
#define HAS_LONGLONG		1
#endif /* __QNX__ */
#endif /* ultrix */
#endif /* hpux */
#endif /* __GNU__ */
#endif /* linux */
#endif /* _IBMR2 */
#endif /* SVR4 */
#endif /* SVR3 */
#endif /* OSF1 */
#endif /* __APPLE__ */
#endif /* __FreeBSD__ */
#endif /* __NetBSD__ */
#endif /* __OpenBSD__ */


/* Some handy macros... */

#define MINIMUM( a, b ) ( ( (a) < (b) ) ? (a) : (b) )
#define MAXIMUM( a, b ) ( ( (a) > (b) ) ? (a) : (b) )


#ifdef WIN32

#define  S_IREAD         S_IRUSR
#define  S_IWRITE        S_IWUSR
#define  S_IEXEC         S_IXUSR

#define  popen           _popen
#define  pclose          _pclose
#define  sys_errlist     _sys_errlist

#endif /* WIN32 */

#ifdef __NeXT__

#define  S_IRUSR        S_IREAD
#define  S_IWUSR        S_IWRITE
#define  S_IXUSR        S_IEXEC
#define  S_IRGRP        (S_IREAD >> 3)
#define  S_IWGRP        (S_IWRITE >> 3)
#define  S_IXGRP        (S_IEXEC >> 3)
#define  S_IROTH        (S_IREAD >> 6)
#define  S_IWOTH        (S_IWRITE >> 6)
#define  S_IXOTH        (S_IEXEC >> 6)
#define  S_IRWXO        (S_IROTH|S_IWOTH|S_IXOTH)
#define  S_IRWXG        (S_IRGRP|S_IWGRP|S_IXGRP)
#define  S_IRWXU        (S_IRUSR|S_IWUSR|S_IXUSR)

#endif /* __NeXT__ */


#if defined(linux) || defined (__GNU__)
#define HAVE_RENAME
#endif /* linux || __GNU__ */



#ifdef WIN32

/* Diese Funktionen koennen direkt umgesetzt werden */
/*--------------------------------------------------*/

#define  echochar( ch )              { addch( ch ); refresh(); }
#define  putp( str )                 puts( str )


/* ... hier ist ein wenig mehr Arbeit noetig ... */
/*-----------------------------------------------*/

#define  vidattr( attr )


/* ... und hier gibt's keine entsprechende Funktion. */
/*---------------------------------------------------*/

#define  typeahead( file )

#endif /* WIN32 */



#ifdef __DJGPP__

/* DJGPP GNU DOS Compiler                           */
/*--------------------------------------------------*/

#define  putp( str )                 puts( str )
#define  vidattr( attr )
#define  typeahead( file )

#endif /* __DJGPP__*/




#ifdef TERMCAP

#define  KEY_BTAB         5000
#define  KEY_DOWN         5001
#define  KEY_UP           5002
#define  KEY_LEFT         5003
#define  KEY_RIGHT        5004
#define  KEY_END          5005
#define  KEY_HOME         5006
#define  KEY_NPAGE        5007
#define  KEY_PPAGE        5008
#define  KEY_DC           5009
#define  KEY_BACKSPACE    5010
#define  KEY_EIC          5011
#define  KEY_IC           5012
#define  KEY_DL           5013

#define  NO_HIGHLIGHT

#define  A_REVERSE        1 
#define  A_BLINK          2

#define  BELL             0x07



/* Diese Funktionen koennen direkt umgesetzt werden */
/*--------------------------------------------------*/

#undef   cbreak  
#define  cbreak()                    raw()
#define  beep()                      putchar( BELL )
#define  echochar( ch )              { addch( ch ); refresh(); }
#define  putp( str )                 tputs( str, 1, putchar )
#define  wnoutrefresh( win )         wrefresh( win )


/* ... hier ist ein wenig mehr Arbeit noetig ... */
/*-----------------------------------------------*/

#define  wgetch( win )               TermcapWgetch( win )
#define  vidattr( attr )             TermcapVidattr( attr )
#define  initscr()                   TermcapInitscr()
#define  endwin()                    TermcapEndwin()


/* ... und hier gibt's keine entsprechende Funktion. */
/*---------------------------------------------------*/

#define  doupdate() 
#define  wattrset( win, attr ) 
#define  typeahead( file )
#define  keypad( win, flag )

#endif /* TERMCAP */


#ifndef KEY_BTAB
#define KEY_BTAB  0x1d
#endif

#ifndef KEY_END
#define KEY_END   KEY_EOL
#endif

#if defined(_IBMR2) && !defined(_AIX41)
#define echochar( c )  { addch( c ); refresh(); }
#define wgetch( w )    AixWgetch( w )
#endif


#if defined( S_IFLNK ) && !defined( isc386 )
#define STAT_(a, b) lstat(a, b)
#else
#define STAT_(a, b) stat(a, b)
#define readlink( a, b, c )  (-1)
#endif /* S_IFLNK */

#ifndef S_ISREG
#define S_ISREG( mode )   (((mode) & S_IFMT) == S_IFREG)
#endif /* S_ISREG */

#ifndef S_ISDIR
#define S_ISDIR( mode )   (((mode) & S_IFMT) == S_IFDIR)
#endif /* S_ISDIR */

#ifndef S_ISCHR
#define S_ISCHR( mode )   (((mode) & S_IFMT) == S_IFCHR)
#endif /* S_ISCHR */

#ifndef S_ISBLK
#define S_ISBLK( mode )   (((mode) & S_IFMT) == S_IFBLK)
#endif /* S_ISBLK */

#ifndef S_ISFIFO
#define S_ISFIFO( mode )   (((mode) & S_IFMT) == S_IFIFO)
#endif /* S_ISFIFO */

#ifndef S_ISLNK
#ifdef  S_IFLNK
#define S_ISLNK( mode )   (((mode) & S_IFMT) == S_IFLNK)
#else
#define S_ISLNK( mode )   FALSE
#endif /* S_IFLNK */
#endif /* S_ISLNK */

#ifndef S_ISSOCK
#ifdef  S_IFSOCK
#define S_ISSOCK( mode )   (((mode) & S_IFMT) == S_IFSOCK)
#else
#define S_ISSOCK( mode )   FALSE
#endif /* S_IFSOCK */
#endif /* S_ISSOCK */


#define VI_KEY_UP    'k'
#define VI_KEY_DOWN  'j'
#define VI_KEY_RIGHT 'l'
#define VI_KEY_LEFT  'h'
#define VI_KEY_NPAGE ( 'D' & 0x1F )
#define VI_KEY_PPAGE ( 'U' & 0x1F )


#define OWNER_NAME_MAX  64
#define GROUP_NAME_MAX  64
#define DISPLAY_OWNER_NAME_MAX  12
#define DISPLAY_GROUP_NAME_MAX  12


/* Sonderzeichen fuer Liniengrafik */
/*---------------------------------*/

#ifndef ACS_ULCORNER
#define ACS_ULCORNER '+'
#endif
#ifndef ACS_URCORNER
#define ACS_URCORNER '+'
#endif
#ifndef ACS_LLCORNER
#define ACS_LLCORNER '+'
#endif
#ifndef ACS_LRCORNER
#define ACS_LRCORNER '+'
#endif
#ifndef ACS_VLINE   
#define ACS_VLINE    '|'
#endif
#ifndef ACS_HLINE   
#define ACS_HLINE    '-'
#endif
#ifndef ACS_RTEE
#define ACS_RTEE    '+'
#endif
#ifndef ACS_LTEE
#define ACS_LTEE    '+'
#endif
#ifndef ACS_BTEE
#define ACS_BTEE    '+'
#endif
#ifndef ACS_TTEE
#define ACS_TTEE    '+'
#endif
#ifndef ACS_BLOCK
#define ACS_BLOCK   '?'
#endif
#ifndef ACS_LARROW
#define ACS_LARROW  '<'
#endif



/* Color Definitionen */

#define DIR_COLOR        1
#define FILE_COLOR       2
#define STATS_COLOR      3
#define BORDERS_COLOR    4
#define MENU_COLOR       5
#define WINDIR_COLOR     6
#define WINFILE_COLOR    7
#define WINSTATS_COLOR   8
#define WINERR_COLOR     9
#define HIDIR_COLOR     10
#define HIFILE_COLOR    11
#define HISTATS_COLOR   12
#define HIMENUS_COLOR   13
#define WINHST_COLOR    14
#define HST_COLOR       15
#define HIHST_COLOR     16
#define WINMTCH_COLOR   14
#define MTCH_COLOR      15
#define HIMTCH_COLOR    16
#define GLOBAL_COLOR    17
#define HIGLOBAL_COLOR  18


#define PROFILE_FILENAME	".ytree"
#define HISTORY_FILENAME	".ytree-hst"


/* User-Defines */
/*--------------*/

#include "config.h"


/* Auswahl der benutzten UNIX-Kommandos */
/*--------------------------------------*/

#define CAT             GetProfileValue( "CAT" )
#define HEXDUMP         GetProfileValue( "HEXDUMP" )
#define EDITOR          GetProfileValue( "EDITOR" )
#define PAGER           GetProfileValue( "PAGER" )
#define MELT            GetProfileValue( "MELT" )
#define UNCOMPRESS      GetProfileValue( "UNCOMPRESS" )
#define GNUUNZIP        GetProfileValue( "GNUUNZIP" )
#define BUNZIP          GetProfileValue( "BUNZIP" )
#define MANROFF         GetProfileValue( "MANROFF" )
#define TARLIST         GetProfileValue( "TARLIST" )
#define TAREXPAND       GetProfileValue( "TAREXPAND" )
#define RPMLIST         GetProfileValue( "RPMLIST" )
#define RPMEXPAND       GetProfileValue( "RPMEXPAND" )
#define ZOOLIST         GetProfileValue( "ZOOLIST" )
#define ZOOEXPAND       GetProfileValue( "ZOOEXPAND" )
#define ZIPLIST         GetProfileValue( "ZIPLIST" )
#define ZIPEXPAND       GetProfileValue( "ZIPEXPAND" )
#define LHALIST         GetProfileValue( "LHALIST" )
#define LHAEXPAND       GetProfileValue( "LHAEXPAND" )
#define ARCLIST         GetProfileValue( "ARCLIST" )
#define ARCEXPAND       GetProfileValue( "ARCEXPAND" )
#define TREEDEPTH       GetProfileValue( "TREEDEPTH" )
#define USERVIEW        GetProfileValue( "USERVIEW" )
#define RARLIST         GetProfileValue( "RARLIST" )
#define RAREXPAND       GetProfileValue( "RAREXPAND" )
#define FILEMODE        GetProfileValue( "FILEMODE" )
#define NUMBERSEP       GetProfileValue( "NUMBERSEP" )
#define NOSMALLWINDOW   GetProfileValue( "NOSMALLWINDOW" )
#define INITIALDIR      GetProfileValue( "INITIALDIR" )
#define DIR1            GetProfileValue( "DIR1" )
#define DIR2            GetProfileValue( "DIR2" )
#define FILE1           GetProfileValue( "FILE1" )
#define FILE2           GetProfileValue( "FILE2" )
#define SEARCHCOMMAND   GetProfileValue( "SEARCHCOMMAND" )
#define HEXEDITOFFSET   GetProfileValue( "HEXEDITOFFSET" )
#define LISTJUMPSEARCH  GetProfileValue( "LISTJUMPSEARCH" )
 

#define DEFAULT_TREE       "."


#define ERROR_MSG( msg )   Error( msg, __FILE__, __LINE__ ) 
#define WARNING( msg )     Warning( msg ) 
#define MESSAGE( msg )     Message( msg ) 
#define NOTICE( msg )      Notice( msg ) 

#define TAGGED_SYMBOL '*'
#define MAX_MODES      11
#define DISK_MODE      0
#define LL_FILE_MODE   1
#define TAR_FILE_MODE  2
#define ZOO_FILE_MODE  3
#define ZIP_FILE_MODE  4
#define LHA_FILE_MODE  5
#define ARC_FILE_MODE  6
#define RPM_FILE_MODE  7
#define RAR_FILE_MODE  8
#define TAPE_MODE      9 
#define USER_MODE      10 

#define NO_COMPRESS                 0
#define FREEZE_COMPRESS             1
#define MULTIPLE_FREEZE_COMPRESS    2
#define COMPRESS_COMPRESS           3
#define MULTIPLE_COMPRESS_COMPRESS  4
#define GZIP_COMPRESS               5
#define BZIP_COMPRESS               6
#define MULTIPLE_GZIP_COMPRESS      7
#define ZOO_COMPRESS                8
#define LHA_COMPRESS                9
#define ARC_COMPRESS                10
#define ZIP_COMPRESS                11
#define RPM_COMPRESS                12
#define TAPE_DIR_NO_COMPRESS        13
#define TAPE_DIR_FREEZE_COMPRESS    14
#define TAPE_DIR_COMPRESS_COMPRESS  15
#define TAPE_DIR_GZIP_COMPRESS      16
#define TAPE_DIR_BZIP_COMPRESS	    17
#define RAR_COMPRESS                18

#define FILE_EXTENSIONS  {                                           \
			   { ".TAP", TAPE_DIR_NO_COMPRESS },         \
			   { ".tap", TAPE_DIR_NO_COMPRESS },         \
			   { ".TAP.F", TAPE_DIR_FREEZE_COMPRESS },   \
			   { ".tap.F", TAPE_DIR_FREEZE_COMPRESS },   \
			   { ".TAP.Z", TAPE_DIR_COMPRESS_COMPRESS }, \
			   { ".tap.Z", TAPE_DIR_COMPRESS_COMPRESS }, \
			   { ".TAP.z", TAPE_DIR_GZIP_COMPRESS },     \
			   { ".tap.z", TAPE_DIR_GZIP_COMPRESS },     \
			   { ".tap.gz", TAPE_DIR_GZIP_COMPRESS },    \
			   { ".tap.bz2", TAPE_DIR_BZIP_COMPRESS },   \
			   { ".TAP.BZ2", TAPE_DIR_BZIP_COMPRESS },   \
			   { ".F",   FREEZE_COMPRESS },              \
			   { ".TFR", FREEZE_COMPRESS },              \
			   { ".Faa", MULTIPLE_FREEZE_COMPRESS },     \
			   { ".Z",   COMPRESS_COMPRESS },            \
			   { ".TZ",  COMPRESS_COMPRESS },            \
			   { ".TZR", COMPRESS_COMPRESS },            \
			   { ".Xaa", MULTIPLE_COMPRESS_COMPRESS },   \
			   { ".bz2", BZIP_COMPRESS },                \
			   { ".z",   GZIP_COMPRESS },                \
			   { ".gz",  GZIP_COMPRESS },                \
			   { ".tz",  GZIP_COMPRESS },                \
			   { ".tzr", GZIP_COMPRESS },                \
			   { ".tgz", GZIP_COMPRESS },                \
			   { ".TGZ", GZIP_COMPRESS },                \
			   { ".taz", GZIP_COMPRESS },                \
			   { ".TAZ", GZIP_COMPRESS },                \
			   { ".tpz", GZIP_COMPRESS },                \
			   { ".TPZ", GZIP_COMPRESS },                \
			   { ".xaa", MULTIPLE_GZIP_COMPRESS },       \
			   { ".zoo", ZOO_COMPRESS },                 \
			   { ".ZOO", ZOO_COMPRESS },                 \
			   { ".lzh", LHA_COMPRESS },                 \
			   { ".LZH", LHA_COMPRESS },                 \
			   { ".arc", ARC_COMPRESS },                 \
			   { ".ARC", ARC_COMPRESS },                 \
			   { ".rar", RAR_COMPRESS },                 \
			   { ".RAR", RAR_COMPRESS },                 \
			   { ".jar", ZIP_COMPRESS },                 \
			   { ".zip", ZIP_COMPRESS },                 \
			   { ".ZIP", ZIP_COMPRESS },                 \
			   { ".JAR", ZIP_COMPRESS },                 \
			   { ".rpm", RPM_COMPRESS },                 \
			   { ".RPM", RPM_COMPRESS },                 \
			   { ".spm", RPM_COMPRESS },                 \
			   { ".SPM", RPM_COMPRESS }                  \
			 }



#define SORT_BY_NAME       1
#define SORT_BY_MOD_TIME   2
#define SORT_BY_CHG_TIME   3
#define SORT_BY_ACC_TIME   4
#define SORT_BY_SIZE       5
#define SORT_BY_OWNER      6
#define SORT_BY_GROUP      7
#define SORT_BY_EXTENSION  8
#define SORT_ASC           10
#define SORT_DSC           20
#define SORT_CASE          40
#define SORT_ICASE         80

#define DEFAULT_FILE_SPEC "*"

#define TAGSYMBOL_VIEWNAME	"tag"
#define FILENAME_VIEWNAME	"fnm"
#define ATTRIBUTE_VIEWNAME	"atr"
#define LINKCOUNT_VIEWNAME	"lct"
#define FILESIZE_VIEWNAME	"fsz"
#define MODTIME_VIEWNAME	"mot"
#define SYMLINK_VIEWNAME	"lnm"
#define UID_VIEWNAME		"uid"
#define GID_VIEWNAME		"gid"
#define INODE_VIEWNAME		"ino"
#define ACCTIME_VIEWNAME	"act"
#define CHGTIME_VIEWNAME	"sct"


#define BLKSIZ             512  /* Blockgroesse fuer SVR3 */

#define CLOCK_INTERVAL	   1

#define FILE_SEPARATOR_CHAR   '/'
#define FILE_SEPARATOR_STRING "/"

#define ERR_TO_NULL           " 2> /dev/null"
#define ERR_TO_STDOUT         " 2>&1 "

#define BOOL       unsigned char
#define LF         10
#define ESC        27
#define LOGIN_ESC  '.'

#ifdef  sgi
#define SGI_CR     0x157    /* Irix 3.2.2 special ? */
#endif

#define CR                     13

#define DIR_WINDOW_X         1  
#define DIR_WINDOW_Y         2 
#define DIR_WINDOW_WIDTH     (COLS - 26)
#define DIR_WINDOW_HEIGHT    ((LINES * 8 / 14)-1)

#define F2_WINDOW_X          DIR_WINDOW_X
#define F2_WINDOW_Y          DIR_WINDOW_Y
#define F2_WINDOW_WIDTH      DIR_WINDOW_WIDTH
#define F2_WINDOW_HEIGHT     (DIR_WINDOW_HEIGHT + 1)

#define FILE_WINDOW_1_X      1
#define FILE_WINDOW_1_Y      DIR_WINDOW_HEIGHT + 3
#define FILE_WINDOW_1_WIDTH  (COLS - 26)
#define FILE_WINDOW_1_HEIGHT (LINES - DIR_WINDOW_HEIGHT - 7 )

#define FILE_WINDOW_2_X      1
#define FILE_WINDOW_2_Y      2
#define FILE_WINDOW_2_WIDTH  (COLS - 26)
#define FILE_WINDOW_2_HEIGHT (LINES - 6) 

#define ERROR_WINDOW_WIDTH   40
#define ERROR_WINDOW_HEIGHT  10
#define ERROR_WINDOW_X       ((COLS - ERROR_WINDOW_WIDTH) >> 1)  
#define ERROR_WINDOW_Y       ((LINES - ERROR_WINDOW_HEIGHT) >> 1)

#define HISTORY_WINDOW_X       1
#define HISTORY_WINDOW_Y       2
#define HISTORY_WINDOW_WIDTH   (COLS - 26)
#define HISTORY_WINDOW_HEIGHT  (LINES - 6)

#define MATCHES_WINDOW_X       1
#define MATCHES_WINDOW_Y       2
#define MATCHES_WINDOW_WIDTH   (COLS - 26)
#define MATCHES_WINDOW_HEIGHT  (LINES - 6)

#define TIME_WINDOW_X        ((COLS > 20) ? (COLS - 20) : 1)
#define TIME_WINDOW_Y        1
#define TIME_WINDOW_WIDTH    ((COLS > 15) ? 15 : COLS)
#define TIME_WINDOW_HEIGHT   1


#define PATH_LENGTH            1024
#define FILE_SPEC_LENGTH       (12 + 1)
#define DISK_NAME_LENGTH       (12 + 1)
#define LL_LINE_LENGTH         512
#define TAR_LINE_LENGTH        512
#define RPM_LINE_LENGTH        512
#define ZOO_LINE_LENGTH        512
#define ZIP_LINE_LENGTH        512
#define LHA_LINE_LENGTH        512
#define ARC_LINE_LENGTH        512
#define RAR_LINE_LENGTH        512
#define MESSAGE_LENGTH         (PATH_LENGTH + 80 + 1)
#define COMMAND_LINE_LENGTH    4096
#define MODE_1                 0
#define MODE_2                 1
#define MODE_3                 2
#define MODE_4                 3
#define MODE_5                 4


#ifdef __NeXT__
#include <sys/ttydev.h>
#define QUICK_BAUD_RATE      B9600
#else
#define QUICK_BAUD_RATE      9600
#endif /* __NeXT__ */

#define ESCAPE               goto FNC_XIT

#define PRINT(ch) (iscntrl(ch) && (((unsigned char)(ch)) < ' ')) ? (ACS_BLOCK) : ((unsigned char)(ch)) 
/* #define PRINT(ch) (ch) */

#ifdef COLOR_SUPPORT
extern void StartColors(void);
extern void WbkgdSet(WINDOW *w, chtype c);
#else
#define StartColors()	;
#define WbkgdSet(a, b)  ;
#endif /* COLOR_SUPPORT */



typedef struct _file_entry
{
  struct _file_entry *next;
  struct _file_entry *prev;
  struct _dir_entry  *dir_entry;
  struct stat        stat_struct;
  BOOL               tagged;
  BOOL               matching;
  char               name[1];
/*char               symlink_name[]; */ /* Folgt direkt dem Namen, falls */
					/* Eintrag == symbolischer Link  */
} FileEntry;



typedef struct _dir_entry
{
  struct _file_entry *file;
  struct _dir_entry  *next;
  struct _dir_entry  *prev;
  struct _dir_entry  *sub_tree;
  struct _dir_entry  *up_tree;
  LONGLONG           total_bytes;
  LONGLONG           matching_bytes;
  LONGLONG           tagged_bytes;
  unsigned int       total_files;
  unsigned int       matching_files;
  unsigned int       tagged_files;
  int                cursor_pos;
  int                start_file;
  struct   stat      stat_struct;
  BOOL               access_denied;
  BOOL               global_flag;
  BOOL               tagged_flag;
  BOOL               only_tagged;
  BOOL               not_scanned;
  BOOL               big_window;
  BOOL               login_flag;
  char               name[1];
} DirEntry;



typedef struct
{
  unsigned long      indent;
  DirEntry           *dir_entry;
  unsigned short     level;
} DirEntryList;


typedef struct
{
  FileEntry          *file;
} FileEntryList;


typedef struct 
{
  DirEntry      *tree;
  LONGLONG	disk_space;
  LONGLONG	disk_capacity;
  LONGLONG	disk_total_files;
  LONGLONG	disk_total_bytes;
  LONGLONG	disk_matching_files;
  LONGLONG	disk_matching_bytes;
  LONGLONG	disk_tagged_files;
  LONGLONG 	disk_tagged_bytes;
  unsigned int  disk_total_directories;
  int           disp_begin_pos;
  int           cursor_pos;
  int           kind_of_sort;
  char          login_path[PATH_LENGTH + 1];
  char          path[PATH_LENGTH + 1];
  char          tape_name[PATH_LENGTH + 1];
  char          file_spec[FILE_SPEC_LENGTH + 1];
  char          disk_name[DISK_NAME_LENGTH + 1];
} Statistic;


typedef union
{
  struct
  {
    char      new_modus[11];
  } change_modus;

  struct
  {
    unsigned  new_owner_id;
  } change_owner;

  struct
  {
    unsigned  new_group_id;
  } change_group;

  struct
  {
    char      *command;
  } execute;

  struct
  {
    Statistic *statistic_ptr;
    DirEntry  *dest_dir_entry;
    char      *to_file;
    char      *to_path;
    BOOL      path_copy;
    BOOL      confirm;
  } copy;

  struct
  {
    char      *new_name;
    BOOL      confirm;
  } rename;

  struct
  {
    DirEntry  *dest_dir_entry;
    char      *to_file;
    char      *to_path;
    BOOL      confirm;
  } mv;

  struct
  {
    FILE      *pipe_file;
  } pipe_cmd;

  struct
  {
   FILE       *zipfile;
   int        method;
   } compress_cmd;

} FunctionData;


typedef struct
{
  FileEntry     *new_fe_ptr;
  FunctionData  function_data;
} WalkingPackage;

/* strerror() is POSIX, and all modern operating systems provide it.  */
#define HAVE_STRERROR 1

#ifndef HAVE_STRERROR
extern const char *StrError(int);
#endif /* HAVE_STRERROR */


extern WINDOW *dir_window;
extern WINDOW *small_file_window;
extern WINDOW *big_file_window;
extern WINDOW *file_window;
extern WINDOW *error_window;
extern WINDOW *history_window;
extern WINDOW *matches_window;
extern WINDOW *f2_window;
extern WINDOW *time_window;

extern Statistic statistic;
extern Statistic disk_statistic;
extern int       mode;
extern int       user_umask;
extern char      message[];
extern BOOL	 print_time;
extern BOOL      resize_request;
extern char      number_seperator;
extern BOOL      bypass_small_window;
extern char      *initial_directory;
extern char 	 builtin_hexdump_cmd[];


#if defined(ultrix)
extern char *getenv(char *);
#else
extern char *getenv(const char *);
#endif

extern int  ytree(int argc, char *argv[]);
extern void DisplayMenu(void);
extern void DisplayDiskStatistic(void);
extern void DisplayDirStatistic(DirEntry *dir_entry);
extern void DisplayDirParameter(DirEntry *dir_entry);
extern void DisplayDirTagged(DirEntry *dir_entry);
extern void DisplayDiskTagged(void);
extern void DisplayDiskName(void);
extern void DisplayFileParameter(FileEntry *file_entry);
extern void DisplayGlobalFileParameter(FileEntry *file_entry);
extern void RefreshWindow(WINDOW *win);
extern int  ReadTree(DirEntry *dir_entry, char *path, int depth);
extern void UnReadTree(DirEntry *dir_entry);
extern int  ReadTreeFromTAR(DirEntry *dir_entry, FILE *f);
extern int  ReadTreeFromRPM(DirEntry *dir_entry, FILE *f);
extern int  ReadTreeFromZOO(DirEntry *dir_entry, FILE *f);
extern int  ReadTreeFromZIP(DirEntry *dir_entry, FILE *f);
extern int  ReadTreeFromLHA(DirEntry *dir_entry, FILE *f);
extern int  ReadTreeFromARC(DirEntry *dir_entry, FILE *f);
extern int  ReadTreeFromRAR(DirEntry *dir_entry, FILE *f);
extern int  GetDiskParameter(char *path, 
			     char *volume_name, 
			     LONGLONG *avail_bytes,
			     LONGLONG *capacity
			    );
extern int  HandleDirWindow(DirEntry *start_dir_entry);
extern void DisplayFileWindow(DirEntry *dir_entry);
extern int Init(char *configuration_file, char *history_file);
extern char *GetPath(DirEntry *dir_entry, char *buffer);
extern BOOL Match(char *file_name);
extern int  SetMatchSpec(char *new_spec);
extern int  SetFileSpec(char *file_spec);
extern void SetMatchingParam(DirEntry *dir_entry);
extern void Error(char *msg, char *module, int line);
extern void Warning(char *msg);
extern void Notice(char *msg);
extern void UnmapNoticeWindow(void);
extern void SetFileMode(int new_file_mode);
extern int  HandleFileWindow(DirEntry *dir_entry);
extern char *GetAttributes(unsigned short modus, char *buffer);
extern void SwitchToSmallFileWindow(void);
extern void SwitchToBigFileWindow(void);
extern int  ReadGroupEntries(void);
extern char *GetGroupName(unsigned int gid);
extern char *GetDisplayGroupName(unsigned int gid);
extern int  GetGroupId(char *name);
extern int  ReadPasswdEntries(void);
extern char *GetPasswdName(unsigned int uid);
extern char *GetDisplayPasswdName(unsigned int uid);
extern int  GetPasswdUid(char *name);
extern char *GetFileNamePath(FileEntry *file_entry, char *buffer);
extern char *GetRealFileNamePath(FileEntry *file_entry, char *buffer);
extern int  SystemCall(char *command_line);
extern int  QuerySystemCall(char *command_line);
extern int  SilentSystemCall(char *command_line);
extern int  SilentSystemCallEx(char *command_line, BOOL enable_clock);
extern int  View(DirEntry * dir_entry, char *file_path);
extern int  ViewHex(char *file_path);
extern int  InternalView(char *file_path);
extern int  Edit(DirEntry * dir_entry, char *file_path);
extern void DisplayAvailBytes(void);
extern void DisplayFileSpec(void);
extern void QuitTo(DirEntry * dir_entry);
extern void Quit(void);
extern int  ReadFileSpec(void);
extern int  InputString(char *s, int y, int x, int cursor_pos, int length, char *term);
extern void RotateFileMode(void);
extern int  Execute(DirEntry *dir_entry, FileEntry *file_entry);
extern int  Pipe(DirEntry *dir_entry, FileEntry *file_entry);
extern int  PipeTaggedFiles(FileEntry *fe_ptr, WalkingPackage *walking_package);
extern int  GetPipeCommand(char *pipe_command);
extern void GetKindOfSort(void);
extern void SetKindOfSort(int new_kind_of_sort);
extern int  ChangeFileModus(FileEntry *fe_ptr);
extern int  ChangeDirModus(DirEntry *de_ptr);
extern int  GetNewFileModus(int y, int x, char *modus, char *term);
extern int  GetModus(char *modus);
extern int  SetFileModus(FileEntry *fe_ptr, WalkingPackage *walking_package);
extern int  CopyTaggedFiles(FileEntry *fe_ptr, WalkingPackage *walking_package);
extern int  CopyFile(Statistic *statistic_ptr, FileEntry *fe_ptr, unsigned char confirm, char *to_file, DirEntry *dest_dir_entry, char *to_dir_path, BOOL path_copy);
extern int  MoveTaggedFiles(FileEntry *fe_ptr, WalkingPackage *walking_package);
extern int  MoveFile(FileEntry *fe_ptr, unsigned char confirm, char *to_file, DirEntry *dest_dir_entry, char *to_dir_path, FileEntry **new_fe_ptr);
extern int  InputChoise(char *msg, char *term);
extern void Message(char *msg);
extern int  GetDirEntry(DirEntry *tree, DirEntry *current_dir_entry, char *dir_path, DirEntry **dir_entry, char *to_path);
extern int  GetFileEntry(DirEntry *de_ptr, char *file_name, FileEntry **file_entry);
extern int  GetCopyParameter(char *from_file, BOOL path_copy, char *to_file, char *to_dir);
extern int  GetMoveParameter(char *from_file, char *to_file, char *to_dir);
extern int  ChangeFileOwner(FileEntry *fe_ptr);
extern int  GetNewOwner(int st_uid);
extern int  SetFileOwner(FileEntry *fe_ptr, WalkingPackage *walking_package);
extern int  ChangeDirOwner(DirEntry *de_ptr);
extern int  ChangeFileGroup(FileEntry *fe_ptr);
extern int  GetNewGroup(int st_gid);
extern int  SetFileGroup(FileEntry *fe_ptr, WalkingPackage *walking_package);
extern int  ChangeDirGroup(DirEntry *de_ptr);
extern void DisplayDirHelp(void);
extern void DisplayFileHelp(void);
extern void ClearHelp(void);
extern int  GetAvailBytes(LONGLONG *avail_bytes);
extern int  DeleteDirectory(DirEntry *dir_entry);
extern int  ExecuteCommand(FileEntry *fe_ptr, WalkingPackage *walking_package);
extern int  GetCommandLine(char *command_line);
extern int  GetSearchCommandLine(char *command_line);
extern int  DeleteFile(FileEntry *fe_ptr);
extern int  RemoveFile(FileEntry *fe_ptr);
extern int  RenameDirectory(DirEntry *de_ptr, char *new_name);
extern int  RenameFile(FileEntry *fe_ptr, char *new_name, FileEntry **new_fe_ptr);
extern int  RenameTaggedFiles(FileEntry *fe_ptr, WalkingPackage *walking_package);
extern int  GetRenameParameter(char *old_name, char *new_name);
extern char *CTime(time_t f_time, char *buffer);
extern int  LoginDisk(char *path);
extern int  GetNewLoginPath(char *path);
extern void PrintSpecialString(WINDOW *win, int y, int x, char *str, int color);
extern void Print(WINDOW *, int, int, char *, int);
extern void PrintOptions(WINDOW *,int, int, char *);
extern void PrintMenuOptions(WINDOW *,int, int, char *, int, int);
extern char *FormFilename(char *dest, char *src, unsigned int max_len);
extern char *CutFilename(char *dest, char *src, unsigned int max_len);
extern char *CutPathname(char *dest, char *src, unsigned int max_len);
extern void   Fnsplit(char *path, char *dir, char *name);
extern void MakeExtractCommandLine(char *command_line, char *path, char *file, char *cmd);
extern int MakeDirectory(DirEntry *father_dir_entry);
extern time_t Mktime(struct tm *tm);
extern int TryInsertArchiveDirEntry(DirEntry *tree, char *dir, struct stat *stat);
extern int InsertArchiveFileEntry(DirEntry *tree, char *path, struct stat *stat);
extern int MinimizeArchiveTree(DirEntry *tree);
extern void HitReturnToContinue(void);
extern int  TermcapWgetch(WINDOW *win);
extern void TermcapVidattr(int attr );
extern void TermcapInitscr(void);
extern void TermcapEndwin(void);
extern int  BuildFilename( char *in_filename, char *pattern, char *out_filename);
extern int  ViKey( int ch );
extern int  GetFileMethod( char *filename );
extern int  AixWgetch( WINDOW *w );
extern BOOL KeyPressed(void);
extern BOOL EscapeKeyPressed(void);
extern int  GetTapeDeviceName(void);
extern int  MakePath( DirEntry *tree, char *dir_path, DirEntry **dest_dir_entry );
extern int  MakeDirEntry( DirEntry *father_dir_entry, char *dir_name );
extern void NormPath( char *in_path, char *out_path );
extern char *Strtok_r( char *str, char *delim, char **old );
extern int  ReadProfile( char *filename );
extern char *GetProfileValue( char *key );
extern int  ScanSubTree( DirEntry *dir_entry );
extern void GetMaxYX(WINDOW *win, int *height, int *width);

extern char *GetHistory(void);
extern void InsHistory(char *new_hist);
extern void ReadHistory(char *filename);
extern void SaveHistory(char *filename);
extern char *GetMatches(char *);
extern int  KeyF2Get(DirEntry *start_dir_entry,
               int disp_begin_pos,
               int cursor_pos,
               char *path);
extern void Switch2F2Window(void);
extern void MapF2Window(void);
extern void UnmapF2Window(void);
extern void ReadExtFile(char *);
extern char *GetExtCmd(char *);
extern int  MvAddStr(int y, int x, char *str);
extern int  MvWAddStr(WINDOW *win, int y, int x, char *str);
extern int  WAddStr(WINDOW *win, char *str);
extern int  AddStr(char *str);
extern void ClockHandler(int);
extern int Strrcmp(char *s1, char* s2);
extern char *Strdup(const char *s);
extern char *GetExtViewer(char *filename);
extern void InitClock(void);
extern void SuspendClock(void);
extern char *GetExtension(char *filename);
extern void StrCp(char *dest, const char *src);
extern int  BuildUserFileEntry(FileEntry *fe_ptr, 
            int max_filename_len, int max_linkname_len, 
            char *template, int linelen, char *line);
extern int  GetUserFileEntryLength(int max_filename_len, 
				   int max_linkname_len, char *template);
extern LONGLONG AtoLL(char* cptr);
extern void DisplayTree(WINDOW *win, int start_entry_no, int hilight_no);
extern void ReCreateWindows(void);
extern int  Getch(void);
extern int  DirUserMode(DirEntry *dir_entry, int ch);
extern int  FileUserMode(FileEntryList *file_entry_list, int ch);
extern char *GetUserFileAction(int chkey, int *pchremap);
extern char *GetUserDirAction(int chkey, int *pchremap);
extern BOOL IsUserActionDefined(void);
extern char *Getcwd(char *buffer, unsigned int len);
extern int  RefreshDirWindow();
extern char *StrLeft(const char *str, size_t count);
extern int  StrVisualLength(const char *str);
extern int  WAttrAddStr(WINDOW *win, int attr, char *str);
extern char *Strndup(const char *s, int len);
extern char *CutName(char *dest, char *src, unsigned int max_len);
