#ifndef __SESSION_H__
#define __SESSION_H__
#include "../../include/baseHeader.h"
#include "NetWorkObject.h"
#include "../../include/CServerBase.hpp"
#include "../../include/CRecvBuf.hpp"
#include "../../include/CSendBuf.hpp"
#include "../../include/packHeader.hpp"
#include "../../include/packageStruct.hpp"
#include "../../session/ClientSession.h"

//class NetWorkObject;

enum eSESSIONSTATUS
{
    active = 1, // active io
    waitactive = 2, // need to active io 
    waitdel = 3, // deactive, need to remove
    registered = 4,//registered
};

class CSession
{
public:
    CSession();
    ~CSession();
    inline void clear() // call when reuse
    {
        m_recvBuff.getBuffQueuePtr()->clear();
        m_sendBuff.getBuffQueuePtr()->clear();
        delete m_pBindNetWorkObj;
        m_pBindNetWorkObj = NULL;
    }

    inline void setSocket(Int32 socket)
    {
        m_socket = socket;
        m_boActive = true;
    }

    inline Int32 getSocket(void)
    {
        return m_socket;
    }

    inline void setSockAddr(struct sockaddr_in addr)
    {
        m_sockAddr = addr;
    }

    inline struct sockaddr_in& getSockAddr(void)
    {
        return m_sockAddr;
    }

    inline void setPort(Int32 nPort)
    {
        m_nPort = nPort;
    }

    inline void setIp(const char* szIp)
    {
        memset(m_szIp, 0x00, sizeof(m_szIp));
        strcpy(m_szIp, szIp);
    }

    inline char* getIp(void)
    {
        return m_szIp;
    }

    inline void setType(SESSION_TYPE type)
    {
        m_eSessionType = type;
    }

    inline void setServer(CServerBase *svr)
    {
        m_ptrServer = svr;
    }

    inline CServerBase* getServer()
    {
        return m_ptrServer;
    }

    int32 send(void *buff, int buffsize);  // logic module call to write msg to buffqueue

    int32 sendToSocket(); //network layer call to send msg with socket
    int32 recv();  // network layer call to recv msg with socket
    void processPacket();  // application layer handle msg from buffqueue, into the logic modules
    int32 modEpollEvent(int32 epollfd, bool addEvent=false);
    int32 delEpollEvent(int32 epollfd);

    inline void bindNetWorkObj(NetWorkObject *networkObj)
    {
        m_pBindNetWorkObj = networkObj;
        m_pBindNetWorkObj->setSesion(this);
    }

    inline NetWorkObject* getNetWorkObject()
    {
        return m_pBindNetWorkObj;
    }

    inline void setStatus(eSESSIONSTATUS status)
    {
        m_eStatus = status;
    }

    inline eSESSIONSTATUS getStatus()
    {
        return m_eStatus;
    }

    void defaultMsgHandle(int16 sysid, int16 msgtype, char *msgbuf, int32 msgsize) // first package to register
    {
       struct c_s_registersession *msg = (struct c_s_registersession*)(msgbuf + sizeof(MsgHeader));
       int16 sessionType = msg->sessionType;
       switch (sessionType)
       {
       case 1: // client
           NetWorkObject *netobj = new ClientSession();
           bindNetWorkObj(netobj);
           struct s_c_registersession ret = { 0 };
           MsgHeader msghead = {1, 1};
           int32 headlen = sizeof(msghead) + sizeof(ret);
           PkgHeader header = { headlen, 0 };
           int32 totalsize = headlen + sizeof(header);
           char buf[totalsize];
           encodepkg(buf, header, msghead, (char *)ret, sizeof(ret));
           send(buf, totalsize);
           break;
       case 2: // gateway
           break;
       case 3: // other account svr
           break;
       case 4: // gameserver
           break;
       case 5: // dbserver
           break;
       default:
           break;
       }
    }

private:
    Int32 m_socket;
    char m_szIp[32];
    Int32 m_nPort;
    bool m_boActive;
    SESSION_TYPE m_eSessionType;
    struct sockaddr_in m_sockAddr;
    NetWorkObject *m_pBindNetWorkObj;
    CRecvBuf m_recvBuff;
    CSendBuf m_sendBuff;
    CServerBase *m_ptrServer;
    eSESSIONSTATUS m_eStatus;
};
#endif // __SESSION_H__
