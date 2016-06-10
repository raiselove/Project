#pragma once

#include <vector>
//#include "common.h"
class BitMap
{
public:
        BitMap( size_t size = 0)
                :_size(0)
        {
                 //用resize开辟空间，_a中的值会被初始化为0

                 //加1为了让值全部能放到数组中，假如有36个数，36/32=1余4，而
                 //多开的那个空间就保证了这4个数能放下

                 //_a.resize(size/32+1);和下面的代码一个性质，只不过用移位运算符比除法的效率高
                _a.resize(( size >> 5) + 1);
        }
         //插入一个数据
         void Set(size_t x)
        {
                 size_t index = x >> 5;
                 size_t num = x % 32;
                 //当前位置如果等于0，没有数据，可以插入
                 if (!(_a[index] & (1 << num)))
                {
                        _a[index] |= (1 << num); //当前位置置1
                        ++_size;
                }
        }
         //删除
         void Reset(size_t x)
        {
                 size_t index = x >> 5;
                 size_t num = x % 32;
                 //当前位置为1，有数据，可以删除
                 if (_a[index] & (1 << num))
                {
                        _a[index] &= ~(1 << num); //当前位置置0
                        --_size;
                }
        }
         //判断是否有数据
         bool BitMapTest(size_t x)
        {
                 size_t index = x >> 5;
                 size_t num = x % 32;
                 if (_a[index] & (1 << num))
                {
                         return true ;
                }
                 return false ;
        }
                 void Resize(size_t size)
        {
                _a.resize( size);
        }
protected:
         vector<size_t > _a;
         size_t _size;
};
