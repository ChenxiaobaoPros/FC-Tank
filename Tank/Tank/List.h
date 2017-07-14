#pragma once

template <class N>
class ListNode		// ˫������ + ѭ������
{
public:
	ListNode(const N&, ListNode<N>*);
	~ListNode();
	N GetNode();

	ListNode<N>* pnext;
	N data;
};


template <class T>
class ListTable
{
public:
	ListTable();
	~ListTable();

	void Add(T);
	void Delete(T);
	int Size();
	ListNode<T>* First();

private:
	ListNode<T>* phead;
	int size;
};


////////////////


template<class N>
ListNode<N>::ListNode(const N& d, ListNode<N>* phead)
{
	pnext = phead;
	data = d;
}

template<class N>
ListNode<N>::~ListNode()
{
	if (pnext != NULL)
		delete pnext;
}

template<class N>
N ListNode<N>::GetNode()
{
	return data;
}

////////////////////////////////////////////

template<class T>
ListTable<T>::ListTable()
{
	phead = NULL;
	size = 0;
}

template<class T>
ListTable<T>::~ListTable()
{
	ListNode<T>* temp = phead;
	for (ListNode<T>* t = phead; t != NULL; )
	{
		temp = t;
		t = t->pnext;
		delete temp;
	}
}


/*
* ����ͷ������
*/
template<class T>
void ListTable<T>::Add(T t)
{
	/*
	* �½��Ľڵ�ָ��ԭ�� phead ָ��Ľڵ�,
	* Ȼ�� phead �任ָ���½��Ľڵ�
	*/
	phead = new ListNode<T>(t, phead);
	size++;
}

template<class T>
void ListTable<T>::Delete(T val)
{
	ListNode<T>* temp = phead;
	for (ListNode<T>* t = phead; t != NULL; temp = t, t = t->pnext)
	{
		if (t->data == val && t == phead)
		{
			phead = t->pnext;
			delete t;
			size--;
		}
		else if (t->data == val)
		{
			temp->pnext = t->pnext;
			delete t;
			size--;
		}
	}
}

template<class T>
int ListTable<T>::Size()
{
	return size;
}

template<class T>
ListNode<T>* ListTable<T>::First()
{
	return phead;
}
