#ifndef __CBUFFQUEUE_H__
#define __CBUFFQUEUE_H__
#include "baseHeader.h"
#include "../Thread/Mutex.h"
#include "../include/packHeader.hpp"

template<typename T>
class CBuffQueue
{
public:
    CBuffQueue():
        m_pData(NULL),
        m_pHead(NULL),
        m_pTail(NULL),
        m_nLength(0),
        m_nSize(0),
        m_nExtraSize(0)
    {
    }

    ~CBuffQueue()
    {
        delete[] m_pData;
        m_pData = NULL;
    }
    void clear()
    {
        m_pHead = m_pData;
        m_pTail = m_pData;
        m_nLength = 0;
        m_nSize = 0;
        m_nExtraSize = 0;
    }

    bool init(int32 size, int32 extraSize)
    {
        m_pData = new T[size + extraSize];
        if (NULL != m_pData)
        {
            m_pHead = m_pData;
            m_pTail = m_pData;
            m_nLength = 0;
            m_nSize = size;
            m_nExtraSize = extraSize;
            return true;
        }
        else
        {
            cout << "buffqueue init error" << endl;
            assert(false);
        }
        return false;
    }

    int32 pushMsg(T *target, int32 size)
    {
        AutoLock qlock(&m_mutex);
        assert(calcFreeSpace() >= size);
        if (calcFreeSpace() < size)
        {
            printf("message too long...buffreesize:%d, targetsize:%d\n", calcFreeSpace(), size);
            return -1;
        }
        
        if (target != NULL)
        {
            if (m_pHead <= m_pTail)
            {
                int32 backSize = m_nSize - ((m_pTail - m_pData) / sizeof(T));
                if (backSize >= size)
                {
                    memcpy(m_pTail, target, size * sizeof(T));
                }
                else
                {
                    memcpy(m_pTail, target, backSize*sizeof(T));
                    memcpy(m_pData, target + backSize*sizeof(T), (size - backSize) * sizeof(T));
                }
            }
            else
            {
                memcpy(m_pTail, target, size * sizeof(T));
            }
        }
        
        m_pTail += size * sizeof(T);
        m_pTail = m_pData + ((m_pTail - m_pData) % m_nSize);
        m_nLength += size;
        return size;
    }

    int32 popMsg(T *des, int32 size)
    {
        AutoLock qlock(&m_mutex);
        assert(m_nLength >= size);

        if (m_nLength < size)
        {
            return -1;
        }

        if (NULL != des)
        {
            if (m_pHead < m_pTail)
            {
                memcpy(des, m_pHead, size * sizeof(T));
            }
            else
            {
                int32 leftSize = m_nSize - ((m_pHead - m_pData) / sizeof(T));
                if (leftSize >= size)
                {
                    memcpy(des, m_pHead, size * sizeof(T));
                }
                else
                {
                    memcpy(des, m_pHead, leftSize * sizeof(T));
                    memcpy(des+leftSize, m_pData, (size - leftSize) * sizeof(T));
                }
            }
        }
        
        int32 HeadSize = ((m_pHead - m_pData) / sizeof(T) + size) % m_nSize;
        m_pHead = m_pData + HeadSize;
        m_nLength -= size;
        return size;
    }
    

    inline int32 calcFreeSpace()
    {
        AutoLock qlock(&m_mutex);
        return (m_nSize - m_nLength);
    }

    inline T* getReadPtr(int32 copySize) // if the backmsg is truncate into two parts, copy "copySize" memory from the head of buffqueue
    {
        AutoLock qlock(&m_mutex);
        T *ret = m_pHead;
        /*if (copySize>0) // always do
        {
            memcpy(m_pData + m_nSize, m_pData, m_nSize-getBackSize());
        }*/
        
        return ret;
    }

    inline T* getWritePtr()
    {
        AutoLock qlock(&m_mutex);
        return m_pTail;
    }
    
    inline int32 getBufLen()
    {
        AutoLock qlock(&m_mutex);
        return m_nLength;
    }
    
    inline int32 getReadableLen()
    {
        AutoLock qlock(&m_mutex);
        //m_mutex.lock();
        if (m_pHead == m_pTail)
        {
            int len = m_nSize - (m_pHead - m_pData) / sizeof(T);
            return (m_nLength > 0 ? len : 0);
        }
        elseif (m_pHead < m_pTail)
        {
            //m_mutex.unLock();
            return (m_pTail - m_pHead) / sizeof(T);
        } 
        else
        {
            int len = m_nSize - (m_pHead - m_pData) / sizeof(T);
            return len;  // just return backmem size
        }
    }

    inline int32 getWriteableLen()
    {
        AutoLock qlock(&m_mutex);
        if (m_pHead == m_pTail)
        {
            return (m_nLength > 0 ? 0 : m_nSize - (m_pTail - m_pData) / sizeof(T));
        }
        else if (m_pHead < m_pTail)
        {
            return (m_pTail - m_pHead) / sizeof(T);
        }
        else
        {
            return m_nSize - (m_pTail - m_pData) / sizeof(T);
        }
    }

    int32 recvFromSocket(int32 socket)
    {
        AutoLock qlock(&m_mutex);
        int32 canRecvlen = getWriteableLen();
        if (0 == canRecvlen)
        {
            return 0;
        }

        int32 recvlen = ::recv(socket, (void *)m_pTail, canRecvlen, 0);

        if (0 == recvlen)
        {
            printf("socket!!!!!!!!recv return 0!!!!!!!!\n");
            return -1;
        }
        else if (recvlen > 0)
        {
            pushMsg(NULL, recvlen);
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

    int32 sendToSocket(int32 socket)
    {
        AutoLock qlock(&m_mutex);
        int32 canSendlen = getReadableLen();
        if (0 == canSendlen)
        {
            return 0;
        }

        int32 sendlen = ::send(socket, getReadPtr(canSendlen), canSendlen, 0);

        if (0 == sendlen)
        {
            printf("socket!!!!!!!!recv return 0!!!!!!!!\n");
            return -1;
        }
        else if (sendlen > 0)
        {
            popMsg(NULL, sendlen);
            return sendlen;
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

    int32 getMsg(char* buf, int32 bufsize) //get without header
    {
        AutoLock qlock(&m_mutex);
        PkgHeader header;
        popMsg((char *)&header, sizeof(header));
        if (header.length > getBufLen())
        {
            return -1;
        }
        return popMsg(buf, bufsize);
    }

    int32 getHead(PkgHeader *header)
    {
        AutoLock qlock(&m_mutex);
        if (sizeof(*header) >= getBufLen())
        {
            return -1;
        }
    
        memcpy((char*)header, getReadPtr(sizeof(*header)), sizeof(*header));
        return sizeof(*header);
    }
protected:
    CBuffQueue(CBuffQueue &queue)
    {
    }
    CBuffQueue& operator=(CBuffQueue &queue)
    {
    }
private:
    T *m_pData;
    T *m_pHead;
    T *m_pTail;
    int32 m_nLength;
    int32 m_nSize;
    int32 m_nExtraSize;
    CMutex m_mutex;
};
#endif 
