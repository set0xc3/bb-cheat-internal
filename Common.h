#ifndef COMMON_SINGLETON_H
#define COMMON_SINGLETON_H

#include <cstdlib>

template<typename T>
class Singleton {
public:
    Singleton() {
        m_Instance = static_cast<T*>(this);
    }

    static T* GetInstancePtr() {
        if (m_Instance == NULL) {
            m_Instance = new T();
        }
        return m_Instance;
    }

    static T& GetInstance() {
        return *GetInstancePtr();
    }

    static void DeleteInstance() {
        delete m_Instance;
    }

private:
    static T* m_Instance;

    Singleton(const Singleton<T>&);
    Singleton& operator=(const Singleton<T>&);
};

template<typename T>
T* Singleton<T>::m_Instance = NULL;

#endif // COMMON_SINGLETON_H