/***************************************************************
 Seth's (Log Class)
 ***************************************************************/

#include "c_log.h"

//////////////////////////////////////////////////////
CLog::CLog() { memset(logfile,0,_MAX_PATH); strcpy(logfile,"log.log"); Initialize();}
//////////////////////////////////////////////////////
CLog::CLog(char *szFilename) { Initialize(); SetName(szFilename); LineFeedsOn();}
//////////////////////////////////////////////////////
CLog::~CLog() { }
//////////////////////////////////////////////////////
void CLog::Initialize(void) {
    //if( getcwd( logfile, _MAX_PATH ) != NULL )
    //    strcpy(logdir,getcwd( "", _MAX_PATH ));
    strcpy(logfile,"[CLog].log");
    strcpy(szBegin,"(");
    strcpy(szEnd,"):");
    strcpy(szLineSep,"*****************************************************************");
    bActive = true;
    bLineFeeds = false;
}
//////////////////////////////////////////////////////
void CLog::SetName(char *szFilename) {
	strcpy(logfile,szFilename);
    //if( getcwd( logfile, _MAX_PATH ) != NULL) strcpy(logdir,getcwd("",_MAX_PATH));
    //strcpy(logfile,szFilename);
}
//////////////////////////////////////////////////////
void CLog::AddEntry(char *fmt, ...) {
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
    if(fp) {
        if(!bLineFeeds)
            fprintf(fp,"%s%s%s%s",szBegin,temp,szEnd,ach);
        else
            fprintf(fp,"%s%s%s%s\n",szBegin,temp,szEnd,ach);
        fclose(fp);
    }

}
//////////////////////////////////////////////////////
void CLog::AddEntryNoTime(char *fmt, ...) {
    if(!bActive) return; char ach[512];
    va_list va; va_start( va, fmt );
    vsprintf( ach, fmt, va ); va_end( va );
    FILE *fp=fopen(logfile,"a+");
    if(fp) {
        if(!bLineFeeds) fprintf(fp,"%s",ach);
        else            fprintf(fp,"%s\n",ach);
        fclose(fp);
    }

}
//////////////////////////////////////////////////////
void CLog::AddLineSep(void) {
    if(bActive == false) return;
    FILE *fp=fopen(logfile,"a+");
    if(fp) { fprintf(fp,"%s\n",szLineSep); fclose(fp); }
}
//////////////////////////////////////////////////////
void CLog::Off(void) {  bActive = false; }
//////////////////////////////////////////////////////
void CLog::On(void) { bActive = true; }
//////////////////////////////////////////////////////
bool CLog::IsActive(void) { return bActive; }
//////////////////////////////////////////////////////
bool CLog::Restart(void) {
    if(!bActive) return false;
    FILE *fp=fopen(logfile,"w+");
    if(fp) { fclose(fp); return true; }
    return false;
}
//////////////////////////////////////////////////////
void CLog::LineFeedsOn(void) { bLineFeeds=true; }
//////////////////////////////////////////////////////
void CLog::LineFeedsOff(void) { bLineFeeds=false; }
