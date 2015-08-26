#ifndef __CRESULT_H__
#define __CRESULT_H__
#include "../include/baseHeader.h"
#include "mysql/mysql.h"

class CResult
{
public:
    CResult();
    ~CResult();
    bool hasNext()
    {
        m_curRow = mysql_fetch_row(m_res);

        if (NULL == m_curRow)
        {
            return false;
        }
        m_nCurReadField = 0;
        return true;
    }
/*
    template<typename T>
    T* getFieldbyT(T& arg, int i)
    {
        if (i > m_nCol || i< 0)
        {
            return NULL;
        }
        stringstream ss;
        ss.str(m_curRow[i]);
        printf("strlen m_curRow:%d\n", (int32)strlen(m_curRow[i]));
        ss >> arg;
        //m_nCurReadField += 1;
        return &arg;
    }
*/
    char* getString(int field)
    {
        if (field < m_nCol)
        {
            return m_curRow[field];
        }
        else
        {
            return NULL;
        }
    }

    int32 getInt(int field)
    {
        if (field < m_nCol)
        {
            return atoi(m_curRow[field]);
        }
        else
        {
            return -1;
        }
    }

    uint32 getUint(int field)
    {
        if (field < m_nCol)
        {
            return (uint32)atoi(m_curRow[field]);
        }
        else
        {
            return -1;
        }
    }

    float getFloat(int field)
    {
        if (field < m_nCol)
        {
            return atof(m_curRow[field]);
        }
        else
        {
            return -1;
        }
    }

    double getDouble(int field)
    {
        if (field < m_nCol)
        {
            return atof(m_curRow[field]);
        }
        else
        {
            return -1;
        }
    }

    inline int32 getRow()
    {
        return m_nRow;
    }

    inline int32 getCol()
    {
        return m_nCol;
    }

    inline void setRow(int32 row)
    {
        m_nRow = row;
    }

    inline void setCol(int32 col)
    {
        m_nCol = col;
    }
    
    inline MYSQL_RES* getRes()
    {
        return m_res;
    }

    inline void setRes(MYSQL_RES *res)
    {
        m_res = res;
    }
private:
    int32 m_nRow;
    int32 m_nCol;
    int32 m_nCurReadField;
    MYSQL_RES *m_res;
    MYSQL_ROW m_curRow;
    
};
#endif
