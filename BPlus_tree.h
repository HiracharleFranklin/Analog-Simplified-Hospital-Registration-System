#ifndef BPLUS_TREE_H
#define BPLUS_TREE_H

#include "BPlus_node.cpp"
#include <vector>
using namespace std;

enum COMPARE_OPERATOR
{
	LT,
	LE,
	EQ,
	BE,
	BT,
	BETWEEN
}; // 比较操作符：<、<=、=、>=、>、<>
const int INVALID_INDEX = -1;

template <typename KeyType, typename DataType>
struct SelectResult
{
	int keyIndex;
	CLeafNode<KeyType, DataType> *targetNode;
};

template <typename KeyType, typename DataType>
class CBPlusTree
{
public:
	// 构造函数 & 析构函数
	CBPlusTree();
	~CBPlusTree();

	// 接口
	bool insert(KeyType key, const DataType data);
	bool remove(KeyType key);
	bool update(KeyType oldKey, KeyType newKey);

	// 定值查询，compareOperator可以是LT(<)、LE(<=)、EQ(=)、BE(>=)、BT(>)
	vector<DataType> oneSideSelect(KeyType compareKey, int compareOpeartor) const;

	// 范围查询，BETWEEN
	vector<DataType> twoSideSelect(KeyType smallKey, KeyType largeKey) const;

	// 查找是否存在
	bool search(KeyType key) const;

	// 获取key对应data的指针
	DataType *getDataHandle(KeyType key) const;

	// 清空
	void clear();

	// 打印所有key（树的形式）
	void print() const;

	// 打印所有data
	void printData() const;

private:
	void recursive_insert(CNode<KeyType, DataType> *parentNode, KeyType key, const DataType data);
	void recursive_remove(CNode<KeyType, DataType> *parentNode, KeyType key);
	void printInConcavo(CNode<KeyType, DataType> *pNode, int count) const;
	bool recursive_search(CNode<KeyType, DataType> *pNode, KeyType key) const;
	void recursive_search(CNode<KeyType, DataType> *pNode, KeyType key, SelectResult<KeyType, DataType> &result);
	DataType *recursive_getDataHandle(CNode<KeyType, DataType> *pNode, KeyType key) const;
	void changeKey(CNode<KeyType, DataType> *pNode, KeyType oldKey, KeyType newKey);
	void search(KeyType key, SelectResult<KeyType, DataType> &result);
	void remove(KeyType key, DataType &dataValue);
	void recursive_remove(CNode<KeyType, DataType> *parentNode, KeyType key, DataType &dataValue);

private:
	CNode<KeyType, DataType> *m_Root;
	CLeafNode<KeyType, DataType> *m_DataHead;
	KeyType m_MaxKey; // B+树中的最大键
};

#endif
