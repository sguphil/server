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

#include "../../common/CPkgBuf.hpp"
#include "../../common/CPkgBufFactory.hpp"
#include "../../common/CPkgBufManager.hpp"

extern int32 MAXPKGLEN;
extern int32 SESSIONBUFLEN;

#define USE_DOUBLE_QUEUE 1
//#define REUSE_NETWORKOBJ 1
class NetWorkObject;
class EpollServer;


class CSession
{
public:
    CSession();
    ~CSession();
    inline void clear() // call when reuse
    {
#if 0 
        #ifdef USE_DOUBLE_QUEUE
        m_recvBuff.clear();
        m_sendBuff.clear();
        #else
        m_recvBuff.getBuffQueuePtr()->clear();
        m_sendBuff.getBuffQueuePtr()->clear();
        #endif
#endif
        #if REUSE_NETWORKOBJ
        g_ClientNetWorkObjectFactory.reuse(m_pBindNetWorkObj);
        #else        
        close(m_socket); // close socket fd
        if (NULL != m_pBindNetWorkObj)
        {
            delete m_pBindNetWorkObj;
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

    inline int32 getPort()
    {
        return m_nPort;
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

    inline void setServer(EpollServer *svr)
    {
        m_ptrServer = svr;
    }

    inline EpollServer* getServer()
    {
        return m_ptrServer;
    }

    int32 processSend(uint16 sysid, uint16 msgid, char *msg, int32 msgsize)
    {
        #if 0
        MsgHeader msgHead;// = {sysid, msgid};
        msgHead.sysId = sysid;
        msgHead.msgType = msgid;

        PkgHeader head;// = {msglen, 0};
        uint16 msglen = sizeof(msgHead) + msgsize;
        uint16 pkglen = msglen + sizeof(head);
        head.length = msglen;

        char buf[pkglen];
        //encodepkg(buf, &head, &msgHead, msg, msgsize);
        int32 headsize = sizeof(head) + sizeof(msgHead);
        memcpy(buf, (char *)&head, sizeof(head));
        memcpy(buf + sizeof(head), (char*)&msgHead, sizeof(msgHead));
        memcpy(buf + headsize, msg, msgsize);
        return send(buf, (int32)pkglen);
        #endif
        ICPkgBuf *sendpkg =  m_SendBufManager.getCurPkg();
        assert(sendpkg != NULL);
        char *bufbegin = sendpkg->getPkgWritePos();
        PkgHeader *pPkgHead = (PkgHeader *)bufbegin;
        MsgHeader *pMsghead = (MsgHeader *)(bufbegin + sizeof(PkgHeader));
        char *msgbuf = bufbegin + sizeof(PkgHeader) + sizeof(MsgHeader);
        
        pPkgHead->length = sizeof(PkgHeader) + sizeof(MsgHeader) + msgsize;
        pMsghead->sysId = sysid;
        pMsghead->msgType = msgid;
        memcpy(msgbuf, msg, msgsize);
        m_SendBufManager.pushPkgToList(pPkgHead->length);
        sendToSocket(); //send directly
        return  pPkgHead->length;
    }

    /**
     * for protobuf Serialize to packagebuf
     * 
     * @author root (11/1/2015)
     * 
     * @param T 
     * @param sysid 
     * @param msgid 
     * @param sendmsg 
     * 
     * @return int32 
     */
    template<typename T>
    int32 processSend(uint16 sysid, uint16 msgid, T& sendmsg)
    {
        int32 sendlen = sendmsg.ByteSize();
        ICPkgBuf *sendpkg =  m_SendBufManager.getCurPkg(sendlen);
        assert(sendpkg != NULL);
        char *bufbegin = sendpkg->getPkgWritePos();
        PkgHeader *pPkgHead = (PkgHeader *)bufbegin;
        MsgHeader *pMsghead = (MsgHeader *)(bufbegin + sizeof(PkgHeader));
        char *msgbuf = bufbegin + sizeof(PkgHeader) + sizeof(MsgHeader);
        sendmsg.SerializeToArray(msgbuf, sendlen);

        pPkgHead->length = sizeof(PkgHeader) + sizeof(MsgHeader) + sendlen;
        pMsghead->sysId = sysid;
        pMsghead->msgType = msgid;
        m_SendBufManager.pushPkgToList(pPkgHead->length);
        sendToSocket(); //send directly
        return  pPkgHead->length;
    }

    //int32 send(char *buff, int32 buffsize);  // logic module call to write msg to buffqueue return:-1 error -2 again >=0 success send length

    int32 sendToSocket(); //network layer call to send msg with socket
    int32 onRecv();  // network layer call to recv msg with socket
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

    inline void setSessionId(uint32 sessionId)
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
#if 0 
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
#endif

    inline void setSvrType(eSERVERTYPE type)
    {
        m_svrType = type;
    }
    
    inline eSERVERTYPE getSvrType()
    {
        return m_svrType;
    }

    inline uint8 getConnectSvrID()
    {
        return m_connSvrID;
    }

    inline void setConnectSvrID(uint8 svrid)
    {
        m_connSvrID = svrid;
    }

    inline bool getIsFromSelf()
    {
        return m_bIsFromSelf;
    }

    inline void setIsFromSelf(bool isFromSelf)
    {
        m_bIsFromSelf = isFromSelf;
    }

private:
    Int32 m_socket;
    char m_szIp[32];
    Int32 m_nPort;
    bool m_boActive;
    SESSION_TYPE m_eSessionType;
    struct sockaddr_in m_sockAddr;
    NetWorkObject *m_pBindNetWorkObj;
    #if 0 
    #ifdef USE_DOUBLE_QUEUE
    CIoBuff m_recvBuff;
    CIoBuff m_sendBuff;
    #else
    CRecvBuf m_recvBuff;
    CSendBuf m_sendBuff;
    #endif
    #endif
    CPkgbufManager m_RecvBufManager;
    CPkgbufManager m_SendBufManager;
    /*
    clwCore::CTwoQueues m_RecvTwoQueue;
    clwCore::CTwoQueues m_SendTwoQueue;
    */
    EpollServer *m_ptrServer;
    eSESSIONSTATUS m_eStatus;
    uint32 m_nSessionId;
    char *m_LeftPkgBuf;
    CpackageFetch m_pkgGet;
    PkgHeader m_header;
    eSERVERTYPE m_svrType;
    uint8 m_connSvrID;
    bool m_bIsFromSelf;// true means this session create by connector
};
#endif // __SESSION_H__
