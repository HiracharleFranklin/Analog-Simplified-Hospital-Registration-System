#pragma once
#include "BPlus_node.h"
#include <iostream>
using namespace std;
// CNode
template <typename KeyType, typename DataType>
CNode<KeyType, DataType>::CNode()
{
	setType(LEAF);
	setKeyNum(0);
}

template <typename KeyType, typename DataType>
CNode<KeyType, DataType>::~CNode()
{
	setKeyNum(0);
}

template <typename KeyType, typename DataType>
NODE_TYPE CNode<KeyType, DataType>::getType() const { return m_Type; }

template <typename KeyType, typename DataType>
void CNode<KeyType, DataType>::setType(NODE_TYPE type) { this->m_Type = type; }

template <typename KeyType, typename DataType>
int CNode<KeyType, DataType>::getKeyNum() const
{
	return m_KeyNum;
}

template <typename KeyType, typename DataType>
void CNode<KeyType, DataType>::setKeyNum(int n)
{
	m_KeyNum = n;
}

template <typename KeyType, typename DataType>
KeyType CNode<KeyType, DataType>::getKeyValue(int i) const
{
	return m_KeyValues[i];
}

template <typename KeyType, typename DataType>
void CNode<KeyType, DataType>::setKeyValue(int i, KeyType key)
{
	m_KeyValues[i] = key;
}

template <typename KeyType, typename DataType>
int CNode<KeyType, DataType>::getKeyIndex(KeyType key) const
{
	// cout << "Now get index of " << key << endl;
	if (getKeyNum() == 0)
		return 0;
	// int left = 0;
	// int right = getKeyNum() - 1;
	// int current;
	// while (left != right)
	// {
	// 	current = (left + right) / 2;
	// 	KeyType currentKey = getKeyValue(current);
	// 	if (key > currentKey)
	// 	{
	// 		left = current + 1;
	// 	}
	// 	else
	// 	{
	// 		right = current;
	// 	}
	// }
	int ret = this->getKeyNum() - 1;
	for (int i=0;i<this->getKeyNum();i++)
	{
		if (this->m_KeyValues[i] >= key)
		{
			ret = i;
			// cout << "index found: " << ret << endl;
			break;
		}
	}
	return ret;
}

// CInternalNode
template <typename KeyType, typename DataType>
CInternalNode<KeyType, DataType>::CInternalNode()
{
	this->setType(INTERNAL);
}

template <typename KeyType, typename DataType>
CInternalNode<KeyType, DataType>::~CInternalNode()
{
}

template <typename KeyType, typename DataType>
CNode<KeyType, DataType> *CInternalNode<KeyType, DataType>::getChild(int i) const { return m_Childs[i]; }

template <typename KeyType, typename DataType>
void CInternalNode<KeyType, DataType>::setChild(int i, CNode<KeyType, DataType> *child) { m_Childs[i] = child; }

template <typename KeyType, typename DataType>
void CInternalNode<KeyType, DataType>::clear()
{
	for (int i = 0; i <= this->m_KeyNum; ++i)
	{
		if (!m_Childs[i])
		{
			m_Childs[i]->clear();
			delete m_Childs[i];
			m_Childs[i] = NULL;
		}
	}
}

template <typename KeyType, typename DataType>
void CInternalNode<KeyType, DataType>::split(CNode<KeyType, DataType> *parentNode, int childIndex)
{
	CInternalNode<KeyType, DataType> *newNode = new CInternalNode<KeyType, DataType>; //分裂后的右节点
	newNode->setKeyNum(MINNUM_KEY);
	int i;
	for (i = 0; i < MINNUM_KEY; ++i) // 拷贝关键字的值
	{
		newNode->setKeyValue(i, this->m_KeyValues[i + MINNUM_CHILD]);
	}
	for (i = 0; i < MINNUM_CHILD; ++i) // 拷贝孩子节点指针
	{
		newNode->setChild(i, m_Childs[i + MINNUM_CHILD]);
	}
	this->setKeyNum(MINNUM_KEY); //更新左子树的关键字个数
	((CInternalNode<KeyType, DataType> *)parentNode)->insert(childIndex, childIndex + 1, this->m_KeyValues[MINNUM_KEY], newNode);
}

