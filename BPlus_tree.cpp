#pragma once
#include "BPlus_tree.h"
#include <iostream>
#include <algorithm>
using namespace std;

template <typename KeyType, typename DataType>
CBPlusTree<KeyType, DataType>::CBPlusTree()
{
	m_Root = NULL;
	m_DataHead = NULL;
}

template <typename KeyType, typename DataType>
CBPlusTree<KeyType, DataType>::~CBPlusTree()
{
	clear();
}

template <typename KeyType, typename DataType>
bool CBPlusTree<KeyType, DataType>::insert(KeyType key, const DataType data)
{
	// cout << "check0" << endl;
	// 是否已经存在
	if (search(key))
	{
		// cout << "check1" << endl;
		return false;
	}
	// 找到可以插入的叶子结点，否则创建新的叶子结点
	if (m_Root == NULL)
	{
		// cout << "check2" << endl;
		m_Root = new CLeafNode<KeyType, DataType>;
		m_DataHead = (CLeafNode<KeyType, DataType> *)m_Root;
		m_MaxKey = key;
	}
	if (m_Root->getKeyNum() >= MAXNUM_KEY) // 根结点已满，分裂
	{
		// cout << "check3" << endl;
		CInternalNode<KeyType, DataType> *newNode = new CInternalNode<KeyType, DataType>; //创建新的根节点
		newNode->setChild(0, m_Root);
		m_Root->split(newNode, 0); // 叶子结点分裂
		m_Root = newNode;		   //更新根节点指针
	}
	if (key > m_MaxKey) // 更新最大键值
	{
		// cout << "check4" << endl;
		m_MaxKey = key;
	}
	recursive_insert(m_Root, key, data);
	return true;
}

template <typename KeyType, typename DataType>
void CBPlusTree<KeyType, DataType>::recursive_insert(CNode<KeyType, DataType> *parentNode, KeyType key, const DataType data)
{
	if (parentNode->getType() == LEAF) // 叶子结点，直接插入
	{
		// cout << "here" << endl;
		((CLeafNode<KeyType, DataType> *)parentNode)->insert(key, data);
	}
	else
	{
		// 找到子结点
		int keyIndex = parentNode->getKeyIndex(key);
		int childIndex = parentNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
		CNode<KeyType, DataType> *childNode = ((CInternalNode<KeyType, DataType> *)parentNode)->getChild(childIndex);
		if (childNode->getKeyNum() >= MAXNUM_LEAF) // 子结点已满，需进行分裂
		{
			childNode->split(parentNode, childIndex);
			if (parentNode->getKeyValue(childIndex) <= key) // 确定目标子结点
			{
				childNode = ((CInternalNode<KeyType, DataType> *)parentNode)->getChild(childIndex + 1);
			}
		}
		recursive_insert(childNode, key, data);
	}
}

template <typename KeyType, typename DataType>
void CBPlusTree<KeyType, DataType>::clear()
{
	if (m_Root != NULL)
	{
		m_Root->clear();
		delete m_Root;
		m_Root = NULL;
		m_DataHead = NULL;
	}
}

template <typename KeyType, typename DataType>
bool CBPlusTree<KeyType, DataType>::search(KeyType key) const
{
	return recursive_search(m_Root, key);
}

template <typename KeyType, typename DataType>
bool CBPlusTree<KeyType, DataType>::recursive_search(CNode<KeyType, DataType> *pNode, KeyType key) const
{
	if (pNode == NULL) //检测节点指针是否为空，或该节点是否为叶子节点
	{
		return false;
	}
	else
	{
		int keyIndex = pNode->getKeyIndex(key);
		int childIndex = pNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
		if (keyIndex < pNode->getKeyNum() && key == pNode->getKeyValue(keyIndex))
		{
			return true;
		}
		else
		{
			if (pNode->getType() == LEAF) //检查该节点是否为叶子节点
			{
				return false;
			}
			else
			{
				return recursive_search(((CInternalNode<KeyType, DataType> *)pNode)->getChild(childIndex), key);
			}
		}
	}
}

template <typename KeyType, typename DataType>
DataType *CBPlusTree<KeyType, DataType>::getDataHandle(KeyType key) const
{
	return recursive_getDataHandle(m_Root, key);
}

