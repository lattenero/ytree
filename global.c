/***************************************************************************
 *
 * $Header: /usr/local/cvsroot/utils/ytree/global.c,v 1.13 2003/08/31 11:11:00 werner Exp $
 *
 * Externe Groessen
 *
 ***************************************************************************/


#include "ytree.h"



WINDOW *dir_window;
WINDOW *small_file_window;
WINDOW *big_file_window;
WINDOW *file_window;
WINDOW *error_window;
WINDOW *history_window;
WINDOW *matches_window;
WINDOW *f2_window;
WINDOW *time_window;

Statistic statistic;
Statistic disk_statistic;
int       mode; 
int	  user_umask;
char      message[MESSAGE_LENGTH + 1];
BOOL	  print_time;
BOOL	  resize_request;
BOOL	  bypass_small_window;
char   number_seperator;
char	  *initial_directory;
