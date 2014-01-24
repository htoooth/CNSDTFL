#ifndef MPI_CPL_DESIGN_PATTERN_H_INCLUDE

#define MPI_CPL_DESIGN_PATTERN_H_INCLUDE

///设计模式：单件
///使用单件来保存常用信息

namespace port
{

template <typename T>
class Singleton
{
public:
	static T * CreateInstance();
	static void DestoryInstance();

private:
	static T * m_pInstance;

	Singleton() {};
	Singleton(const Singleton &) {};
	Singleton & operator=(const Singleton &) {};
	~Singleton() {};
};

template <typename T>
T * Singleton<T>::CreateInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new T();
	}

	return m_pInstance;
}

template <typename T>
void Singleton<T>::DestoryInstance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

template <typename T>
T * Singleton<T>::m_pInstance = NULL;

}

#endif