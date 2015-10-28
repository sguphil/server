#include "../include/Acceptor.h"

Acceptor::Acceptor(SESSION_TYPE type): m_eAcceptType(type)
{
    memset(m_listenIp, 0x00, sizeof(m_listenIp));
    m_boIsListen = false;
    m_nServerSock = -1;
    m_epollfd = epoll_create(3000);
    m_SessionFactory.init(1, 5);
    m_acceptListLock.lock();
    m_pReadList = &m_acceptList;
    m_pWriteList = &m_acceptListSec;
    m_acceptListLock.unLock();
    cout << " Acceptor constructer finished!!!" << endl;
}

Acceptor::~Acceptor()
{

    //dtor
}

void Acceptor::init(int maxAcc)
{
    m_nCurrAccept = 0;
    m_nMaxAccept = 3000;
    m_nMaxAcc = maxAcc;
}

bool Acceptor::startListen(const char *szIP, Int32 nPort)
{
    strcpy(m_listenIp, szIP);
    m_listenPort = nPort;
    if(!createSocket())
    {
        return false;
    }

    struct epoll_event epEvent;
    epEvent.events = EPOLLIN;
    epEvent.data.fd = m_nServerSock;

    epoll_ctl(m_epollfd, EPOLL_CTL_ADD, m_nServerSock, &epEvent);
    return true;
}

void Acceptor::setSocketNoBlock(int32 socket)
{
    //设置非堵塞
    Int32 ioFlag;
    ioFlag = fcntl(socket, F_GETFL);
    fcntl(socket, F_SETFL, ioFlag|O_NONBLOCK);
}

bool Acceptor::createSocket()
{
    m_nServerSock = socket(AF_INET, SOCK_STREAM, 0);

    //设置地址重用
    Int32 reuse = 1;
    setsockopt(m_nServerSock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    //设置非堵塞
    Int32 ioFlag;
    ioFlag = fcntl(m_nServerSock, F_GETFL);
    fcntl(m_nServerSock, F_SETFL, ioFlag|O_NONBLOCK);
    //bind
    struct sockaddr_in svrAddr;
    svrAddr.sin_family = AF_INET;
    svrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    svrAddr.sin_port = htons(m_listenPort);

    if (bind(m_nServerSock, (struct sockaddr *)&svrAddr, sizeof(sockaddr_in)) < 0)
    {
        return false;
    }
    //listen
    if (listen(m_nServerSock, 10)< 0)
    {
        return false;
    }
    return true;
}

bool Acceptor::getAcceptList(std::list<CSession *>& retList)
{
    retList.splice(retList.begin(), *m_pReadList);
    AutoLock autoLock(&m_acceptListLock);
    CommonList<CSession> *pSwap = m_pReadList;
    m_pReadList = m_pWriteList;
    m_pWriteList = pSwap;
    return true;
}

void* Acceptor::threadRoutine(void *args)
{
    //Acceptor *acceptor = (Acceptor*)args;
    struct epoll_event epEvent;
    cout << "Acceptor::threadRoutine start" << endl;
    while(true)
    {
        //cout << "Acceptor::threadRoutine(void *args)2" << endl;
        Int32 evCount = epoll_wait(m_epollfd, &epEvent, 1, -1);
        if (evCount > 0)
        {
            //struct sockaddr_id cliAddr;
            Int32 clientSock = accept(m_nServerSock, NULL, NULL);
            if (clientSock > 0)
            {
                CSession *session = m_SessionFactory.allocate();
                session->setType(m_eAcceptType);
               //setSocketNoBlock(clientSock);

                if (NULL != session)
                {
                    session->setSocket(clientSock);
                    session->setSvrType(this->m_svrType);
                    addSession2List(session);
                    printf("alloc session\n");
                }
                else
                {
                    printf("can not alloc session\n");
                }
            }
            else
            {
                usleep(10000);
                printf("accept error\n");
            }
        }
    }
    return NULL;
}
