#ifndef ACCOUNTSVR_H
#define ACCOUNTSVR_H
#include "Singleton.h"

class AccountSvr: public base::Singleton<AccountSvr>
{
    public:
        AccountSvr();
        virtual ~AccountSvr();
        inline int getServerID() {return m_ServerID;}
    protected:
    private:
        int m_ServerID;
};

#endif // ACCOUNTSVR_H