template <typename KeyType, typename DataType>
void CInternalNode<KeyType, DataType>::insert(int keyIndex, int childIndex, KeyType key, CNode<KeyType, DataType> *childNode)
{
	int i;
	for (i = this->getKeyNum(); i > keyIndex; --i) //将父节点中的childIndex后的所有关键字的值和子树指针向后移一位
	{
		setChild(i + 1, m_Childs[i]);
		this->setKeyValue(i, this->m_KeyValues[i - 1]);
	}
	if (i == childIndex)
	{
		setChild(i + 1, m_Childs[i]);
	}
	setChild(childIndex, childNode);
	this->setKeyValue(keyIndex, key);
	this->setKeyNum(this->m_KeyNum + 1);
}

template <typename KeyType, typename DataType>
void CInternalNode<KeyType, DataType>::mergeChild(CNode<KeyType, DataType> *parentNode, CNode<KeyType, DataType> *childNode, int keyIndex)
{
	// 合并数据
	insert(MINNUM_KEY, MINNUM_KEY + 1, parentNode->getKeyValue(keyIndex), ((CInternalNode<KeyType, DataType> *)childNode)->getChild(0));
	int i;
	for (i = 1; i <= childNode->getKeyNum(); ++i)
	{
		insert(MINNUM_KEY + i, MINNUM_KEY + i + 1, childNode->getKeyValue(i - 1), ((CInternalNode<KeyType, DataType> *)childNode)->getChild(i));
	}
	//父节点删除index的key
	parentNode->removeKey(keyIndex, keyIndex + 1);
	delete ((CInternalNode<KeyType, DataType> *)parentNode)->getChild(keyIndex + 1);
}

template <typename KeyType, typename DataType>
void CInternalNode<KeyType, DataType>::removeKey(int keyIndex, int childIndex)
{
	for (int i = 0; i < this->getKeyNum() - keyIndex - 1; ++i)
	{
		this->setKeyValue(keyIndex + i, this->getKeyValue(keyIndex + i + 1));
		setChild(childIndex + i, getChild(childIndex + i + 1));
	}
	this->setKeyNum(this->getKeyNum() - 1);
}

template <typename KeyType, typename DataType>
void CInternalNode<KeyType, DataType>::borrowFrom(CNode<KeyType, DataType> *siblingNode, CNode<KeyType, DataType> *parentNode, int keyIndex, SIBLING_DIRECTION d)
{
	switch (d)
	{
	case LEFT: // 从左兄弟结点借
	{
		insert(0, 0, parentNode->getKeyValue(keyIndex), ((CInternalNode<KeyType, DataType> *)siblingNode)->getChild(siblingNode->getKeyNum()));
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(siblingNode->getKeyNum() - 1));
		siblingNode->removeKey(siblingNode->getKeyNum() - 1, siblingNode->getKeyNum());
	}
	break;
	case RIGHT: // 从右兄弟结点借
	{
		insert(this->getKeyNum(), this->getKeyNum() + 1, parentNode->getKeyValue(keyIndex), ((CInternalNode<KeyType, DataType> *)siblingNode)->getChild(0));
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
		siblingNode->removeKey(0, 0);
	}
	break;
	default:
		break;
	}
}

template <typename KeyType, typename DataType>
int CInternalNode<KeyType, DataType>::getChildIndex(KeyType key, int keyIndex)
{
	if (key >= this->getKeyValue(keyIndex))
	{
		return keyIndex + 1;
	}
	else
	{
		return keyIndex;
	}
}

