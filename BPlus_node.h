#ifndef BPLUS_NODE
#define BPLUS_NODE

enum NODE_TYPE
{
	INTERNAL,
	LEAF
}; // 结点类型：内结点、叶子结点
enum SIBLING_DIRECTION
{
	LEFT,
	RIGHT
}; // 兄弟结点方向：左兄弟结点、右兄弟结点

const int ORDER = 5;					 	// B+树的阶（非根内结点的最小子树个数）
const int MINNUM_KEY = ORDER - 1;		 	// 最小键值个数
const int MAXNUM_KEY = 2 * ORDER - 1;	 	// 最大键值个数
const int MINNUM_CHILD = MINNUM_KEY + 1; 	// 最小子树个数
const int MAXNUM_CHILD = MAXNUM_KEY + 1; 	// 最大子树个数
const int MINNUM_LEAF = MINNUM_KEY;		 	// 最小叶子结点键值个数
const int MAXNUM_LEAF = MAXNUM_KEY;		 	// 最大叶子结点键值个数

// 结点基类
template <typename KeyType, typename DataType>
class CNode
{
public:
	CNode();
	virtual ~CNode();

	NODE_TYPE getType() const;
	void setType(NODE_TYPE type);
	int getKeyNum() const;
	void setKeyNum(int n);
	KeyType getKeyValue(int i) const;
	void setKeyValue(int i, KeyType key);

	int getKeyIndex(KeyType key) const; // 找到键值在结点中存储的下标
	// 纯虚函数，定义接口
	virtual void removeKey(int keyIndex, int childIndex) = 0;																				  // 从结点中移除键值
	// 分裂父亲的哪个孩子
	virtual void split(CNode *parentNode, int childIndex) = 0;																				  // 分裂结点
	virtual void mergeChild(CNode<KeyType, DataType> *parentNode, CNode<KeyType, DataType> *childNode, int keyIndex) = 0;					  // 合并结点
	virtual void clear() = 0;																												  // 清空结点，同时会清空结点所包含的子树结点
	virtual void borrowFrom(CNode<KeyType, DataType> *destNode, CNode<KeyType, DataType> *parentNode, int keyIndex, SIBLING_DIRECTION d) = 0; // 从兄弟结点中借一个键值
	virtual int getChildIndex(KeyType key, int keyIndex) = 0;																			  // 根据键值获取孩子结点指针下标

protected:
	NODE_TYPE m_Type;
	int m_KeyNum;
	KeyType m_KeyValues[MAXNUM_KEY];
};

// 内结点
template <typename KeyType, typename DataType>
class CInternalNode : public CNode<KeyType, DataType>
{
public:
	CInternalNode();
	virtual ~CInternalNode();

	CNode<KeyType, DataType> *getChild(int i) const;
	void setChild(int i, CNode<KeyType, DataType> *child);

	void insert(int keyIndex, int childIndex, KeyType key, CNode<KeyType, DataType> *childNode);
	virtual void split(CNode<KeyType, DataType> *parentNode, int childIndex);
	virtual void mergeChild(CNode<KeyType, DataType> *parentNode, CNode<KeyType, DataType> *childNode, int keyIndex);
	virtual void removeKey(int keyIndex, int childIndex);
	virtual void clear();
	virtual void borrowFrom(CNode<KeyType, DataType> *destNode, CNode<KeyType, DataType> *parentNode, int keyIndex, SIBLING_DIRECTION d);
	virtual int getChildIndex(KeyType key, int keyIndex);

private:
	CNode<KeyType, DataType> *m_Childs[MAXNUM_CHILD];
};

// 叶子结点
template <typename KeyType, typename DataType>
class CLeafNode : public CNode<KeyType, DataType>
{
	// 修改日志 TODO
	// #1. 写pour：从overflow倒进main里，要有序！！！
	// #2. 改insert：如果main已填充的个数 + overflow的容量 > main的容量，则插入到main里，否则插入到overflow里
	// #3. 改getData：get任何数据前，先执行pour操作
	// #4. 改setData：set任何数据前，先执行pour操作
	// #5. 改getDataAddr：get任何数据的address前，先执行pour操作
	// #6. 改split：split前，先执行pour操作
	// #7. 改mergeChild：merge前，先执行pour操作
	// #8. 改removeKey：remove任何数据前，先执行pour操作
	//// #9. 改clear：clear前，先执行pour操作
	// #10.改borrowFrom：borrow前，先执行pour操作
	// #11.改getChildIndex：get前，先执行pour操作
	// #12.改构造函数，初始overflow size设为0
	// #13.写sortOverflow：把overflow数组里的key和data同时排序
	// 14.改print，打印前先pour

public:
	CLeafNode();
	virtual ~CLeafNode();

	CLeafNode<KeyType, DataType> *getLeftSibling() const;
	void setLeftSibling(CLeafNode<KeyType, DataType> *node);
	CLeafNode<KeyType, DataType> *getRightSibling() const;
	void setRightSibling(CLeafNode<KeyType, DataType> *node);
	DataType getData(int i);
	void setData(int i, const DataType &data);

	DataType *getDataAddr(int i);

	void mainInsert(KeyType key, const DataType data);
	void insert(KeyType key, const DataType data);
	virtual void split(CNode<KeyType, DataType> *parentNode, int childIndex);
	virtual void mergeChild(CNode<KeyType, DataType> *parentNode, CNode<KeyType, DataType> *childNode, int keyIndex);
	virtual void removeKey(int keyIndex, int childIndex);
	virtual void clear();
	virtual void borrowFrom(CNode<KeyType, DataType> *destNode, CNode<KeyType, DataType> *parentNode, int keyIndex, SIBLING_DIRECTION d);
	virtual int getChildIndex(KeyType key, int keyIndex);

	// 和Overflow Block有关的操作
	void pour();
	void sortOverflow();
	void overflowInsert(KeyType key, const DataType data);

private:
	CLeafNode<KeyType, DataType> *m_LeftSibling;
	CLeafNode<KeyType, DataType> *m_RightSibling;
	DataType m_Data[MAXNUM_LEAF];

	// 新加的Overflow Block
	KeyType m_keys_overflow[ORDER/2];
	DataType m_data_overflow[ORDER/2];
	int m_overflow_size;
};

#endif
