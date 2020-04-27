/***************************************************************
 **
 **   DLSTORM   Deadline's Code Storm Library
 **
 **          /\
 **   ---- D/L \----
 **       \/
 **
 **   License:      BSD
 **   Copyright:    2017
 **   File:         sys/linux.h
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_SYS_LINUX
#define _DLCS_SYS_LINUX

#define MAC_STATIC
#define PATH_SEP '/'
#undef  DEL
#define DEL      delete

#include <arpa/inet.h>
#include <ctype.h>
#include <curses.h> // apt-get install libncurses5-dev
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <tcl.h> // apt install tcl-dev
#include <term.h> // apt install libncurses5-dev
#include <termios.h>
#include <unistd.h>

#endif  // _DLCS_SYS_LINUX
