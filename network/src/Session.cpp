#include "../include/Session.h"

CSession::CSession()
{
    m_socket = -1;
    m_boActive = false;
    m_recvBuff.init(SESSIONBUFLEN, SESSIONBUFLEN);
    m_sendBuff.init(SESSIONBUFLEN, SESSIONBUFLEN);
}

CSession::~CSession()
{

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
    int recvlen = m_recvBuff.getBuffQueuePtr()->recvFromSocket(m_socket); //::recv(m_socket, m_recvBuff.getBuffQueuePtr()->getWritePtr(), writelen, 0);
    return recvlen;
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
    int32 sendLen = m_sendBuff.getBuffQueuePtr()->sendToSocket(m_socket);
    return sendLen;
}

void CSession::processPacket()
{
    int handlePkgCount = 0;
    CpackageFetch pkgGet;
    while (m_recvBuff.getBuffQueuePtr()->fetchFullPkg(pkgGet) > 0)
    {
        handlePackage(this, &pkgGet.m_pkgHeader, &pkgGet.m_msgHeader, pkgGet.m_msgbuf, pkgGet.m_nMsglen);
        if (handlePkgCount++ > 30) // handle 30 packages each loop
        {
            break;
        }
    }
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
            netobj = new ClientSession;
            bindNetWorkObj(netobj);
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
