#include "../include/Session.h"
#include "../../Factory/BaseFactory.h"
#include "../../session/ClientSession.h"
#include "../../session/AccsvrSession.h"
#include "../../session/DBSession.h"
#include "../../session/LogicSession.h"
#include "../../session/StrictClient.h"

#define REUSE_NETWORKOBJ 1
extern CBaseFactory<ClientSession> g_ClientNetWorkObjectFactory;

CSession::CSession()
{
    m_socket = -1;
    m_boActive = false;
    m_recvBuff.init(SESSIONBUFLEN, SESSIONBUFLEN);
    m_sendBuff.init(SESSIONBUFLEN, SESSIONBUFLEN);
    m_pBindNetWorkObj = NULL;
    m_nSessionId = 0;
    m_LeftPkgBuf = new char[MAXPKGLEN];
    assert(m_LeftPkgBuf != NULL);
    //m_LeftPkgBuf = new char[MAXPKGLEN];
    /*
    m_RecvTwoQueue.init(SESSIONBUFLEN);
    m_SendTwoQueue.init(SESSIONBUFLEN); 
    */ 
}

CSession::~CSession()
{
    delete[] m_LeftPkgBuf;
}
int32 CSession::send(char *buff, int32 buffsize)
{
    return m_sendBuff.putMsg(buff, buffsize);
}

int32 CSession::recv()
{
    /*
    int32 writelen = m_recvBuff.getBuffQueuePtr()->getWriteableLen();
    if (writelen <= 0)
    {
        return 0;
    }
    */
    #ifdef USE_DOUBLE_QUEUE
    m_recvBuff.lockSwap();
    int recvlen = m_recvBuff.getWRQueuePtr()->recvFromSocket(m_socket); //::recv(m_socket, m_recvBuff.getBuffQueuePtr()->getWritePtr(), writelen, 0);
    if (0 == recvlen)
    {
        cout << "===============test===========recv readbuff len:" << m_recvBuff.getRDQueuePtr()->getBufLen() << endl;
    }
    m_recvBuff.unLockSwap();
    return recvlen;
    #else
    int recvlen = m_recvBuff.getBuffQueuePtr()->recvFromSocket(m_socket); //::recv(m_socket, m_recvBuff.getBuffQueuePtr()->getWritePtr(), writelen, 0);
    return recvlen;
    #endif
}

int32 CSession::modEpollEvent(int32 epollfd, bool isRecv, bool addEvent)
{
    struct epoll_event chkEvent;
    chkEvent.data.ptr = this;
    
    if (isRecv)
    {
        chkEvent.events = EPOLLIN | EPOLLONESHOT;
    }
    else
    {
        chkEvent.events = EPOLLOUT | EPOLLONESHOT;
    }

    if (addEvent)
    {
        return epoll_ctl(epollfd, EPOLL_CTL_ADD, m_socket, &chkEvent);
    }
    else
    {
        return epoll_ctl(epollfd, EPOLL_CTL_MOD, m_socket, &chkEvent);
    }
}

int32 CSession::delEpollEvent(int32 epollfd)
{
    return epoll_ctl(epollfd, EPOLL_CTL_DEL, m_socket, NULL);
}

int32 CSession::sendToSocket()
{
    #ifdef USE_DOUBLE_QUEUE
    int32 sendLen = m_sendBuff.getRDQueuePtr()->sendToSocket(m_socket);
    if (m_sendBuff.getRDQueuePtr()->getReadableLen() == 0)
    {
        m_sendBuff.swapQueue();
    }
    #else
    int32 sendLen = m_sendBuff.getBuffQueuePtr()->sendToSocket(m_socket);
    #endif

    return sendLen;
}

