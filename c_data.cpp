/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2017
 **   File:         c_data.h
 **   Class:        CC_Data
 **                 C_Profile
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_data.h"

//////////////////// CC_Data class
CC_Data::CC_Data() {
    Init();
    Initialize();
    SetToDefaults();
}
CC_Data::CC_Data(char* infilename) {
    Init();
    Initialize();
    SetToDefaults();
    filename.assign(infilename);
}
CC_Data::~CC_Data() {
    CleanUp();
}
void CC_Data::Initialize(void) {
    SetToDefaults();
}
void CC_Data::CleanUp(void) {

}
void CC_Data::SetToDefaults(void) {

}
