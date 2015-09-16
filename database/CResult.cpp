#include "CResult.hpp"

CResult::CResult():m_res(NULL)
{
    
}

CResult::~CResult()
{
    if (NULL != m_res)
    {
        mysql_free_result(m_res);
    }
}
