// ---------------------------------------------------------------------------
#ifndef uCRListTH
#define uCRListTH
// ---------------------------------------------------------------------------
#include <classes.hpp>

// ---------------------------------------------------------------------------
template<class T>
class CRListT
{
private:
	TList* L;

public:
	CRListT();
	~CRListT();
	T* operator[](int _i);
	T* Add(void);
	T* Add(T* _p);

	inline int Count(void)
	{
		return (L->Count);
	};
	void Clear(void);
	T* Insert(int _i);
	T* Insert(int _i, T* _p);
	void Reverse(void);
	void Remove(T* _p);
	void Delete(int _i);
	void Capacity(int _i);
};

// ------------------------------------------------------------------------------
template<class T>
CRListT<T>::CRListT()
{
	L = new TList;
}

// ---------------------------------------------------------------------------
template<class T>
CRListT<T>::~CRListT()
{
	T* p;
	for (int i = 0; i < L->Count; i++)
	{
		p = (T*)L->Items[i];
		delete p;
	}
	delete L;
}

// ---------------------------------------------------------------------------
template<class T>
T* CRListT<T>:: operator[](int _i)
{
	if (_i > L->Count)
		return (NULL);
	return ((T*)L->Items[_i]);
}

// ---------------------------------------------------------------------------
template<class T>
T* CRListT<T>::Add(void)
{
	T* p = new T;
	L->Add(p);
	return (p);
};

// ---------------------------------------------------------------------------
template<class T>
T* CRListT<T>::Add(T* _p)
{
	L->Add(_p);
	return (_p);
};

// ---------------------------------------------------------------------------
template<class T>
void CRListT<T>::Clear(void)
{
	T* p;
	for (int i = 0; i < L->Count; i++)
	{
		p = (T*)L->Items[i];
		delete p;
	}
	L->Clear();
}

// ---------------------------------------------------------------------------
template<class T>
T* CRListT<T>::Insert(int _i)
{
	if (_i < 0 || _i >= L->Count)
		return (Add());
	T* p = new T;
	L->Insert(_i, p);
	return (p);
}

// ---------------------------------------------------------------------------
template<class T>
T* CRListT<T>::Insert(int _i, T* _p)
{
	if (_i < 0 || _i >= L->Count)
		return (Add(_p));
	L->Insert(_i, _p);
	return (p);
}

// ---------------------------------------------------------------------------
template<class T>
void CRListT<T>::Reverse(void)
{

	if (L->Count <= 2)
		return;
	int count = L->Count / 2;
	void* buf;
	for (int i = 0; i < count; i++)
	{
		buf = L->Items[i];
		L->Items[i] = L->Items[L->Count - 1 - i];
		L->Items[L->Count - 1 - i] = buf;
	}
}
// ---------------------------------------------------------------------------
template<class T> void CRListT<T>::Remove(T* _p)
{
	L->Remove(_p);
	delete _p;
}
template<class T> void CRListT<T>::Delete(int _i)
{
	T* p = (T*)L->Items[_i];
	delete p;
	L->Delete(_i);
}
//---------------------------------------------------------------------------
template<class T>
void CRListT<T>::Capacity(int _i)
{
	L->Capacity=_i;
}
#endif
