/***************************************************************
    DLSTORM Deadline's Code Storm Library
    Author: Seth Parson

****************************************************************/
#include "c_hash.h"

#include <iostream>
#include <string>
#include <map>

using namespace std;

namespace { // for example only
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

void Mapper() {
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

C_Hash::C_Hash() {
    //ctor
}

C_Hash::~C_Hash() {
    //dtor
}
