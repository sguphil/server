#ifndef __BASEFACTORY_H__
#define __BASEFACTORY_H__
#include "../include/baseHeader.h"
#include "../Logic/Player.h"
template<typename T>
class CBaseFactory
{
public:
    CBaseFactory() {}
    virtual ~CBaseFactory() { }
    virtual void init(Int32 ItemNum, Int32 accNum)
    {
        m_nAccNum = accNum;
        addItem(ItemNum, true);
    }

    virtual T* allocate()
    {
        if (m_nItemLess <= 0)
        {
            addItem(m_nAccNum, false);
        }

        T* allocItem = m_ItemList.front();
        m_ItemList.pop_front();
        m_nItemLess --;
        return allocItem;
    }

    virtual void reuse(T *item )
    {
        m_ItemList.push_back(item);
        m_nItemLess++;
    }
    virtual void freeItem(){}

private:
    void addItem(Int32 num, bool flashTotalNum = false)
    {
        if (num > 0)
        {
            for (int i=0; i<num; i++)
            {
                T *addPlayer = new T;
                m_ItemList.push_back(addPlayer);
                m_nItemLess++;
                if (flashTotalNum)
                    m_nTotalItem++;
            }
        }
    }

private:
    list<T*> m_ItemList;
    Int32 m_nTotalItem;
    Int32 m_nItemLess;
    Int32 m_nAccNum;
};
#endif // __BASEFACTORY_H__
