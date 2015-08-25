#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <assert.h>  
#include "../Thread/Mutex.h"

namespace clwCore  
{  
#define MALLOC_SIZE 128  
typedef CMutex CRITICAL_SECTION;

  
    typedef struct tagDataHead  
    {  
        tagDataHead()  
        {  
            pHead = NULL;  
            nLen = 0;  
            pNext = NULL;  
        }  
        void ReSet()  
        {  
            pHead = NULL;  
            nLen = 0;  
            pNext = NULL;  
        }  
        char* pHead;  
        unsigned int nLen;  
        tagDataHead* pNext;  
    }DATAHEAD;  
  
    typedef struct tagDataMem  
    {  
        tagDataMem(unsigned int nSize)  
        {  
            if (0 == nSize)  
            {  
                assert(false);  
                return ;  
            }  
  
            nMaxSize = nSize;  
            pDataMem = (char*)malloc(nSize*sizeof(char));  
            nDataPos = 0;  
  
            if (NULL == pDataMem)  
            {  
                // CTwoQueues??malloc?????  
                assert(false);  
            }  
  
            pListDataHead = NULL;  
            pCurDataHead = NULL;  
            pFreeDataHead = NULL;  
        }  
        ~tagDataMem()  
        {  
            // ????  
            ReSet();  
            while(NULL != pFreeDataHead)  
            {  
                DATAHEAD* pTempDataHead = pFreeDataHead;  
                pFreeDataHead = pFreeDataHead->pNext;  
  
                delete pTempDataHead;  
                pTempDataHead = NULL;  
            }  
  
            free(pDataMem);  
            pDataMem = NULL;  
            nDataPos = 0;  
        }  
        bool ReAlloc()  
        {  
            for (unsigned short i=0; i<MALLOC_SIZE; ++i)  
            {  
                DATAHEAD* pTempDataHead = new DATAHEAD;  
                //pTempDataHead->ReSet();    //????????  
  
                if (NULL == pTempDataHead)  
                {  
                    // ??DATAHEAD?????  
                    assert(false);  
                    return false;  
                }  
  
                pTempDataHead->pNext = pFreeDataHead;  
                pFreeDataHead = pTempDataHead;  
            }  
  
            return true;  
        }  
        DATAHEAD* GetDataHead()  
        {  
            if (NULL != pFreeDataHead)  
            {  
                DATAHEAD* pTempDataHead = pFreeDataHead;  
                pFreeDataHead = pFreeDataHead->pNext;  
  
                return pTempDataHead;  
            }  
  
            if (ReAlloc())  
            {  
                if (NULL != pFreeDataHead)  
                {  
                    DATAHEAD* pTempDataHead = pFreeDataHead;  
                    pFreeDataHead = pFreeDataHead->pNext;  
  
                    return pTempDataHead;  
                }  
            }  
  
            // ASSERT("GetDataHead??NULL?");  
            assert(false);  
            return NULL;  
        }  
        unsigned int GetFreeLen()   //??????  
        {  
            return nMaxSize-nDataPos;  
        }  
        bool PushData(void* pData, unsigned int nLen)  
        {  
            if (nDataPos+nLen >= nMaxSize)  
            {  
                return false;  
            }  
  
            DATAHEAD* pTempDataHead = GetDataHead();  
  
            if (NULL == pTempDataHead)  
            {  
                return false;  
            }  
  
            // ???????  
            pTempDataHead->pHead = (pDataMem+nDataPos);  
            pTempDataHead->nLen = nLen;  
            pTempDataHead->pNext = NULL;  
  
            // ????  
            memcpy(pDataMem+nDataPos, pData, nLen);  
            nDataPos += nLen;  
  
            if (NULL == pListDataHead)  
            {  
                pListDataHead = pTempDataHead;  
                pCurDataHead = pTempDataHead;  
                return true;  
            }  
            else  
            {  
                pCurDataHead->pNext = pTempDataHead;  
                pCurDataHead = pCurDataHead->pNext;  
                return true;  
            }  
        }  
  
        bool IsEmpty()  //???????  
        {  
            return (NULL==pListDataHead)?true:false;  
        }  
  
        bool PrepareData(const void*& pData, unsigned int& nLen)    //??????  
        {  
            if (NULL != pListDataHead)  
            {  
                pData = pListDataHead->pHead;  
                nLen = pListDataHead->nLen;  
                return true;  
            }  
            else  
            {  
                return false;  
            }  
        }  
        void ConfimData()   //??????  
        {  
            if (NULL == pListDataHead)  
            {  
                return ;  
            }  
  
            DATAHEAD* pTempDataHead = pListDataHead;  
            pListDataHead = pListDataHead->pNext;  
  
            pTempDataHead->ReSet();  
            pTempDataHead->pNext = pFreeDataHead;  
            pFreeDataHead = pTempDataHead;  
        }  
        void ReSet()    //????????  
        {  
            while(NULL != pListDataHead)  
            {  
                DATAHEAD* pTempDataHead = pListDataHead;  
                pListDataHead = pListDataHead->pNext;  
  
                pTempDataHead->ReSet();  
                pTempDataHead->pNext = pFreeDataHead;  
                pFreeDataHead = pTempDataHead;  
            }  
  
            nDataPos = 0;  
            pCurDataHead = NULL;  
        }  
  
        char* pDataMem;         //??????  
        unsigned int nDataPos;  //??????  
        unsigned int nMaxSize;  //????????  
  
        DATAHEAD* pListDataHead;  
        DATAHEAD* pCurDataHead;  
        DATAHEAD* pFreeDataHead;    //???????  
    }DATAMEM;  
  