template <typename KeyType, typename DataType>
DataType *CBPlusTree<KeyType, DataType>::recursive_getDataHandle(CNode<KeyType, DataType> *pNode, KeyType key) const
{
	// cout << "first" << endl;
	if (pNode == NULL) //检测节点指针是否为空，或该节点是否为叶子节点
	{
		return nullptr;
	}

	int keyIndex = pNode->getKeyIndex(key);
	int childIndex = pNode->getChildIndex(key, keyIndex); // 孩子结点指针索引

	// cout << "second" << endl;
	if (pNode->getType() == LEAF)
	{
		// ((CLeafNode<KeyType, DataType> *)pNode)->pour();

		// cout << "now \"pnode\" points to a leaf." << endl;
		// cout << "key to get handle: " << key << endl;
		// cout << "keyindex: " << keyIndex << endl;
		// cout << "childindex: " << childIndex << endl;
		// cout << "pNode->getKeyNum(): " << pNode->getKeyNum() << endl;
		// cout << "pNode->getKeyValue(keyIndex): " << pNode->getKeyValue(keyIndex) << endl;

		if (keyIndex < pNode->getKeyNum() && key == pNode->getKeyValue(keyIndex))
		{
			// cout << "third" << endl;
			return ((CLeafNode<KeyType, DataType> *)pNode)->getDataAddr(childIndex);
		}
		else
		{
			// cout << "fourth" << endl;
			return nullptr;
		}
	}
	else
		// cout << "\"pnode\" still points to an internal node." << endl;

	return recursive_getDataHandle(((CInternalNode<KeyType, DataType> *)pNode)->getChild(childIndex), key);
}

template <typename KeyType, typename DataType>
void CBPlusTree<KeyType, DataType>::print() const
{
	cout << endl;
	cout << endl;
	cout << "Now print the keys in tree form:" << endl
		 << endl;
	printInConcavo(m_Root, 10);
	cout << endl;
}

template <typename KeyType, typename DataType>
void CBPlusTree<KeyType, DataType>::printInConcavo(CNode<KeyType, DataType> *pNode, int count) const
{
	if (pNode->getType() == LEAF)
		((CLeafNode<KeyType, DataType> *)pNode)->pour();
	if (pNode != NULL)
	{
		int i, j;
		for (i = 0; i < pNode->getKeyNum(); ++i)
		{
			if (pNode->getType() != LEAF)
			{
				printInConcavo(((CInternalNode<KeyType, DataType> *)pNode)->getChild(i), count - 2);
			}
			for (j = count; j >= 0; --j)
			{
				cout << "---";
			}
			cout << pNode->getKeyValue(i) << endl;
		}
		if (pNode->getType() != LEAF)
		{
			printInConcavo(((CInternalNode<KeyType, DataType> *)pNode)->getChild(i), count - 2);
		}
	}
}

template <typename KeyType, typename DataType>
void CBPlusTree<KeyType, DataType>::printData() const
{
	cout << endl;
	cout << "Now print all the data stored in B+ tree:" << endl
		 << endl;
	CLeafNode<KeyType, DataType> *itr = m_DataHead;
	itr->pour();
	while (itr != NULL)
	{
		cout << "block size: " << itr->getKeyNum() << endl;
		cout << "[ ";
		for (int i = 0; i < itr->getKeyNum(); ++i)
		{
			cout << itr->getData(i) << " , ";
		}
		cout << " ]" << endl;
		itr = itr->getRightSibling();
	}
	cout << endl;
}

template <typename KeyType, typename DataType>
bool CBPlusTree<KeyType, DataType>::remove(KeyType key)
{
	if (!search(key)) //不存在
	{
		return false;
	}
	if (m_Root->getKeyNum() == 1) //特殊情况处理
	{
		if (m_Root->getType() == LEAF)
		{
			clear();
			return true;
		}
		else
		{
			CNode<KeyType, DataType> *pChild1 = ((CInternalNode<KeyType, DataType> *)m_Root)->getChild(0);
			CNode<KeyType, DataType> *pChild2 = ((CInternalNode<KeyType, DataType> *)m_Root)->getChild(1);
			if (pChild1->getKeyNum() == MINNUM_KEY && pChild2->getKeyNum() == MINNUM_KEY)
			{
				pChild1->mergeChild(m_Root, pChild2, 0);
				delete m_Root;
				m_Root = pChild1;
			}
		}
	}
	recursive_remove(m_Root, key);
	return true;
}