// CLeafNode
template <typename KeyType, typename DataType>
CLeafNode<KeyType, DataType>::CLeafNode()
{
	this->setType(LEAF);
	setLeftSibling(NULL);
	setRightSibling(NULL);

	m_overflow_size = 0;
}

template <typename KeyType, typename DataType>
CLeafNode<KeyType, DataType>::~CLeafNode()
{
}

template <typename KeyType, typename DataType>
CLeafNode<KeyType, DataType> *CLeafNode<KeyType, DataType>::getLeftSibling() const { return m_LeftSibling; }

template <typename KeyType, typename DataType>
void CLeafNode<KeyType, DataType>::setLeftSibling(CLeafNode<KeyType, DataType> *node) { m_LeftSibling = node; }

template <typename KeyType, typename DataType>
CLeafNode<KeyType, DataType> *CLeafNode<KeyType, DataType>::getRightSibling() const { return m_RightSibling; }

template <typename KeyType, typename DataType>
void CLeafNode<KeyType, DataType>::setRightSibling(CLeafNode<KeyType, DataType> *node) { m_RightSibling = node; }

template <typename KeyType, typename DataType>
DataType CLeafNode<KeyType, DataType>::getData(int i)
{
	this->pour();
	return m_Data[i];
}

template <typename KeyType, typename DataType>
void CLeafNode<KeyType, DataType>::setData(int i, const DataType &data)
{
	m_Data[i] = data;
}

template <typename KeyType, typename DataType>
DataType *CLeafNode<KeyType, DataType>::getDataAddr(int i)
{
	this->pour();
	return &(m_Data[i]);
}

template <typename KeyType, typename DataType>
void CLeafNode<KeyType, DataType>::clear()
{
	for (int i = 0; i < this->m_KeyNum; ++i)
	{
		// if type of m_Data is pointer
		//delete m_Data[i];
		//m_Data[i] = NULL;
	}
}

template <typename KeyType, typename DataType>
void CLeafNode<KeyType, DataType>::mainInsert(KeyType key, const DataType data)
{
	// cout << "this way " << endl;
	int i;
	for (i = this->m_KeyNum; i >= 1 && this->m_KeyValues[i - 1] > key; --i)
	{
		this->setKeyValue(i, this->m_KeyValues[i - 1]);
		setData(i, m_Data[i - 1]);
	}
	this->setKeyValue(i, key);
	setData(i, data);
	this->setKeyNum(this->m_KeyNum + 1);
}

template <typename KeyType, typename DataType>
void CLeafNode<KeyType, DataType>::insert(KeyType key, const DataType data)
{
	// mainInsert(key, data);

	if (this->m_KeyNum + ORDER / 2 > MAXNUM_LEAF)
	{
		mainInsert(key, data);
	}
	else
	{
		overflowInsert(key, data);
	}
}

template <typename KeyType, typename DataType>
void CLeafNode<KeyType, DataType>::split(CNode<KeyType, DataType> *parentNode, int childIndex)
{
	this->pour();
	CLeafNode<KeyType, DataType> *newNode = new CLeafNode<KeyType, DataType>; //分裂后的右节点
	this->setKeyNum(MINNUM_LEAF);
	newNode->setKeyNum(MINNUM_LEAF + 1);
	newNode->setRightSibling(getRightSibling());
	setRightSibling(newNode);
	newNode->setLeftSibling(this);
	int i;
	for (i = 0; i < MINNUM_LEAF + 1; ++i) // 拷贝关键字的值
	{
		newNode->setKeyValue(i, this->m_KeyValues[i + MINNUM_LEAF]);
	}
	for (i = 0; i < MINNUM_LEAF + 1; ++i) // 拷贝数据
	{
		newNode->setData(i, m_Data[i + MINNUM_LEAF]);
	}
	((CInternalNode<KeyType, DataType> *)parentNode)->insert(childIndex, childIndex + 1, this->m_KeyValues[MINNUM_LEAF], newNode);
}