void CSession::processPacket()
{
    #ifdef USE_DOUBLE_QUEUE
    bool isbreak = false;
    int handlePkgCount = 0;
    int32 leftPkgRet = m_recvBuff.checkLeftQueue(m_LeftPkgBuf, &m_header);
    if(leftPkgRet >= 0)
    {
        if (leftPkgRet > 0)
        {
            handlePackage(this, &m_header, (MsgHeader *)(m_LeftPkgBuf + sizeof(m_header)), (m_LeftPkgBuf + sizeof(m_header) + sizeof(MsgHeader)), m_header.length - sizeof(MsgHeader));
            handlePkgCount++;
            assert(m_recvBuff.getTempQueLen() == 0);

            if (m_recvBuff.getTempQueLen() == 0)
            {
                while (m_recvBuff.getRDQueuePtr()->fetchFullPkg(m_pkgGet) > 0)
                {
                    assert(m_recvBuff.getTempQueLen() == 0);
                    handlePackage(this, &m_pkgGet.m_pkgHeader, &m_pkgGet.m_msgHeader, m_pkgGet.m_msgbuf, m_pkgGet.m_nMsglen);
                }
                if (!isbreak)
                {
                    m_recvBuff.swapQueue();
                }
            }
        }
        else if (m_recvBuff.getRDQueuePtr()->getBufLen() == 0)
        {
            m_recvBuff.swapQueue();
        }
        else
        {
            while (m_recvBuff.getRDQueuePtr()->fetchFullPkg(m_pkgGet) > 0)
            {
                assert(m_recvBuff.getTempQueLen() == 0);
                handlePackage(this, &m_pkgGet.m_pkgHeader, &m_pkgGet.m_msgHeader, m_pkgGet.m_msgbuf, m_pkgGet.m_nMsglen);
            }
            if (!isbreak)
            {
                m_recvBuff.swapQueue();
            }
        }
    }
    else
    {
        setStatus(waitdel);
        assert(false);
    }
    #else
    int handlePkgCount = 0;
    while (m_recvBuff.getBuffQueuePtr()->fetchFullPkg(m_pkgGet) > 0)
    {
        handlePackage(this, &m_pkgGet.m_pkgHeader, &m_pkgGet.m_msgHeader, m_pkgGet.m_msgbuf, m_pkgGet.m_nMsglen);
        if (handlePkgCount++ > 30) // handle 30 packages each loop
        {
            break;
        }
    }
    #endif
}
void CSession::defaultMsgHandle(MsgHeader *msgHead, char *msgbuf, int32 msgsize) // first package to register
{
    struct c_s_registersession *msg = (struct c_s_registersession*)(msgbuf);
    int16 sessionType = msg->sessionType;
    NetWorkObject *netobj = NULL;
    struct c_s_registersession ret;
    MsgHeader msghead = *msgHead;
    int32 msglen = 0;
    PkgHeader header;
    int32 totalsize = 0;
    //char *buf = NULL;
    switch (sessionType)
    {
    case 1: // client
        {
            netobj = g_ClientNetWorkObjectFactory.allocate();
            //netobj = new ClientSession;
            assert(netobj != NULL);
            bindNetWorkObj(netobj);
            ret.sessionType = 6; // modify the typeof client
            msglen = sizeof(msghead) + sizeof(ret);

            header.length = msglen;
            header.reserved = 0;

            totalsize = msglen + sizeof(header);
            char buf[totalsize];
            encodepkg(buf, &header, &msghead, (char *)&ret, (int32)sizeof(ret));
            send(buf, totalsize);// send back the same struct
            setType((SESSION_TYPE)1);
            //cout << "sessionType:client send reg sessiontype:" << ret.sessionType << endl;
            break;
        }
    case 2: // gateway
        setType((SESSION_TYPE)2);
        break;
    case 3: // other account svr
        setType((SESSION_TYPE)3);
        break;
    case 4: // gameserver/logicServer
        setType((SESSION_TYPE)4);
        break;
    case 5: // dbserver
        netobj = new DBSession;
        assert(NULL != netobj);
        bindNetWorkObj(netobj);
        setType((SESSION_TYPE)5);
        cout << "strictclient got msg" << endl;
        break;
    case 6: // strict client for test
        {
            netobj = new StrictClient;
            assert(NULL != netobj);
            bindNetWorkObj(netobj);
            cout << "got strictclient msg" << endl;
            setType((SESSION_TYPE)6);
            break;
        }

    case 7: //Account server
        netobj = new DBSession;
        assert(NULL != netobj);
        bindNetWorkObj(netobj);
        setType((SESSION_TYPE)7);
        cout << "got accsvr msg" << endl;
        break;
    default:
        break;
    }
}

void CSession::defaultMsgHandle(int16 sysid, int16 msgtype, char *msgbuf, int32 msgsize) // first package to register
{
    struct c_s_registersession *msg = (struct c_s_registersession*)(msgbuf + sizeof(MsgHeader));
    int16 sessionType = msg->sessionType;
    NetWorkObject *netobj = NULL;
    struct c_s_registersession ret;
    MsgHeader msghead;
    int32 msglen = 0;
    PkgHeader header;
    int32 totalsize = 0;
    //char *buf = NULL;
    switch (sessionType)
    {
    case 1: // client
        {
            netobj = new ClientSession;
            bindNetWorkObj(netobj);
            msghead.sysId = 1;
            msghead.msgType = 1;

            ret.sessionType = 6; // modify the typeof client
            msglen = sizeof(msghead) + sizeof(ret);

            header.length = msglen;
            header.reserved = 0;

            totalsize = msglen + sizeof(header);
            char buf[totalsize];
            encodepkg(buf, &header, &msghead, (char *)&ret, (int32)sizeof(ret));
            send(buf, totalsize);// send back the same struct

            cout << "sessionType:client send reg sessiontype:" << ret.sessionType << endl;
            break;
        }
    case 2: // gateway
        break;
    case 3: // other account svr
        break;
    case 4: // gameserver
        break;
    case 5: // dbserver
        break;
    case 6: // strict client for test
        {
            netobj = new StrictClient;
            bindNetWorkObj(netobj);
            cout << "strictclient got msg" << endl;
            break;
        }
    default:
        break;
    }
}
