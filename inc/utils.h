#ifndef _UTILS_H_
#define _UTILS_H_

#include <errno.h>
#include <sstream>
using namespace std;


#ifndef DEBUG
#define D(s)
#else
#include <iostream>
#define D(s) cout << s;
#endif

#endif