// parentNode中包含的键值数>MINNUM_KEY
template <typename KeyType, typename DataType>
void CBPlusTree<KeyType, DataType>::recursive_remove(CNode<KeyType, DataType> *parentNode, KeyType key)
{
	int keyIndex = parentNode->getKeyIndex(key);
	int childIndex = parentNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
	if (parentNode->getType() == LEAF)						   // 找到目标叶子节点
	{
		if (key == m_MaxKey && keyIndex > 0)
		{
			m_MaxKey = parentNode->getKeyValue(keyIndex - 1);
		}
		parentNode->removeKey(keyIndex, childIndex); // 直接删除
		// 如果键值在内部结点中存在，也要相应的替换内部结点
		if (childIndex == 0 && m_Root->getType() != LEAF && parentNode != m_DataHead)
		{
			changeKey(m_Root, key, parentNode->getKeyValue(0));
		}
	}
	else // 内结点
	{
		cout << "call" << endl;
		CNode<KeyType, DataType> *pChildNode = ((CInternalNode<KeyType, DataType> *)parentNode)->getChild(childIndex); //包含key的子树根节点
		if (pChildNode->getKeyNum() == MINNUM_KEY)																	   // 包含关键字达到下限值，进行相关操作
		{
			CNode<KeyType, DataType> *pLeft = childIndex > 0 ? ((CInternalNode<KeyType, DataType> *)parentNode)->getChild(childIndex - 1) : NULL;						 //左兄弟节点
			CNode<KeyType, DataType> *pRight = childIndex < parentNode->getKeyNum() ? ((CInternalNode<KeyType, DataType> *)parentNode)->getChild(childIndex + 1) : NULL; //右兄弟节点
			// 先考虑从兄弟结点中借
			if (pLeft && pLeft->getKeyNum() > MINNUM_KEY) // 左兄弟结点可借
			{
				pChildNode->borrowFrom(pLeft, parentNode, childIndex - 1, LEFT);
			}
			else if (pRight && pRight->getKeyNum() > MINNUM_KEY) //右兄弟结点可借
			{
				pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
			}
			//左右兄弟节点都不可借，考虑合并
			else if (pLeft) //与左兄弟合并
			{
				pLeft->mergeChild(parentNode, pChildNode, childIndex - 1);
				pChildNode = pLeft;
			}
			else if (pRight) //与右兄弟合并
			{
				pChildNode->mergeChild(parentNode, pRight, childIndex);
			}
		}
		recursive_remove(pChildNode, key);
	}
}

template <typename KeyType, typename DataType>
void CBPlusTree<KeyType, DataType>::changeKey(CNode<KeyType, DataType> *pNode, KeyType oldKey, KeyType newKey)
{
	if (pNode != NULL && pNode->getType() != LEAF)
	{
		int keyIndex = pNode->getKeyIndex(oldKey);
		if (keyIndex < pNode->getKeyNum() && oldKey == pNode->getKeyValue(keyIndex)) // 找到
		{
			pNode->setKeyValue(keyIndex, newKey);
		}
		else // 继续找
		{
			changeKey(((CInternalNode<KeyType, DataType> *)pNode)->getChild(keyIndex), oldKey, newKey);
		}
	}
}

template <typename KeyType, typename DataType>
bool CBPlusTree<KeyType, DataType>::update(KeyType oldKey, KeyType newKey)
{
	if (search(newKey)) // 检查更新后的键是否已经存在
	{
		return false;
	}
	else
	{
		int dataValue;
		remove(oldKey, dataValue);
		if (dataValue == INVALID_INDEX)
		{
			return false;
		}
		else
		{
			return insert(newKey, dataValue);
		}
	}
}

