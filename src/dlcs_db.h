#ifndef _DLCS_DB
#define _DLCS_DB

#define _DLCS_DB_FLATFILE 1
#define _DLCS_DB_MYSQL    2
#define _DLCS_DB_SQLITE   3

#include "dlcs.h"

struct stMysqlConfig {
    char szMysqlHost[_HOST_NAME_MAX];
    char szMysqlPort[6];
    char szMysqlUser[_TEXTNAME_SIZE];
    char szMysqlPassword[_TEXTNAME_SIZE];
    char szMysqlDB[_TEXTNAME_SIZE];
};

class C_DLCS_DB {
   public:
    C_DLCS_DB();
    ~C_DLCS_DB();

    unsigned int  iType;
    stMysqlConfig SMysqlConfig;

    void Init(void);
};

#endif