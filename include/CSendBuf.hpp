#ifndef __CSENDBUF_H__
#define __CSENDBUF_H__
#include "baseHeader.h"
#include "packHeader.hpp"
#include "CBuffQueue.hpp"

class CSendBuf
{
public:
    CSendBuf();
    ~CSendBuf();
    bool init(int32 size, int32 extraSize);
    int32 putMsg(PkgHeader *header, char* msg, int32 msgSize);
    
    inline CBuffQueue<char>* getBuffQueuePtr()
    {
        return &m_sendbuf;
    }
protected:
    CSendBuf(CSendBuf& bufqueue);

    CSendBuf& operator=(CSendBuf& bufqueue);
private:
    CBuffQueue<char> m_sendbuf;
};

#endif 
