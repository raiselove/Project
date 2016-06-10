#ifndef __HaffmanTree_H_
#define __HaffmanTree_H_
#include "Heap.h"
template<class T>				
struct  HaffmanTreeNode
{
	typedef HaffmanTreeNode<T> Node;

	T _weight;
	Node* _left;
	Node* _right;

	HaffmanTreeNode(const T& weight)
		:_weight(weight)
		, _left(NULL)
		, _right(NULL)
	{}
};

template<class T>
class HaffmanTree
{
	typedef HaffmanTreeNode<T> Node;
public:
	HaffmanTree()
		:_root(NULL)
	{}
	HaffmanTree(const T* arr, size_t size)
	{
		_root = _Create(arr, size);
	}
	//构造函数
	HaffmanTree(const T* arr, size_t size, const T& invalid)
	{ 
		_root = _Create(arr, size, invalid);
	}

	~HaffmanTree()
	{
		if (_root)
		{
			_Destroy(_root);
		}
	}

	Node* GetRoot()
	{
		return _root;
	}

protected:
	//创建哈夫曼树
	Node* _Create(const T* arr, size_t size, const T& invalid)
	{ 
		struct compare
		{
			bool operator ()(const Node *left, const Node *right)
			{
				return left->_weight > right->_weight;
			}
		};

		Heap<Node*,compare>  _heap;
		//把值放到vector中
		for (size_t i = 0; i < size; ++i)
		{
			if (arr[i] != invalid)
			{
				Node *tmp = new Node(arr[i]);
				_heap.Push(tmp);
			}
		}
		//构造哈夫曼树
		while (_heap.Size() > 1)
		{
			Node *left = _heap.Top();
			_heap.Pop();
			Node*right = _heap.Top();
			_heap.Pop();

			Node *parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;

			_heap.Push(parent);
		}

		return _heap.Top();
	}
	//释放节点
	void _Destroy(Node* root)
	{ 
		if (root->_left == NULL && root->_right == NULL)
		{
			delete root;
			root = NULL;
		}
		else
		{
			_Destroy(root->_left);
			_Destroy(root->_right);
		}
	}

private:
	Node *_root;
};
#endif //__HaffmanTree_H_
