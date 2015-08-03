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

int32 CSession::recv()
{
    int32 writelen = m_recvBuff.getBuffQueuePtr()->getReadableLen();
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
        int32 retHeadLen = m_recvBuff.getHead(header);
        if (retHeadLen != sizeof(header))
        {
            break;
        }
        int32 msgLen = header.length;
        if (msgLen<=0 || msgLen > MAXPKGLEN)
        {
            printf("get error packeage!!! msglen>MAXPKGLEN\n");
            setStatus(waitdel);
            break;
        }
        char buf[msgLen];
        int32 retMsgLen = getMsg(buf, msglen);
        if (retMsgLen != msgLen)
        {
            printf("get error packeage!!! retMsgLen != msgLen\n");
            setStatus(waitdel);
            break;
        }

        
    }
}