template <typename KeyType, typename DataType>
void CLeafNode<KeyType, DataType>::mergeChild(CNode<KeyType, DataType> *parentNode, CNode<KeyType, DataType> *childNode, int keyIndex)
{
	this->pour();
	// 合并数据
	for (int i = 0; i < childNode->getKeyNum(); ++i)
	{
		insert(childNode->getKeyValue(i), ((CLeafNode *)childNode)->getData(i));
	}
	setRightSibling(((CLeafNode *)childNode)->getRightSibling());
	//父节点删除index的key，
	parentNode->removeKey(keyIndex, keyIndex + 1);
}

template <typename KeyType, typename DataType>
void CLeafNode<KeyType, DataType>::removeKey(int keyIndex, int childIndex)
{
	this->pour();
	for (int i = keyIndex; i < this->getKeyNum() - 1; ++i)
	{
		this->setKeyValue(i, this->getKeyValue(i + 1));
		setData(i, getData(i + 1));
	}
	this->setKeyNum(this->getKeyNum() - 1);
}

template <typename KeyType, typename DataType>
void CLeafNode<KeyType, DataType>::borrowFrom(CNode<KeyType, DataType> *siblingNode, CNode<KeyType, DataType> *parentNode, int keyIndex, SIBLING_DIRECTION d)
{
	this->pour();
	switch (d)
	{
	case LEFT: // 从左兄弟结点借
	{
		insert(siblingNode->getKeyValue(siblingNode->getKeyNum() - 1), ((CLeafNode *)siblingNode)->getData(siblingNode->getKeyNum() - 1));
		siblingNode->removeKey(siblingNode->getKeyNum() - 1, siblingNode->getKeyNum() - 1);
		parentNode->setKeyValue(keyIndex, this->getKeyValue(0));
	}
	break;
	case RIGHT: // 从右兄弟结点借
	{
		insert(siblingNode->getKeyValue(0), ((CLeafNode *)siblingNode)->getData(0));
		siblingNode->removeKey(0, 0);
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
	}
	break;
	default:
		break;
	}
}

template <typename KeyType, typename DataType>
int CLeafNode<KeyType, DataType>::getChildIndex(KeyType key, int keyIndex)
{
	this->pour();
	return keyIndex;
}

template <typename KeyType, typename DataType>
void CLeafNode<KeyType, DataType>::pour()
{
	if (m_overflow_size == 0)
		return;
	// cout << "overflow size is: " << m_overflow_size << endl;
	sortOverflow();
	// cout << "again, overflow size is: " << m_overflow_size << endl;
	for (int k = 0; k < m_overflow_size; k++)
	{
		KeyType key = m_keys_overflow[k];
		DataType data = m_data_overflow[k];

		mainInsert(key, data);
	}
	m_overflow_size = 0;
}

template <typename KeyType, typename DataType>
void CLeafNode<KeyType, DataType>::sortOverflow()
{
	int end_of_sorted = 0;

	while (end_of_sorted < m_overflow_size - 1)
	{
		int start = 0;

		while (m_keys_overflow[start] < m_keys_overflow[end_of_sorted + 1])
		{
			start++;
		}

		int temp = m_keys_overflow[end_of_sorted + 1];

		for (int i = end_of_sorted; i >= start; i--)
		{
			m_keys_overflow[i + 1] = m_keys_overflow[i];
			m_data_overflow[i + 1] = m_data_overflow[i];
		}

		m_keys_overflow[start] = temp;
		end_of_sorted++;
	}
}

template <typename KeyType, typename DataType>
void CLeafNode<KeyType, DataType>::overflowInsert(KeyType key, const DataType data)
{
	if (m_overflow_size >= ORDER / 2)
		pour();
	m_keys_overflow[m_overflow_size] = key;
	m_data_overflow[m_overflow_size] = data;
	m_overflow_size++;
}