    class CTwoQueues  
    {  
    public:  
        CTwoQueues(void)  
        {  
            //InitializeCriticalSection(&m_crit);  
            m_pDataMemPush = NULL;  
            m_pDataMemPop = NULL;  
        }  
        ~CTwoQueues(void)  
        {  
            if (NULL != m_pDataMemPush)  
            {  
                delete m_pDataMemPush;  
                m_pDataMemPush = NULL;  
            }  
  
            if (NULL != m_pDataMemPop)  
            {  
                delete m_pDataMemPop;  
                m_pDataMemPop = NULL;  
            }  
            //DeleteCriticalSection(&m_crit);  
        }  
  
    public:  
        void Init(unsigned int nSize)  
        {  
            if (0 == nSize)  
            {  
                // ???CTwoQueues?????  
                assert(false);  
                return ;  
            }  
  
            m_pDataMemPush = new DATAMEM(nSize);  
            m_pDataMemPop = new DATAMEM(nSize);  
        }  
  
        bool PushData(void* pData, unsigned int nLen)  
        {  
            //unsigned int nFreeLen = m_pDataMemPush->GetFreeLen();  
  
            bool bResult = false;  
  
            //EnterCriticalSection(&m_crit);  
            m_crit.lock();
            bResult = m_pDataMemPush->PushData(pData, nLen);  
            //LeaveCriticalSection(&m_crit);  
            m_crit.unLock();
  
            return bResult;  
        }  
        bool PrepareData(const void*& pData, unsigned int& nLen)  
        {  
            bool bCanRead = true;  
            if (m_pDataMemPop->IsEmpty())  
            {  
                // ???????  
                //EnterCriticalSection(&m_crit);  
                m_crit.lock();
                if (m_pDataMemPush->IsEmpty())  
                {  
                    // Push????  
                    //LeaveCriticalSection(&m_crit);  
                    m_crit.unLock();
                    bCanRead = false;  
                }  
                else  
                {  
                    m_pDataMemPop->ReSet();  //??????  
                    DATAMEM* pTempDataMem = m_pDataMemPush;  
                    m_pDataMemPush = m_pDataMemPop;  
                    m_pDataMemPop = pTempDataMem;  
                    //LeaveCriticalSection(&m_crit);  
                    m_crit.unLock();
                    bCanRead = true;  
                }  
            }  
  
            if (bCanRead)  
            {  
                return m_pDataMemPop->PrepareData(pData, nLen);  
            }  
            else  
            {  
                return false;  
            }  
        }  
  
        void ConfimData()  
        {  
            m_pDataMemPop->ConfimData();  
        }  
  
    private:  
        DATAMEM* m_pDataMemPush;  
        DATAMEM* m_pDataMemPop;  
        CRITICAL_SECTION m_crit;  
  
    };  
}

#endif
