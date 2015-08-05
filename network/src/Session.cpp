#include "../include/Session.h"

CSession::CSession()
{
    m_socket = -1;
    m_boActive = false;
    m_recvBuff.init(SESSIONBUFLEN, 4); //SESSIONBUFLEN);
    m_sendBuff.init(SESSIONBUFLEN, 4); //SESSIONBUFLEN);
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
    int32 writelen = m_recvBuff.getBuffQueuePtr()->getWriteableLen();
    if (writelen <= 0)
    {
        return 0;
    }

    int recvlen = ::recv(m_socket, m_recvBuff.getBuffQueuePtr()->getWritePtr(), writelen, 0);

    if (0 == recvlen)
    {
        printf("socket!!!!!!!!recv return 0!!!!!!!!\n");
        return -1;
    }
    else if (recvlen > 0)
    {
        m_recvBuff.getBuffQueuePtr()->pushMsg(NULL, recvlen);
        return recvlen;
    }
    else
    {
        if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
        {
            printf("socket!!!!!!!!EAGAIN!!!!!!!!\n");
            return 0;
        }
        else
        {
            return -1;
        }
    }
}

int32 CSession::modEpollEvent(int32 epollfd, bool addEvent)
{
    struct epoll_event chkEvent;
    chkEvent.events = EPOLLIN | EPOLLOUT | EPOLLONESHOT;
    chkEvent.data.ptr = this;

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
    struct epoll_event chkEvent;
    chkEvent.events = EPOLLIN | EPOLLOUT | EPOLLONESHOT;
    return epoll_ctl(epollfd, EPOLL_CTL_DEL, m_socket, &chkEvent);
}

int32 CSession::sendToSocket()
{
    int32 canSendlen = m_sendBuff.getBuffQueuePtr()->getReadableLen();
    if (canSendlen <= 0)
    {
        return 0;
    }

    int32 sendLen = ::send(m_socket, m_sendBuff.getBuffQueuePtr()->getReadPtr(canSendlen), canSendlen, 0);

    if (0 == sendLen)
    {
        printf("socket!!!!!!!!send return 0!!!!!!!!\n");
        return -1;
    }
    else if (sendLen > 0)
    {
        m_sendBuff.getBuffQueuePtr()->popMsg(NULL, sendLen);
        return sendLen;
    }
    else
    {
        if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
        {
            printf("socket send!!!!!!!!EAGAIN!!!!!!!!\n");
            return 0;
        }
        else
        {
            return -1;
        }
    }
}

void CSession::processPacket()
{
    PkgHeader header;

    while (m_recvBuff.getBuffQueuePtr()->getBufLen() > 0)
    {
        int32 retHeadLen = m_recvBuff.getHead(&header);
        if (retHeadLen != sizeof(header))
        {
            break;
        }
        int32 msgLen = header.length;
        if (msgLen<=0 || msgLen > MAXPKGLEN)
        {
            //printf("file:%s get error packeage!!! msglen>MAXPKGLEN\n", __FILE__);
            //setStatus(waitdel);
            break;
        }
        char buf[msgLen];
        int32 retMsgLen = m_recvBuff.getMsg(buf, msgLen);
        if (retMsgLen != msgLen)
        {
            //printf("get error packeage!!! retMsgLen:%d != msgLen:%d\n", retMsgLen, msgLen);
            break;
        }
        handlePackage(this, &header, buf, msgLen);
        
    }
}

void CSession::defaultMsgHandle(int16 sysid, int16 msgtype, char *msgbuf, int32 msgsize) // first package to register
{
    struct c_s_registersession *msg = (struct c_s_registersession*)(msgbuf + sizeof(MsgHeader));
    int16 sessionType = msg->sessionType;
    NetWorkObject *netobj = NULL;
    struct s_c_registersession ret;
    MsgHeader msghead;
    int32 headlen = 0;
    PkgHeader header;
    int32 totalsize = 0;
    //char *buf = NULL;
    switch (sessionType)
    {
    case 1: // client
        {
            netobj = new ClientSession;
            bindNetWorkObj(netobj);
            ret.retcode = 0;
            msghead.sysId = 1;
            msghead.msgType = 1;
            headlen = sizeof(msghead) + sizeof(ret);
            header.length = headlen;
            header.reserved = 0;
            totalsize = headlen + sizeof(header);
            char buf[totalsize];
            encodepkg(buf, &header, &msghead, (char *)&ret, (int32)sizeof(ret));
            //send(buf, totalsize);
            cout << "sessionType:client send len:" << endl;
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
            netobj = new ClientSession;
            bindNetWorkObj(netobj);
            ret.retcode = 0;
            msghead.sysId = 1;
            msghead.msgType = 1;
            headlen = sizeof(msghead) + sizeof(ret);
            header.length = headlen;
            header.reserved = 0;
            totalsize = headlen + sizeof(header);
            char buf[totalsize];
            encodepkg(buf, &header, &msghead, (char *)&ret, (int32)sizeof(ret));
            //send(buf, totalsize);
            cout << "strictclient got msg" << endl;
            break;
        }
    default:
        break;
    }
}
