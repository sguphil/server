#include "AccountSvr.h"

AccountSvr::AccountSvr()
{
    m_ServerID = 1;
    //ctor
}

AccountSvr::~AccountSvr()
{
    //dtor
}

void AccountSvr::start()
{
    m_acceptor.init();
    m_acceptor.startListen("127.0.0.1", 9997);
    m_acceptor.start();
}
