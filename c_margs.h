#include "dlstorm.h"
class C_Muh_Args {
public:
    C_Muh_Args(int argc_, char * argv_[],bool switches_on_=false);
    ~C_Muh_Args(){}
    string get_arg(int i);
    string get_arg(string s);
private:
    int argc;
    vector<string> argv;
    bool switches_on;
    map<string,string> switch_map;
};