template <typename KeyType, typename DataType>
void CBPlusTree<KeyType, DataType>::remove(KeyType key, DataType &dataValue)
{
	if (!search(key)) //不存在
	{
		dataValue = INVALID_INDEX;
		return;
	}
	if (m_Root->getKeyNum() == 1) //特殊情况处理
	{
		if (m_Root->getType() == LEAF)
		{
			dataValue = ((CLeafNode<KeyType, DataType> *)m_Root)->getData(0);
			clear();
			return;
		}
		else
		{
			CNode<KeyType, DataType> *pChild1 = ((CInternalNode<KeyType, DataType> *)m_Root)->getChild(0);
			CNode<KeyType, DataType> *pChild2 = ((CInternalNode<KeyType, DataType> *)m_Root)->getChild(1);
			if (pChild1->getKeyNum() == MINNUM_KEY && pChild2->getKeyNum() == MINNUM_KEY)
			{
				pChild1->mergeChild(m_Root, pChild2, 0);
				delete m_Root;
				m_Root = pChild1;
			}
		}
	}
	recursive_remove(m_Root, key, dataValue);
}

template <typename KeyType, typename DataType>
void CBPlusTree<KeyType, DataType>::recursive_remove(CNode<KeyType, DataType> *parentNode, KeyType key, DataType &dataValue)
{
	int keyIndex = parentNode->getKeyIndex(key);
	int childIndex = parentNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
	if (parentNode->getType() == LEAF)						   // 找到目标叶子节点
	{
		if (key == m_MaxKey && keyIndex > 0)
		{
			m_MaxKey = parentNode->getKeyValue(keyIndex - 1);
		}
		dataValue = ((CLeafNode<KeyType, DataType> *)parentNode)->getData(keyIndex);
		parentNode->removeKey(keyIndex, childIndex); // 直接删除
		// 如果键值在内部结点中存在，也要相应的替换内部结点
		if (childIndex == 0 && m_Root->getType() != LEAF && parentNode != m_DataHead)
		{
			changeKey(m_Root, key, parentNode->getKeyValue(0));
		}
	}
	else // 内结点
	{
		CNode<KeyType, DataType> *pChildNode = ((CInternalNode<KeyType, DataType> *)parentNode)->getChild(childIndex); //包含key的子树根节点
		if (pChildNode->getKeyNum() == MINNUM_KEY)																	   // 包含关键字达到下限值，进行相关操作
		{
			CNode<KeyType, DataType> *pLeft = childIndex > 0 ? ((CInternalNode<KeyType, DataType> *)parentNode)->getChild(childIndex - 1) : NULL;						 //左兄弟节点
			CNode<KeyType, DataType> *pRight = childIndex < parentNode->getKeyNum() ? ((CInternalNode<KeyType, DataType> *)parentNode)->getChild(childIndex + 1) : NULL; //右兄弟节点
			// 先考虑从兄弟结点中借
			if (pLeft && pLeft->getKeyNum() > MINNUM_KEY) // 左兄弟结点可借
			{
				pChildNode->borrowFrom(pLeft, parentNode, childIndex - 1, LEFT);
			}
			else if (pRight && pRight->getKeyNum() > MINNUM_KEY) //右兄弟结点可借
			{
				pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
			}
			//左右兄弟节点都不可借，考虑合并
			else if (pLeft) //与左兄弟合并
			{
				pLeft->mergeChild(parentNode, pChildNode, childIndex - 1);
				pChildNode = pLeft;
			}
			else if (pRight) //与右兄弟合并
			{
				pChildNode->mergeChild(parentNode, pRight, childIndex);
			}
		}
		recursive_remove(pChildNode, key, dataValue);
	}
}

