#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

#include <termios.h>



int
main(void)
{
   fd_set rfds;
   struct timeval tv;
   int retval;


struct termios info;
tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
info.c_lflag &= ~ICANON;      /* disable canonical mode */
info.c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
info.c_cc[VTIME] = 0;         /* no timeout */
tcsetattr(0, TCSANOW, &info); /* set immediately */


   /* Watch stdin (fd 0) to see when it has input. */

   FD_ZERO(&rfds);
   FD_SET(0, &rfds);

   /* Wait up to five seconds. */

   tv.tv_sec = 5;
   tv.tv_usec = 0;

   retval = select(1, &rfds, NULL, NULL, &tv);
   /* Don't rely on the value of tv now! */

   if (retval == -1)
       perror("select()");
   else if (retval)
       printf("Data is available now.\n");
       /* FD_ISSET(0, &rfds) will be true. */
   else
       printf("No data within five seconds.\n");

//return console window back to canonical mode
tcgetattr(0, &info);
info.c_lflag |= ICANON;
tcsetattr(0, TCSANOW, &info);

   exit(EXIT_SUCCESS);
}
