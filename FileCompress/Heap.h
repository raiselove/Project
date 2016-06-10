#ifndef __Heap_H_
#define __Heap_H_

#include<vector>
#include<assert.h>
template<class T>
//比较器
class Less
{
public:
	bool operator() (const T& left, const T& right)
	{
		return left > right;
	}
};

template<class T>
class Greater
{
public:
	bool operator() (const T& left, const T& right)
	{
		return left < right;
	}
};

template<class T, class Compare = Less<T> >
class Heap
{
public:
	//构造函数
	Heap()
		:_arr(NULL)
	{}
	Heap(const T* arr, int size)
	{
		assert(arr);
		_arr.reserve(size);

		for (int i = 0; i < size; i++)
		{
			_arr.push_back(arr[i]);
		}

		int begin = _arr.size() / 2 - 1;
		while (begin >= 0)
		{
			_AdjustDown(begin);
			begin--;         
		}
	}
	//拷贝构造
	Heap(vector<T>& s)
	{
		_arr = s;
		int begin = _arr.size() / 2 - 1;
		while (begin >= 0)
		{
			_AdjustDown(begin);
			begin--;            
		}
	}
	void Push(const T& x)
	{
		_arr.push_back(x);
		int begin = _arr.size();
		_AdjustUp(begin);
	}

	void Pop() 
	{
		_arr[0] = _arr[_arr.size() - 1];
		_arr.pop_back();
		_AdjustDown(0);
	}

	void Clear()
	{
		_arr.clear();
	}

	bool Empty()
	{
		return _arr.empty();
	}

  T& Top() 
	{
		if (!Empty())
		{
			return _arr[0];
		}
		exit(1);
	}

	size_t Size()
	{
		return _arr.size();
	}

protected:
	//下调
	void _AdjustDown(int parent)
	{ // 从根节点向下调整
		int size = _arr.size();
		int left = parent * 2 + 1;
		int right = left + 1;
		int key = left;
		while (left < size)
		{//Compare()仿函数
			//if (right < size && array[left] > array[right])
			if (right < size && Compare()(_arr[left], _arr[right]))  //右边小
			{
				key = right;
			}
			else
			{
				key = left;
			}
			//if ((array[key] > array[parent]))
			if (Compare()(_arr[key], _arr[parent]))
			{
				break;
			}
			//else if (array[parent] > array[key])
			else if (Compare()(_arr[parent], _arr[key])) //左边小
			{
				swap(_arr[key], _arr[parent]);
			}

			parent = key;
			left = parent * 2 + 1;
			right = left + 1;
		}
	}
	//上调
	void _AdjustUp(int child)
	{  //从叶子节点或子节点向上调整
		int size = _arr.size();
		if (size == 0 || size == 1)
		{
			return;
		}

		int parent = (child - 2) / 2;
		int left = parent * 2 + 1;
		int right = left + 1;
		int key = 0;
		while (parent >= 0)
		{
			//if (right < size && array[left] > array[right])
			if (right < size && Compare()(_arr[left], _arr[right]))
			{
				key = right;
			}
			else
			{
				key = left;
			}
			//if (array[parent] > array[key])
			if (Compare()(_arr[parent], _arr[key]))
			{
				swap(_arr[parent], _arr[key]);
			}
			else
			{
				break;
			}

			if (parent == 0)
			{
				break;
			}

			parent = (parent - 1) / 2;
			left = parent * 2 + 1;
			right = left + 1;
		}
	}

private:
	vector<T> _arr; 
};

#endif //__Heap_H_
