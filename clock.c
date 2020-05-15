/***************************************************************************
 *
 * $Header: /usr/local/cvsroot/utils/ytree/clock.c,v 1.4 2003/08/31 11:11:00 werner Exp $
 *
 * Clock Module
 *
 ***************************************************************************/


#include "ytree.h"



void InitClock()
{

#ifdef CLOCK_SUPPORT

  struct itimerval value, ovalue;
  print_time = TRUE;

  if (getitimer(ITIMER_REAL, &value)!= 0) {
      sprintf(message,"getitimer() failed: %s", strerror(errno));
      ERROR_MSG(message);
  }
  value.it_interval.tv_sec = CLOCK_INTERVAL;
  value.it_value.tv_sec = CLOCK_INTERVAL;
  value.it_interval.tv_usec = 0;
  
  if (setitimer(ITIMER_REAL, &value, &ovalue)!= 0) {
      sprintf(message,"setitimer() failed: %s", strerror(errno));
      ERROR_MSG(message);
  }
  ClockHandler(0);
#endif
}




void ClockHandler(int sig)
{
#ifdef CLOCK_SUPPORT

   char strtm[23];
   time_t HORA;
   struct tm *hora;

   if(COLS > 15 && print_time)
   {
      time(&HORA);
      hora = localtime(&HORA);
      *strtm = '\0';
      
      sprintf(strtm,"[time %.2d:%.2d:%.2d]",hora->tm_hour,hora->tm_min,hora->tm_sec);

#ifdef COLOR_SUPPORT
      mvwaddch(time_window, 0, 0, ACS_RTEE| COLOR_PAIR(MENU_COLOR)|A_BOLD);
      mvwaddch(time_window, 0, 14, ACS_LTEE| COLOR_PAIR(MENU_COLOR)|A_BOLD);
#else
      mvwaddch(time_window, 0, 0, ACS_RTEE);
      mvwaddch(time_window, 0, 14, ACS_LTEE);
#endif
      PrintMenuOptions(time_window,0, 1, strtm, MENU_COLOR, HIMENUS_COLOR);
   }
   signal(SIGALRM,  ClockHandler );
#endif
}

void SuspendClock(void)
{
#ifdef CLOCK_SUPPORT
  struct itimerval value, ovalue;
  value.it_interval.tv_sec = 0;
  value.it_value.tv_sec = 0;
  value.it_interval.tv_usec = 0;
  setitimer(ITIMER_REAL, &value, &ovalue);
  signal(SIGALRM, SIG_IGN);
#endif
  return;
  }
  
