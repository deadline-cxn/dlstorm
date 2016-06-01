/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2013
 **   File:         c_hash.cpp
 **   Class:        C_Hash
 **   Description:  Function Hash system (in development)
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#include "c_hash.h"
namespace {
    void DoOne() {
        cout << "One" << endl;
    }
    void DoTwo() {
        cout << "Two" << endl;
    }
    void DoNil() {
        cout << "Nil" << endl;
    }
}
void Hash_Mapper() {
    typedef void (*DoIt)();
    typedef map<string,DoIt> MapCall;
    MapCall	doit;
    doit.insert(MapCall::value_type("One",DoOne));
    doit.insert(MapCall::value_type("Two",DoTwo));
    doit.insert(MapCall::value_type("Nil",DoOne));
    MapCall::const_iterator call;
    call = doit.find("One");
    if (call != doit.end())
        (*call).second();
    else
        cout << "Unknown call requested" << endl;
}
//////////////////////////////////// C_Hash class
C_Hash::C_Hash() { }
C_Hash::~C_Hash() { }
