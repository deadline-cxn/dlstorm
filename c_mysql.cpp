/***************************************************************
 **
 **   DLSTORM   Deadline's Code Storm Library
 **
 **          /\
 **   ---- D/L \----
 **       \/
 **
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_mysql.cpp
 **   Description:  MySQL class wrapper
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ** You must compile with `mysql_config --cflags --libs`
 **
 ******************************************************************************/
 #include "c_mysql.h"
 //#include <mysql.h>
 ///#include <my_global.h>
C_MySQL::C_MySQL(char *host, char *user, char *pass, char *db) {
    con = mysql_init(NULL); // if(con==NULL) { fprintf(stderr, "%s\n", mysql_error(con)); exit(1); }
    mysql_real_connect(con, host, user, pass, db, 0, NULL, 0);
};
C_MySQL::~C_MySQL() {
    mysql_close(con);
};
MYSQL_RES *C_MySQL::Query(char *q){
    mysql_query(con, q);
    return (mysql_store_result(con));
};
MYSQL_ROW C_MySQL::Fetch_Object(MYSQL_RES *result) {
    return mysql_fetch_row(result);
};




