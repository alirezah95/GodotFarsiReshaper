#ifndef RVECTOR_CPP
#define RVECTOR_CPP

#include <memory>
#include <vector>
#include <stdexcept>
#include <math.h>
#include <limits>


using std::unique_ptr;
using std::make_unique;

typedef unsigned long	ulong;
typedef unsigned int	uint;
typedef unsigned char	uint8_t;

template<class T>
class Rvector
{
public:
	typedef T	value_type;
	typedef T*	iterator;
	typedef const T*	const_iterator;


	struct Iterator {
		value_type* ptr;
		Iterator(value_type* __ptr): ptr(__ptr) {}
		bool operator!=(Iterator __ptr) { return ptr != __ptr.ptr; }
		value_type& operator*() { return *ptr; }
		void operator++() { ++ptr; }
	};


	explicit Rvector();
	~Rvector();
	size_t size();
	size_t capacity();
	int lastIndx();
	Iterator begin();
	Iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	void push_front(T __val);
	value_type& at(uint __indx) noexcept(false);
	const value_type& at(uint __indx) const noexcept(false);
	void prepend(const std::vector<value_type>& __other);
	void prepend(const Rvector<value_type>& __other);
	void prepend(const value_type* __other, size_t __size);
	void clear();
private:
	void reallocAndMove(size_t newCapacity);
private:
	static const size_t sMAX_SIZE;
	const uint8_t cINITIAL_SIZE = 2;

	T* mData;
	int mLastIndx;
	size_t mSize;
	size_t mCapacity;
};


template<class T>
const size_t Rvector<T>::sMAX_SIZE = std::vector<T>().max_size();

template<class T>
Rvector<T>::Rvector()
{
	mData= new T[cINITIAL_SIZE];
	mCapacity = cINITIAL_SIZE;
	mLastIndx = mCapacity;
	mSize = 0;
}


template<class T>
size_t Rvector<T>::size()
{
	return mSize;
}// size()


template<class T>
size_t Rvector<T>::capacity()
{
	return mCapacity;
}// capacity()

template<class T>
int Rvector<T>::lastIndx()
{
	return mLastIndx;
}

template<class T>
typename Rvector<T>::Iterator Rvector<T>::begin()
{
	return &mData[mLastIndx];
}

template<class T>
typename Rvector<T>::Iterator Rvector<T>::end()
{
	return &mData[mCapacity];
}

template<class T>
typename Rvector<T>::const_iterator Rvector<T>::begin() const
{
	return &mData[mLastIndx];
}

template<class T>
typename Rvector<T>::const_iterator Rvector<T>::end() const
{
	return &mData[mCapacity];
}


template<class T>
void Rvector<T>::push_front(T __val)
{
	if (mSize >= mCapacity) {
		reallocAndMove(mCapacity * 2);
	}

	mLastIndx--;
	mData[mLastIndx] = __val;
	mSize++;

	return;
}// push_front()



template<class T>
void Rvector<T>::reallocAndMove(size_t __newCapacity)
{
	auto newData = new T[__newCapacity];

	uint newLastIndx = __newCapacity;
	for (int i = mCapacity - 1; i >= mLastIndx ; --i) {
		newLastIndx--;
		newData[newLastIndx] = mData[i];
	}
	mCapacity = __newCapacity;
	mLastIndx = newLastIndx;

	delete [] mData;
	mData = newData;
	newData = nullptr;

	return;
}// reallocAndMove()



template<class T>
T& Rvector<T>::at(uint __indx)
{
	if (__indx < mLastIndx || __indx >= mCapacity) {
		std::out_of_range e("Out of range index value");
		throw e;
	}

	return mData[__indx];

}// at()



template<class T>
const T& Rvector<T>::at(uint __indx) const
{
	if (__indx < mLastIndx || __indx >= mCapacity) {
		std::out_of_range e("Out of range index value");
		throw e;
	}

	return mData[__indx];
}// at()


template<class T>
void Rvector<T>::prepend(const std::vector<T>& __other)
{
	if (__other.size() + mSize > sMAX_SIZE) {
		std::length_error e("Size can not exceeds MAX_SIZE");
		throw e;
	}
	size_t newCapacity = mCapacity;
	while (__other.size() + mSize > newCapacity) {
		newCapacity *= 2;
	}

	if (newCapacity != mCapacity)
		reallocAndMove(newCapacity);
	for (int i = __other.size() - 1; i >= 0; --i) {
		push_front(__other[i]);
	}

	return;
}// prepend


template<class T>
void Rvector<T>::prepend(const Rvector<T>& __other)
{
	if (__other.size() + mSize > sMAX_SIZE) {
		std::length_error e("Size can not exceeds MAX_SIZE");
		throw e;
	}
	size_t newCapacity = mCapacity;
	while (__other.size() + mSize > newCapacity) {
		newCapacity *= 2;
	}

	if (newCapacity != mCapacity)
		reallocAndMove(newCapacity);
	for (const T& val: __other) {
		push_front(val);
	}

	return;
}// prepend


template<class T>
void Rvector<T>::prepend(const T* __other, size_t __size)
{
	if (__size + mSize > sMAX_SIZE) {
		std::length_error e("Size can not exceeds MAX_SIZE");
		throw e;
	}
	size_t newCapacity = mCapacity;
	while (__size + mSize > newCapacity) {
		newCapacity *= 2;
	}

	if (newCapacity != mCapacity)
		reallocAndMove(newCapacity);
	for (int i = __size - 1; i >= 0; --i) {
		push_front(__other[i]);
	}

	return;
}// prepend


template<class T>
void Rvector<T>::clear()
{
	delete [] mData;
	mData = new T[cINITIAL_SIZE];
	mCapacity = cINITIAL_SIZE;
	mLastIndx = mCapacity;
	mSize = 0;
}


template<class T>
Rvector<T>::~Rvector()
{
	delete [] mData;
	mData = nullptr;
}


#endif
