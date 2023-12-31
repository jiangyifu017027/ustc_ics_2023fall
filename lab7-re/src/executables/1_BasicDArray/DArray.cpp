// implementation of class DArray
#include "DArray.h"
#include<iostream>
using namespace std;

// default constructor
DArray::DArray() {
	Init();
}

// set an array with default values
DArray::DArray(int nSize, double dValue) :m_pData(new double[nSize]), m_nSize(nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		m_pData[i] = dValue;
	}
	//TODO
}

DArray::DArray(const DArray& arr) {
	m_nSize = arr.GetSize();
	m_pData = new double[m_nSize];
	for (int i = 0; i < m_nSize; i++)
	{
		m_pData[i] = arr[i];
	}
	//TODO
}

// deconstructor
DArray::~DArray() {
	Free();
}

// display the elements of the array
void DArray::Print() const {
	for (int i = 0; i < m_nSize; i++)
	{
		cout << " " << GetAt(i);
	}
	cout << endl;
	//TODO
}

// initilize the array
void DArray::Init() {
	m_nSize = 0;
	m_pData = nullptr;
	//TODO
}

// free the array
void DArray::Free() {
	delete[] m_pData;
	m_pData = nullptr;
	m_nSize = 0;
	//TODO
}

// get the size of the array
int DArray::GetSize() const {
	//TODO
	return m_nSize; // you should return a correct value
}

// set the size of the array
void DArray::SetSize(int nSize) {
	if (m_nSize == nSize)return;
	int temp = m_nSize > nSize ? nSize : m_nSize;
	double* p = new double[nSize];
	for (int i = 0; i < temp; i++)
	{
		p[i] = m_pData[i];
	}
	for (int i = temp; i < nSize; i++)
	{
		p[i] = 0;
	}
	m_nSize = nSize;
	m_pData = p;
	//TODO
}

// get an element at an index
const double& DArray::GetAt(int nIndex) const {
	//TODO
	if (nIndex < 0 || nIndex >= m_nSize)
	{
		cout << "wrong" << endl;
		return 0;
	}
	return m_pData[nIndex];
}

// set the value of an element 
void DArray::SetAt(int nIndex, double dValue) {
	if (nIndex < 0 || nIndex >= m_nSize)
	{
		cout << "wrong" << endl;
		return;
	}
	m_pData[nIndex] = dValue;
	//TODO
}

// overload operator '[]'
const double& DArray::operator[](int nIndex) const {
	//TODO
	if (nIndex < 0 || nIndex >= m_nSize)
	{
		cout << "wrong" << endl;
		return 0;
	}
	return m_pData[nIndex];
}

// add a new element at the end of the array
void DArray::PushBack(double dValue) {
	double* pTemp = new double[m_nSize + 1];
	for (int i = 0; i < m_nSize; i++)
		pTemp[i] = m_pData[i];
	pTemp[m_nSize] = dValue;
	delete[] m_pData;
	m_pData = pTemp;
	m_nSize++;
	//TODO
}

// delete an element at some index
void DArray::DeleteAt(int nIndex) {
	if (nIndex < 0 || nIndex >= m_nSize)
	{
		cout << "wrong" << endl;
		return;
	}
	double* p = new double[m_nSize - 1];
	for (int i = 0; i < nIndex; i++)
		p[i] = m_pData[i];

	for (int i = nIndex; i < m_nSize - 1; i++)
		p[i] = m_pData[i + 1];
	delete[] m_pData;
	m_nSize--;
	m_pData = p;
	//TODO
}

// insert a new element at some index
void DArray::InsertAt(int nIndex, double dValue) {
	//TODO
	if (nIndex < 0 || nIndex > m_nSize)
	{
		cout << "wrong" << endl;
		return;
	}
	double* pTemp = new double[m_nSize + 1];
	for (int i = 0; i < nIndex; i++)
		pTemp[i] = m_pData[i];
	pTemp[nIndex] = dValue;
	for (int i = nIndex + 1; i < m_nSize + 1; i++)
		pTemp[i] = m_pData[i - 1];
	delete[] m_pData;
	m_pData = pTemp;
	m_nSize++;
}

// overload operator '='
DArray& DArray::operator = (const DArray& arr) {
	//TODO
	delete[] m_pData;
	m_pData = new double[arr.m_nSize];
	m_nSize = arr.m_nSize;
	for (int i = 0; i < m_nSize; i++)
	{
		m_pData[i] = arr[i];
	}
	return *this;
}
