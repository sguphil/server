#ifndef SINGLETON_H
#define SINGLETON_H
#include "baseHeader.h"

namespace base
{
    template <typename T>
    class Singleton
    {
        public:
            static T* GetInstance()
            {
                //static T instance;
                //return &instance;
                ///*
                if (NULL == instance)
                {
                    instance = new T();
                    return instance;
                }
                else
                {
                    return instance;
                }
                //*/
            }

            static void DestroyInstance()
            {
                ///*
                if (NULL != instance)
                {
                    delete instance;
                    instance = NULL;
                }
                //*/
            }

        protected:
            Singleton() {};
            virtual ~Singleton() {};

        private:
            static T* instance;
    };

    template <typename T> T* Singleton<T>::instance = NULL;
}

#endif // SINGLETON_H
