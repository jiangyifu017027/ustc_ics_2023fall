#include <iostream>
#include <assert.h>

//把double改成T即可
template<typename T>
DArray<T>::DArray() {
	Init();
}

template<typename T>
DArray<T>::DArray(int nSize, const T& dValue)
	: m_pData(new T[nSize]), m_nSize(nSize), m_nMax(nSize)
{
	for (int i = 0; i < nSize; i++)
		m_pData[i] = dValue;
}

template<typename T>
DArray<T>::DArray(const DArray& arr)
	: m_pData(new T[arr.m_nSize]), m_nSize(arr.m_nSize), m_nMax(arr.m_nSize)
{
	for (int i = 0; i < m_nSize; i++)
		m_pData[i] = arr.m_pData[i];
}

template<typename T>
DArray<T>::~DArray()
{
	Free();
}

template<typename T>
void DArray<T>::Print() const {
	std::cout << "size= " << m_nSize << ":";
	for (int i = 0; i < m_nSize; i++)
		std::cout << " " << GetAt(i);

	std::cout << std::endl;
}

template<typename T>
void DArray<T>::Init() {
	m_pData = nullptr;
	m_nSize = 0;
	m_nMax = 0;
}

template<typename T>
void DArray<T>::Free() {
	delete[] m_pData;
	m_pData = nullptr;

	m_nSize = 0;
	m_nMax = 0;
}

template<typename T>
int DArray<T>::GetSize() const {
	return m_nSize;
}

template<typename T>
void DArray<T>::Reserve(int nSize) {
	if (m_nMax >= nSize)
		return;
	if (m_nMax == 0)m_nMax++;
	while (m_nMax < nSize)
		m_nMax *= 2;

	T* pData = new T[m_nMax];

	for (int i = 0; i < m_nSize; i++)
		pData[i] = m_pData[i];

	delete[] m_pData;
	m_pData = pData;
}

template<typename T>
void DArray<T>::SetSize(int nSize) {
	if (m_nSize == nSize)
		return;

	Reserve(nSize);

	for (int i = m_nSize; i < nSize; i++)
		m_pData[i] = static_cast<T>(0);

	m_nSize = nSize;
}

template<typename T>
const T& DArray<T>::GetAt(int nIndex) const {
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

template<typename T>
void DArray<T>::SetAt(int nIndex, const T& dValue) {
	assert(nIndex >= 0 && nIndex < m_nSize);
	m_pData[nIndex] = dValue;
}

template<typename T>
const T& DArray<T>::operator[](int nIndex) const {
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

template<typename T>
void DArray<T>::PushBack(const T& dValue) {
	Reserve(m_nSize + 1);

	m_pData[m_nSize] = dValue;
	m_nSize++;
}

template<typename T>
void DArray<T>::DeleteAt(int nIndex) {
	assert(nIndex >= 0 && nIndex < m_nSize);

	for (int i = nIndex + 1; i < m_nSize; i++)
		m_pData[i - 1] = m_pData[i];

	m_nSize--;
}
template<typename T>
void DArray<T>::InsertAt(int nIndex, const T& dValue) {
	assert(nIndex >= 0 && nIndex <= m_nSize);
	Reserve(m_nSize + 1);

	for (int i = m_nSize; i > nIndex; i--)
		m_pData[i] = m_pData[i - 1];

	m_pData[nIndex] = dValue;

	m_nSize++;
}

template<typename T>
DArray<T>& DArray<T>::operator = (const DArray& arr) {
	if (this == &arr)
		return *this;

	delete[] m_pData;

	m_nSize = arr.m_nSize;
	m_nMax = arr.m_nMax;

	m_pData = new T[m_nMax];

	for (int i = 0; i < m_nSize; i++)
		m_pData[i] = arr[i];

	return *this;
}
