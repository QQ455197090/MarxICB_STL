#ifndef MARXICB_STL_ITERATOR_BASE
#define MARXICB_STL_ITERATOR_BASE
#include <stddef.h>
#include "stl_config.h"
__STL_BEGIN_NAMESPACE
//五种迭代器类型
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag:public input_iterator_tag{};
struct bidirectonal_iterator_tag:public forward_iterator_tag{};
struct random_access_iterator_tag:public bidirectonal_iterator_tag{};

//经典迭代器
template<class Category,class T,class Distance=ptrdiff_t,
		 class Pointer=T*,class Reference=T&>
struct iterator {
	typedef Category iterator_category;
	typedef T		 value_type;
	typedef Distance difference_type;
	typedef Reference reference;
	typedef Pointer  pointer;
};
//traits
template<class Iterator>
struct iterator_traits {
	typedef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::value_type		 value_type;
	typedef typename Iterator::difference_type   difference_type;
	typedef typename Iterator::pointer           pointer;
	typedef typename Iterator::reference         reference;
};
//针对原生指针设计的traits偏特化版
template<class T>
struct iterator_traits<T* > {
	typedef  random_access_iterator_tag    iterator_category;
	typedef  T							   value_type;
	typedef  ptrdiff_t                     difference_type;
	typedef  T*                            pointer;
	typedef  T&                            reference;
};
//针对const pointer类型
template<class T>
struct iterator_traits<const T* > {
	typedef  random_access_iterator_tag    iterator_category;
	typedef  T							   value_type;
	typedef  ptrdiff_t                     difference_type;
	typedef  T*                            pointer;
	typedef  T&                            reference;
};

template<class Iterator>
inline typename iterator_traits<Iterator>::iterator_catagory
iterator_category(const Iterator&) {
	typedef typename iterator_traits<Iterator>::iterator_category category;
	return category();//它们都是struct，所以()
}

template<class Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&) {
	return typename iterator_traits<Iterator>::difference_type *(0);
}

template<class Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator& ) {
//	return  typename iterator_traits<Iterator>::value_type *(0); 报错
	typename iterator_traits<Iterator>::value_type* t = 0;
	return t;
}

template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last,
		   input_iterator_tag) {
	typename iterator_traits<InputIterator>::difference_type n = 0;
	while (first != last) {
		++first; ++n;
	}
	return n;
}

template<class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last,
		   random_access_iterator_tag) {
	return last - first;
}

template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
	typename iterator_traits<InputIterator>::iterator_category category;
	return __distance(first, last, category());
}

template<class InputIterator,class Distance>
inline void __advance(InputIterator& i, Distance n,
					  input_iterator_tag) {
	while (n--)i++;
}

template<class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n,
					   bidirectonal_iterator_tag) {
	if (n >= 0)
		while (n--)++i;
	else
		while (n++)--i;
}

template<class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n,
				      random_access_iterator_tag) {
	i += n;
}

template<class InputIterator,class Distance>
inline void advance(InputIterator& i, Distance n) {
	__advance(i, n, iterator_category(i));
}

__STL_END_NAMESPACE
#endif
