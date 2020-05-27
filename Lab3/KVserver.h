#ifndef KVSERVER_H
#define KVSERVER_H

#include <iostream>
#include <cstring>
#include <map>
#include <vector>
using namespace std;

struct MSG{
    bool state;
    string message;
};
const MSG errorMSG={.state=false,.message="-ERROR\r\n"};
struct Socket{
    string IP;
    string port;
};
vector<string> mysplit(string str,string pattern);

#endif