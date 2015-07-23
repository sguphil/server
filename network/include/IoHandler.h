#ifndef __IOHANDLER_H__
#define __IOHANDLER_H__
#include "Acceptor.h"
#include "Connector.h"
#include "../../include/baseHeader.h"

class IoHandler
{
public:
    IoHandler();
    ~IoHandler();
    void init();
    void run();
    void update();
    void processActiveSessionList();
    void processAcceptList();
    void processErrorList();

private:
    Acceptor m_acceptor;
    Connector m_connector;
    std::list<CSession*> m_ActiveSessionList;
    std::list<CSession*> m_ErrorSessionList;
    Int32 m_nUpdateTime;
    std::list<CSession*> m_tempSessionList;
};
#endif // __IOHANDLER_H__
