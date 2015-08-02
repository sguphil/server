#ifndef __SESSION_H__
#define __SESSION_H__
#include "../../include/baseHeader.h"
#include "NetWorkObject.h"
//class NetWorkObject;

class CSession
{
public:
    CSession();
    ~CSession();
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

    Int32 send(void *buff, int buffsize) { return 0; }
    Int32 recv();

    inline void bindNetWorkObj(NetWorkObject *networkObj)
    {
        m_pBindNetWorkObj = networkObj;
        m_pBindNetWorkObj->setSesion(this);
    }

    void processPacket()
    {
        //从缓冲区中取出完整数据包然后调用 m_pBindNetWorkObj->onRecv(packet, size) 让上层处理逻辑。
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
};
#endif // __SESSION_H__
