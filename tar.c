/***************************************************************************
 *
 * $Header: /usr/local/cvsroot/utils/ytree/tar.c,v 1.11 2000/05/20 20:41:11 werner Exp $
 *
 * Funktionen zum Lesen des Dateibaumes aus TAR-Dateien
 *
 ***************************************************************************/


#include "ytree.h"



static int GetStatFromTAR(char *tar_line, char *name, struct stat *stat);



/* Dateibaum aus TAR-Listing lesen */
/*---------------------------------*/

int ReadTreeFromTAR(DirEntry *dir_entry, FILE *f)
{ 
  char tar_line[TAR_LINE_LENGTH + 1];
  char path_name[PATH_LENGTH +1];
  struct stat stat;
  BOOL   dir_flag = FALSE;

  *dir_entry->name = '\0';

  while( fgets( tar_line, TAR_LINE_LENGTH, f ) != NULL )
  {
    /* \n loeschen */
    /*-------------*/

    tar_line[ strlen( tar_line ) - 1 ] = '\0';

    if( GetStatFromTAR( tar_line, path_name, &stat ) )
    {
      (void) sprintf( message, "unknown tarinfo*%s", tar_line );
      MESSAGE( message );
    }
    else
    {
      if( (path_name[strlen( path_name ) - 1] == FILE_SEPARATOR_CHAR) ||
	  !strcmp( path_name, "." ) ||
	  *tar_line == 'd' 
	)
      {
        /* Directory */
        /*-----------*/
        
#ifdef DEBUG
  fprintf( stderr, "DIR: %s\n", path_name );
#endif

	if( strcmp( path_name, "./" ) )
	{
	  /* "./" wird ignoriert */
	  /*---------------------*/

          (void) TryInsertArchiveDirEntry( dir_entry, path_name, &stat );
	  DisplayDiskStatistic();
	  doupdate();
	}
      }
      else
      {
        /* File */
        /*------*/
  
#ifdef DEBUG
  fprintf( stderr, "FILE: \"%s\"\n", path_name );
#endif
        (void) InsertArchiveFileEntry( dir_entry, path_name, &stat );
      }
    }
  }
  
  if( dir_flag == FALSE )
  {
    statistic.disk_total_directories++;
    (void) memset( (char *) &dir_entry->stat_struct, 0, sizeof( struct stat ) );
    dir_entry->stat_struct.st_mode = S_IFDIR;
  }
  return( MinimizeArchiveTree( dir_entry ) );
}





static int GetStatFromTAR(char *tar_line, char *name, struct stat *stat)
{
  char *t, *old;
  int  i, id;
  struct tm tm_struct;
  static char *month[] = { "Jan", "Feb", "Mar", "Apr", "Mai", "Jun",
	 	           "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };


  (void) memset( stat, 0, sizeof( struct stat ) );

  stat->st_nlink = 1;

  t = Strtok_r( tar_line, " \t", &old ); if( t == NULL ) return( -1 );

  /* Attribute */
  /*-----------*/

  if( strlen( t ) != 10 ) return( -1 );
  stat->st_mode = GetModus( t );
  t = Strtok_r( NULL, " \t/", &old ); if( t == NULL ) return( -1 );


  /* Owner */
  /*-------*/

  id = GetPasswdUid( t );
  if( id == -1 ) id = atoi( t );
  stat->st_uid = (unsigned) id;

  t = Strtok_r( NULL, " \t", &old ); if( t == NULL ) return( -1 );

  /* Group */
  /*-------*/

  id = GetGroupId( t );
  if( id == -1 ) id = atoi( t );
  stat->st_gid = (unsigned) id;
  
  t = Strtok_r( NULL, " \t", &old ); if( t == NULL ) return( -1 );

  /* Dateilaenge */
  /*-------------*/

  if( !isdigit( *t ) ) return( -1 );
  stat->st_size = AtoLL( t );
  t = Strtok_r( NULL, " \t", &old ); if( t == NULL ) return( -1 );

  /* M-Datum */
  /*---------*/

  for( i=0; i < 12; i++ )
  {
    if( !strcmp( t, month[i] ) ) break;
  }
  if( i >= 12 ) 
  {
    t[4] = t[7] = '\0';
      
    tm_struct.tm_year = atoi(t) - 1900;
    tm_struct.tm_mon  = atoi(&t[5]) - 1;
    tm_struct.tm_mday = atoi(&t[8]);
    
    t = Strtok_r( NULL, " \t:", &old ); if( t == NULL ) return( -1 );
    tm_struct.tm_hour = atoi( t );
    t = Strtok_r( NULL, " \t", &old ); if( t == NULL ) return( -1 );
    tm_struct.tm_min = atoi( t );
    t = Strtok_r( NULL, " \t", &old ); if( t == NULL ) return( -1 );
    goto XDATE;
  } 
    

  tm_struct.tm_mon = i;
  t = Strtok_r( NULL, " \t", &old ); if( t == NULL ) return( -1 );

  tm_struct.tm_mday = atoi( t );
  t = Strtok_r( NULL, " \t:", &old ); if( t == NULL ) return( -1 );

  tm_struct.tm_hour = atoi( t );
  t = Strtok_r( NULL, " \t", &old ); if( t == NULL ) return( -1 );

  tm_struct.tm_min = atoi( t );
  t = Strtok_r( NULL, " \t", &old ); if( t == NULL ) return( -1 );
  
  tm_struct.tm_year = atoi( t ) - 1900;
  t = Strtok_r( NULL, " \t", &old ); if( t == NULL ) return( -1 );
  
XDATE:

  tm_struct.tm_sec = 0;
  tm_struct.tm_isdst = -1;

  stat->st_atime = 0;
  stat->st_ctime = 0;

  stat->st_mtime = Mktime( &tm_struct );

  /* Dateiname */
  /*-----------*/

  (void) strcpy( name, t );


  if( S_ISLNK( stat->st_mode ) )
  {
    /* Symbolischer Link */
    /*-------------------*/

    t = Strtok_r( NULL, " \t", &old ); if( t == NULL ) return( -1 ); 
    t = Strtok_r( NULL, " \t", &old ); if( t == NULL ) return( -1 );
    (void) strcpy( &name[ strlen( name ) + 1 ], t );
  }

  return( 0 );
}


