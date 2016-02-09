#ifndef __MAIN_H_
#define __MAIN_H_

#include <cstring>

enum CLI_MODE{
    CLIENT = 0,
    SERVER,
    REDIR
};

enum PROXY_MODE{
    SOCKS,
    HTTP,
    GLOBAL_HTTP
};

#endif
