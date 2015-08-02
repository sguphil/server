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
    
}
