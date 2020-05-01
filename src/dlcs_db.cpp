#include "dlcs_db.h"

// - create new database class that abstracts db layer (will be able to use different methods such as flatfile, mysql,sqlite)

C_DLCS_DB::C_DLCS_DB() {}

C_DLCS_DB::~C_DLCS_DB() {}

void C_DLCS_DB::Init(void) {
    iType = 0;
    ZeroMemory(&SMysqlConfig, sizeof(stMysqlConfig));
};