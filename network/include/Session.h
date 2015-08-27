#ifndef __SESSION_H__
#define __SESSION_H__
#include "../../include/baseHeader.h"
//#include "NetWorkObject.h"
#include "../../include/CServerBase.hpp"
#include "../../include/CRecvBuf.hpp"
#include "../../include/CSendBuf.hpp"
#include "../../include/CIoBuff.hpp"
#include "../../include/packHeader.hpp"
#include "../../include/packageStruct.hpp"
#include "../../session/ClientSession.h"
#include "../../session/StrictClient.h"
#include "../../include/PackageHandler.hpp"
#include "../../include/CPackageFetch.hpp"
//#include "../../include/queue.hpp"

extern int32 MAXPKGLEN;
extern int32 SESSIONBUFLEN;

#define USE_DOUBLE_QUEUE 1

class NetWorkObject;
//class ClientSession;


class CSession
{
public:
    CSession();
    ~CSession();
    inline void clear() // call when reuse
    {
        #ifdef USE_DOUBLE_QUEUE
        m_recvBuff.clear();
        m_sendBuff.clear();
        #else
        m_recvBuff.getBuffQueuePtr()->clear();
        m_sendBuff.getBuffQueuePtr()->clear();
        #endif

        #if REUSE_NETWORKOBJ
        g_ClientNetWorkObjectFactory.reuse(m_pBindNetWorkObj);
        #else
        delete m_pBindNetWorkObj;
        close(m_socket); // close socket fd
        if (NULL != m_pBindNetWorkObj)
        {
            m_pBindNetWorkObj = NULL;
        }
        #endif
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

    inline SESSION_TYPE getType()
    {
        return m_eSessionType;
    }

    inline void setServer(CServerBase *svr)
    {
        m_ptrServer = svr;
    }

    inline CServerBase* getServer()
    {
        return m_ptrServer;
    }

    int32 send(char *buff, int32 buffsize);  // logic module call to write msg to buffqueue return:-1 error -2 again >=0 success send length

    int32 sendToSocket(); //network layer call to send msg with socket
    int32 recv();  // network layer call to recv msg with socket
    void processPacket();  // application layer handle msg from buffqueue, into the logic modules
    int32 modEpollEvent(int32 epollfd, bool isRecv, bool addEvent=false);
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

    inline uint32 getSessionId()
    {
        return m_nSessionId;
    }

    inline void setm_nSessionId(uint32 sessionId)
    {
        m_nSessionId = sessionId;
    }

    void defaultMsgHandle(int16 sysid, int16 msgtype, char *msgbuf, int32 msgsize); // first package to register
    void defaultMsgHandle(MsgHeader *msgHead, char *msgbuf, int32 msgsize);
/*
    inline CSendBuf* getSendbufPtr()
    {
        return &m_sendBuff;
    }

    inline CRecvBuf* getRecvbufPtr()
    {
        return &m_recvBuff;
    }
*/
    #ifdef USE_DOUBLE_QUEUE
    inline CIoBuff* getRecvBuffPtr()
    {
        return &m_recvBuff;
    }

    inline CIoBuff* getSendBuffPtr()
    {
        return &m_sendBuff;
    } 
    #endif

    inline void setRecvNSendBuffSwapTick(int32 recvSwapTick, int32 sendSwapTick)
    {
        #ifdef USE_DOUBLE_QUEUE
        m_recvBuff.setBuffSwapTick(recvSwapTick);
        m_sendBuff.setBuffSwapTick(sendSwapTick);
        #endif
    }
private:
    Int32 m_socket;
    char m_szIp[32];
    Int32 m_nPort;
    bool m_boActive;
    SESSION_TYPE m_eSessionType;
    struct sockaddr_in m_sockAddr;
    NetWorkObject *m_pBindNetWorkObj;
    #ifdef USE_DOUBLE_QUEUE
    CIoBuff m_recvBuff;
    CIoBuff m_sendBuff;
    #else
    CRecvBuf m_recvBuff;
    CSendBuf m_sendBuff;
    #endif
    /*
    clwCore::CTwoQueues m_RecvTwoQueue;
    clwCore::CTwoQueues m_SendTwoQueue;
    */
    CServerBase *m_ptrServer;
    eSESSIONSTATUS m_eStatus;
    uint32 m_nSessionId;
    char *m_LeftPkgBuf;
    CpackageFetch m_pkgGet;
    PkgHeader m_header;
};
#endif // __SESSION_H__