template <typename KeyType, typename DataType>
vector<DataType> CBPlusTree<KeyType, DataType>::oneSideSelect(KeyType compareKey, int compareOpeartor) const
{
	vector<DataType> results;
	if (m_Root != NULL)
	{
		if (compareKey > m_MaxKey) // 比较键值大于B+树中最大的键值
		{
			if (compareOpeartor == LE || compareOpeartor == LT)
			{
				for (CLeafNode<KeyType, DataType> *itr = m_DataHead; itr != NULL; itr = itr->getRightSibling())
				{
					for (int i = 0; i < itr->getKeyNum(); ++i)
					{
						results.push_back(itr->getData(i));
					}
				}
			}
		}
		else if (compareKey < m_DataHead->getKeyValue(0)) // 比较键值小于B+树中最小的键值
		{
			if (compareOpeartor == BE || compareOpeartor == BT)
			{
				for (CLeafNode<KeyType, DataType> *itr = m_DataHead; itr != NULL; itr = itr->getRightSibling())
				{
					for (int i = 0; i < itr->getKeyNum(); ++i)
					{
						results.push_back(itr->getData(i));
					}
				}
			}
		}
		else // 比较键值在B+树中
		{
			SelectResult<KeyType, DataType> result;
			search(compareKey, result);
			switch (compareOpeartor)
			{
			case LT:
			case LE:
			{
				CLeafNode<KeyType, DataType> *itr = m_DataHead;
				int i;
				while (itr != result.targetNode)
				{
					for (i = 0; i < itr->getKeyNum(); ++i)
					{
						results.push_back(itr->getData(i));
					}
					itr = itr->getRightSibling();
				}
				for (i = 0; i < result.keyIndex; ++i)
				{
					results.push_back(itr->getData(i));
				}
				if (itr->getKeyValue(i) < compareKey ||
					(compareOpeartor == LE && compareKey == itr->getKeyValue(i)))
				{
					results.push_back(itr->getData(i));
				}
			}
			break;
			case EQ:
			{
				if (result.targetNode->getKeyValue(result.keyIndex) == compareKey)
				{
					results.push_back(result.targetNode->getData(result.keyIndex));
				}
			}
			break;
			case BE:
			case BT:
			{
				CLeafNode<KeyType, DataType> *itr = result.targetNode;
				if (compareKey < itr->getKeyValue(result.keyIndex) ||
					(compareOpeartor == BE && compareKey == itr->getKeyValue(result.keyIndex)))
				{
					results.push_back(itr->getData(result.keyIndex));
				}
				int i;
				for (i = result.keyIndex + 1; i < itr->getKeyNum(); ++i)
				{
					results.push_back(itr->getData(i));
				}
				itr = itr->getRightSibling();
				while (itr != NULL)
				{
					for (i = 0; i < itr->getKeyNum(); ++i)
					{
						results.push_back(itr->getData(i));
					}
					itr = itr->getRightSibling();
				}
			}
			break;
			default: // 范围查询
				break;
			}
		}
	}
	sort<vector<DataType>::iterator>(results.begin(), results.end());
	return results;
}

template <typename KeyType, typename DataType>
vector<DataType> CBPlusTree<KeyType, DataType>::twoSideSelect(KeyType smallKey, KeyType largeKey) const
{
	vector<DataType> results;
	if (smallKey <= largeKey)
	{
		SelectResult<KeyType, DataType> start, end;
		search(smallKey, start);
		search(largeKey, end);
		CLeafNode<KeyType, DataType> *itr = start.targetNode;
		int i = start.keyIndex;
		if (itr->getKeyValue(i) < smallKey)
		{
			++i;
		}
		if (end.targetNode->getKeyValue(end.keyIndex) > largeKey)
		{
			--end.keyIndex;
		}
		while (itr != end.targetNode)
		{
			for (; i < itr->getKeyNum(); ++i)
			{
				results.push_back(itr->getData(i));
			}
			itr = itr->getRightSibling();
			i = 0;
		}
		for (; i <= end.keyIndex; ++i)
		{
			results.push_back(itr->getData(i));
		}
	}
	sort<vector<DataType>::iterator>(results.begin(), results.end());
	return results;
}

template <typename KeyType, typename DataType>
void CBPlusTree<KeyType, DataType>::search(KeyType key, SelectResult<KeyType, DataType> &result)
{
	recursive_search(m_Root, key, result);
}

template <typename KeyType, typename DataType>
void CBPlusTree<KeyType, DataType>::recursive_search(CNode<KeyType, DataType> *pNode, KeyType key, SelectResult<KeyType, DataType> &result)
{
	// cout << "check0" << endl;
	int keyIndex = pNode->getKeyIndex(key);
	int childIndex = pNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
	if (pNode->getType() == LEAF)
	{
		result.keyIndex = keyIndex;
		result.targetNode = (CLeafNode<KeyType, DataType> *)pNode;
		return;
	}
	else
	{
		recursive_search(((CInternalNode<KeyType, DataType> *)pNode)->getChild(childIndex), key, result);
	}
}