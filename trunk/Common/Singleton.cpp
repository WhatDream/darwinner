///////////////////////////////////////////////////////////
//  Singleton.cpp
//  Implementation of the Class Singleton
//  Created on:      02-Apr-2009 21:26:26
//  Original author: Tim Kuo
///////////////////////////////////////////////////////////


#if !defined(EA_7651E40F_B1AE_46ba_BB3B_F8AE8EFBF7C8__INCLUDED_)
#define EA_7651E40F_B1AE_46ba_BB3B_F8AE8EFBF7C8__INCLUDED_

#include "common.h"

template<class T> class Singleton
{

public:
	~Singleton();

	static T* instance();

protected:
	Singleton();

private:
	static T* m_instance;

};


template<class T> T* Singleton<T>::m_instance = NULL;


template<class T> Singleton<T>::~Singleton(){

}

template<class T> Singleton<T>::Singleton(){

}


template<class T>  T* Singleton<T>::instance(){
	if (NULL == m_instance) {
		m_instance = new T();
	}
	DBINFO("create one singleton instance @", m_instance);
	return m_instance;
}

#endif // !defined(EA_7651E40F_B1AE_46ba_BB3B_F8AE8EFBF7C8__INCLUDED_)
