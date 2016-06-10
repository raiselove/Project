#pragma once

#include<string>
#include "BitMap.h"
#include "common.h"
template<class T = string,
         class HashFunc1 =__HashFunc1< T>,
         class HashFunc2 = __HashFunc2< T>,
         class HashFunc3 = __HashFunc3< T>,
         class HashFunc4 = __HashFunc4< T>,
         class HashFunc5 = __HashFunc5< T>
>
class BoolmFilter
{
public:
        BoolmFilter( size_t capatity = 0)
        {
                _capatity = NewSize( capatity);
                _bm.Resize(_capatity);
        }
         void Set(const T& key)
        {
                 size_t index1 = HashFunc1 ()(key);
                 size_t index2 = HashFunc2 ()(key);
                 size_t index3 = HashFunc3 ()(key);
                 size_t index4 = HashFunc4 ()(key);
                 size_t index5 = HashFunc5 ()(key);

                _bm.Set(index1%_capatity);
                _bm.Set(index2%_capatity);
                _bm.Set(index3%_capatity);
                _bm.Set(index4%_capatity);
                _bm.Set(index5%_capatity);
        }
         bool Test(const T& key)
        {
                 size_t index1 = HashFunc1 ()(key);
                 if (!_bm.BitMapTest(index1%_capatity))
                {
                         return false ;
                }
                 size_t index2 = HashFunc2 ()(key);
                 if (!_bm.BitMapTest(index2%_capatity))
                {
                         return false ;
                }
                 size_t index3 = HashFunc3 ()(key);
                 if (!_bm.BitMapTest(index3%_capatity))
                {
                         return false ;
                }
                 size_t index4 = HashFunc4 ()(key);
                 if (!_bm.BitMapTest(index4%_capatity))
                {
                         return false ;
                }
                 size_t index5 = HashFunc5 ()(key);
                 if (!_bm.BitMapTest(index5%_capatity))
                {
                         return false ;
                }
                 return true ;
        }
protected:
         BitMap _bm;
         size_t _capatity;
};
