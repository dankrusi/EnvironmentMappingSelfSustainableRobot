/*
 * BinaryPriorityQueue.h
 *
 *  Created on: Mar 14, 2009
 *      Author: dankrusi
 */

#ifndef BINARYPRIORITYQUEUE_H_
#define BINARYPRIORITYQUEUE_H_

#include <QList>

template <typename T>
class BinaryPriorityQueue {

private:
	QList<T> *innerList;

public:
	BinaryPriorityQueue();
	virtual ~BinaryPriorityQueue();
	void switchElements(int i, int j);
	//int compare(int i, int j);
	int push(const T & o);
	const T & pop();
	void update(int i);
	const T & peek();
	bool contains(const T & value);
	void clear();
	int count();
	typename QList<T>::ConstIterator getEnumerator() const;
	const T & at(int index);
	void set(int index, const T & value);
	int add(const T & o);

};

template <typename T>
BinaryPriorityQueue<T>::BinaryPriorityQueue() {
	innerList = new QList<T>();
}

template <typename T>
BinaryPriorityQueue<T>::~BinaryPriorityQueue() {
	delete innerList;
}



template <typename T>
void BinaryPriorityQueue<T>::switchElements(int i, int j)
{
//	T h = innerList[i];
//	innerList[i] = innerList[j];
//	innerList[j] = h;
	innerList->swap(i,j);
}

//template <typename T>
//int BinaryPriorityQueue<T>::compare(int i, int j)
//{
//	int i = innerList->count();
//	//TODO!
//	if(innerList->at(i) == innerList->at(j)) return 0;
//	if(innerList->at(i) > innerList->at(j)) return 1;
//	else return -1;
//	//return innerList[i] == innerList[j];
//	//return Comparer.Compare(InnerList[i],InnerList[j]);
//}


/// <summary>
/// Push an object onto the PQ
/// </summary>
/// <param name="O">The new object</param>
/// <returns>The index in the list where the object is _now_. This will change when objects are taken from or put onto the PQ.</returns>
template <typename T>
int BinaryPriorityQueue<T>::push(const T &  o)
{
	int p = innerList->count(), p2;
	innerList->append(o); // E[p] = O
	do {
		if(p==0) break;
		p2 = (p-1)/2;
		//if(compare(p,p2)<0) {
		if(p < p2) {
			switchElements(p,p2);
			p = p2;
		} else break;
	} while(true);
	return p;
}

/// <summary>
/// Get the smallest object and remove it.
/// </summary>
/// <returns>The smallest object</returns>
template <typename T>
const T &  BinaryPriorityQueue<T>::pop()
{
	//int i = innerList->count();
	if(innerList->count()==0) return NULL;
	T result = innerList->at(0);
	int p = 0,p1,p2,pn;
	innerList[0] = innerList[innerList->count()-1];
	innerList->removeAt(innerList->count()-1);
	do
	{
		pn = p;
		p1 = 2*p+1;
		p2 = 2*p+2;
		if(innerList->count()>p1 && p > p1) // links kleiner
			p = p1;
		if(innerList->count()>p2 && p > p2) // rechts noch kleiner
			p = p2;

		if(p==pn)
			break;
		switchElements(p,pn);
	}while(true);
	return result;
}

/// <summary>
/// Notify the PQ that the object at position i has changed
/// and the PQ needs to restore order.
/// Since you dont have access to any indexes (except by using the
/// explicit IList.this) you should not call this function without knowing exactly
/// what you do.
/// </summary>
/// <param name="i">The index of the changed object.</param>
template <typename T>
void BinaryPriorityQueue<T>::update(int i)
{
	int p = i,pn;
	int p1,p2;
	// aufsteigen
	do {
		if(p==0)
			break;
		p2 = (p-1)/2;
		if(p<p2)
		{
			switchElements(p,p2);
			p = p2;
		}
		else
			break;
	} while(true);
	if(p<i) return;
	// absteigen
	do {
		pn = p;
		p1 = 2*p+1;
		p2 = 2*p+2;
		if(innerList->Count>p1 && p>p1) // links kleiner
			p = p1;
		if(innerList->Count>p2 && p>p2) // rechts noch kleiner
			p = p2;

		if(p==pn)
			break;
		switchElements(p,pn);
	}while(true);
}

/// <summary>
/// Get the smallest object without removing it.
/// </summary>
/// <returns>The smallest object</returns>
template <typename T>
const T & BinaryPriorityQueue<T>::peek()
{
	if(innerList->count() > 0) 	return innerList[0];
	else						return NULL;
}

template <typename T>
bool BinaryPriorityQueue<T>::contains(const T & value)
{
	return innerList->contains(value);
}

template <typename T>
void BinaryPriorityQueue<T>::clear()
{
	innerList->clear();
}

template <typename T>
int BinaryPriorityQueue<T>::count()
{
	return innerList->count();
}

template <typename T>
typename QList<T>::ConstIterator BinaryPriorityQueue<T>::getEnumerator() const
{
	return innerList->begin();
}




template <typename T>
const T & BinaryPriorityQueue<T>::at(int index) {
	return innerList[index];
}

template <typename T>
void BinaryPriorityQueue<T>::set(int index, const T & value) {
	innerList[index] = value;
	this->udpate(index);
}



template <typename T>
int BinaryPriorityQueue<T>::add(const T &  o)
{
	return push(o);
}


#endif /* BINARYPRIORITYQUEUE_H_ */
