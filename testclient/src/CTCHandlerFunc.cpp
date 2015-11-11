#include "../include/CTCHandlerFunc.hpp"
#include "../include/MessageDef.hpp"

int32 CTCHandlerFunc::testfunc(CSession *session, char *pMsg, int32 msglen)
{
    printf("%s\n",  __FUNCTION__ );
    return 0;
}

int32 CTCHandlerFunc::checkuser(CSession *session, char *pMsg, int32 msglen)
{
    printf("%s\n",  __FUNCTION__ );
    return 0;
}

int32 CTCHandlerFunc::dbcheckuserret(CSession *session, char *pMsg, int32 msglen)
{
    printf("%s\n",  __FUNCTION__ );
    return 0;
}
