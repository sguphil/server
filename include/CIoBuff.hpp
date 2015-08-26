#ifndef __CIOBUFF_H__
#define __CIOBUFF_H__

#include "baseHeader.h"
#include "CBuffQueue.hpp"
#include "packHeader.hpp"
#include "acctTimeTool.hpp"

class CIoBuff
{
public:
    CIoBuff();
    CIoBuff(int32 size, int32 extraSize);
    ~CIoBuff();

    void clear()
    {
        m_pRDQueue->clear();
        m_pWRQueue->clear();
        m_tempQueue.clear();
    }

    inline bool init(int32 size, int32 extraSize)
    {
        m_pRDQueue = &m_RDBuffQueue;
        m_pWRQueue = &m_buffQueue;
        m_buffQueue.init(size, extraSize);
        m_RDBuffQueue.init(size, extraSize);
        m_tempQueue.init(size, extraSize);
        return true;
    }

    inline bool checkHead(PkgHeader *header) //check if buff large than header size
    {
        int32 size = sizeof(*header);
        return size < m_pRDQueue->getBufLen();
    }

    inline bool checkMsg(PkgHeader *header) //check if buff is large than (sizeof(PkgHeader)+ msgsize)
    {
        int32 size = header->length;
        return size < m_pRDQueue->getBufLen();
    }

    //int32 getHead(PkgHeader *header);
    
    //int32 getMsg(char *buf, int32 bufsize);

    inline CBuffQueue<char>* getRDQueuePtr()
    {
        return m_pRDQueue;
    }

    inline CBuffQueue<char>* getWRQueuePtr()
    {
        return m_pWRQueue;
    }

    inline void lockSwap()
    {
        m_swapMutex.lock();
    }

    inline void unLockSwap()
    {
        m_swapMutex.unLock();
    }

    inline void swapQueue()
    {
        if (acct_time::getCurTimeMs() - m_nBuffSwapTick < m_nSwapFPS) 
        {
            //return;
        }

        m_nBuffSwapTick = acct_time::getCurTimeMs();

        lockSwap();
        if (m_pWRQueue->getBufLen() > 0)
        {
            //cout << "=================swap len is:" << m_pWRQueue->getBufLen() << endl;
            int32 rdLeftLen = m_pRDQueue->getBufLen();
            if (rdLeftLen > 0)
            {
                m_tempQueue.pushMsg(m_pRDQueue->getReadPtr(rdLeftLen), rdLeftLen); 
            }
            m_pRDQueue->clear();
            CBuffQueue<char> *tempRDptr = m_pRDQueue;
            m_pRDQueue = m_pWRQueue;
            m_pWRQueue = tempRDptr;
        }
        unLockSwap();
    }

    int32 checkLeftQueue(char* msg, PkgHeader* head) //cos there maybe some msg still in the readQueue if swap queues,so we need to handle the left msgbuf
    {
        int32 headsize = sizeof(*head);
        int32 leftLen = m_tempQueue.getBufLen();
        if (leftLen > 0)
        {
            if (leftLen >= headsize)
            {
                memcpy(head, m_tempQueue.getReadPtr(headsize), headsize);
                int32 msgSize = head->length;
                if (msgSize > MAXPKGLEN)
                {
                    return -1;
                }
                
                //int32 leftMsgLen = m_tempQueue.getBufLen();
                
                memcpy(msg, m_tempQueue.getReadPtr(leftLen), leftLen);
                int32 leftMsgLen2 = msgSize + sizeof(*head) - leftLen;
                if (leftMsgLen2 > 0)
                {
                    int32 readQueueLen = m_pRDQueue->getBufLen();
                    if (leftMsgLen2 <= readQueueLen)
                    {
                        memcpy(msg + leftLen, m_pRDQueue->getReadPtr(leftMsgLen2), leftMsgLen2);
                        m_tempQueue.popMsg(NULL, leftLen);
                        m_pRDQueue->popMsg(NULL, leftMsgLen2);
                    }
                    else
                    {
                        char buf[readQueueLen];
                        memcpy(buf, m_pRDQueue->getReadPtr(readQueueLen), readQueueLen);
                        assert(m_tempQueue.pushMsg(buf, readQueueLen) > 0);
                        m_pRDQueue->popMsg(NULL, readQueueLen);
                        return 0;
                    }
                }
                return msgSize;
            }
            else
            {
                int32 leftHeadSize = headsize - leftLen;
                memcpy((char *)head, m_tempQueue.getReadPtr(leftLen), leftLen);
                
                if (m_pRDQueue->getBufLen()>=leftHeadSize)
                {
                    memcpy((char *)head + leftLen, m_pRDQueue->getReadPtr(leftHeadSize), leftHeadSize);
                }
                else
                {
                    int32 cpLen = m_pRDQueue->getBufLen();
                    char buf[cpLen];
                    memcpy((char *)buf, m_pRDQueue->getReadPtr(cpLen), cpLen);
                    m_tempQueue.pushMsg(buf, cpLen);
                    m_pRDQueue->popMsg(NULL, cpLen);
                    return 0;
                }

                int32 msgSize = head->length;
                if (msgSize > MAXPKGLEN)
                {
                    return -1;
                }
                //int32 leftMsgLen = m_tempQueue.getBufLen();
                memcpy(msg, m_tempQueue.getReadPtr(leftLen), leftLen);
                int32 leftMsgSize = msgSize + sizeof(*head) - leftLen;
                int32 rdQlen = m_pRDQueue->getBufLen();
                if (rdQlen >= (leftMsgSize))
                {
                    memcpy((char *)msg+leftLen, m_pRDQueue->getReadPtr(leftMsgSize), leftMsgSize);
                    m_tempQueue.popMsg(NULL, leftLen);
                    m_pRDQueue->popMsg(NULL, leftMsgSize);
                }
                else
                {
                    char buf[rdQlen];
                    memcpy((char *)buf, m_pRDQueue->getReadPtr(rdQlen), rdQlen);
                    m_tempQueue.pushMsg(buf, rdQlen);
                    m_pRDQueue->popMsg(NULL, rdQlen);
                    return 0;
                }
                return msgSize;
            }
        }

        return 0;
    }
    
    //about send msg
    int32 putMsg(PkgHeader *header, char* msg, int32 msgSize)
    {
        //lockSwap();
        int32 pkgLen = msgSize + sizeof(*header);
        char buf[pkgLen];
        memcpy(buf, (char*)header, sizeof(*header));
        memcpy(buf + sizeof(*header), msg, msgSize);
        m_pWRQueue->pushMsg(buf,msgSize+sizeof(*header));
        //unLockSwap();
        return pkgLen;
    }

    int32 putMsg(char *buf, int32 buffsize)
    {
        lockSwap();
        m_pWRQueue->pushMsg(buf, buffsize);
        unLockSwap();
        return buffsize;
    }

    inline int32 getTempQueLen()
    {
        return m_tempQueue.getBufLen();
    }

    inline int32 getBuffSwapTick()
    {
        return m_nSwapFPS;
    }

    inline void setBuffSwapTick(int32 tick)
    {
        m_nSwapFPS = tick;
    }
protected:
    CIoBuff(CIoBuff& bufqueue);

    CIoBuff& operator=(CIoBuff& bufqueue);
private:
    CBuffQueue<char> m_buffQueue;
    CBuffQueue<char> m_RDBuffQueue;
    CBuffQueue<char> m_tempQueue;
    CBuffQueue<char> *m_pRDQueue;
    CBuffQueue<char> *m_pWRQueue;

    CMutex m_swapMutex;
    int32 m_nBuffSwapTick;
    int32 m_nSwapFPS;
};

#endif
