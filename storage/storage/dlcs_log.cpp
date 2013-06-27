/***************************************************************
 Deadline's Code Storm (Log Class)
 ***************************************************************/

#include "dlcs_log.h"

#ifdef _WIN32
#include <direct.h>
#endif
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef _WIN32
#include <unistd.h>
#endif

//////////////////////////////////////////////////////
C_DLCS_Log::C_DLCS_Log() { Initialize();}
//////////////////////////////////////////////////////
C_DLCS_Log::C_DLCS_Log(char *szFilename) { Initialize(); SetName(szFilename); }
//////////////////////////////////////////////////////
C_DLCS_Log::~C_DLCS_Log() { }
//////////////////////////////////////////////////////
void C_DLCS_Log::Initialize(void)
{
    if( getcwd( logfile, _MAX_PATH ) != NULL )
        strcpy(logdir,logfile);
    strcat(logfile,"[C_DLCS_Log].log");
    strcpy(szBegin,"(");
    strcpy(szEnd,"):");
    strcpy(szLineSep,"*****************************************************************");
    bActive = true;
    bLineFeeds = false;
}
//////////////////////////////////////////////////////
void C_DLCS_Log::SetName(char *szFilename)
{
    if( getcwd( logfile, _MAX_PATH ) != NULL) strcpy(logdir,logfile);
    strcpy(logfile,szFilename);
}
//////////////////////////////////////////////////////
void C_DLCS_Log::AddEntry(char *fmt, ...)
{
    if(!bActive) return;
    char ach[512];
    char temp[512];
    va_list va;
    va_start( va, fmt );
    vsprintf( ach, fmt, va );
    va_end( va );
    time_t td;
    struct tm *dc;
    time(&td);
    dc=localtime(&td);
    strcpy(temp,asctime(dc));
    temp[strlen(temp)-1]=0;
    FILE *fp=fopen(logfile,"a+");
    if(fp)
    {
        if(!bLineFeeds)            
            fprintf(fp,"%s%s%s%s",szBegin,temp,szEnd,ach);
        else
            fprintf(fp,"%s%s%s%s\n",szBegin,temp,szEnd,ach);
        fclose(fp);
    }
}
//////////////////////////////////////////////////////
void C_DLCS_Log::AddEntryNoTime(char *fmt, ...)
{
    if(!bActive) return; char ach[512];
    va_list va; va_start( va, fmt );
    vsprintf( ach, fmt, va ); va_end( va );
    FILE *fp=fopen(logfile,"a+");
    if(fp)
    {
        if(!bLineFeeds) fprintf(fp,"%s",ach);
        else            fprintf(fp,"%s\n",ach);
        fclose(fp);
    }
}
//////////////////////////////////////////////////////
void C_DLCS_Log::AddLineSep(void)
{
    if(bActive == false) return;
    FILE *fp=fopen(logfile,"a+");
    if(fp) { fprintf(fp,"%s\n",szLineSep); fclose(fp); }
}
//////////////////////////////////////////////////////
void C_DLCS_Log::Off(void) {  bActive = false; }
//////////////////////////////////////////////////////
void C_DLCS_Log::On(void) { bActive = true; }
//////////////////////////////////////////////////////
bool C_DLCS_Log::IsActive(void) { return bActive; }
//////////////////////////////////////////////////////
bool C_DLCS_Log::Restart(void)
{
    if(!bActive) return false;
    FILE *fp=fopen(logfile,"w+");
    if(fp) { fclose(fp); return true; }
    return false;
}
//////////////////////////////////////////////////////
void C_DLCS_Log::LineFeedsOn(void) { bLineFeeds=true; }
//////////////////////////////////////////////////////
void C_DLCS_Log::LineFeedsOff(void) { bLineFeeds=false; }
