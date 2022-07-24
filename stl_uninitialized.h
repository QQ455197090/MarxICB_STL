#ifndef MARXICB_STL_UNITIALIZATION
#define MARXICB_STL_UNITIALIZATION
#include"stl_config.h"
#include"type_traits.h"
#include"stl_iterator_base.h"
#include"stl_construct.h"
#include<cstring>
__STL_BEGIN_NAMESPACE
// Copy
template<class InputIterator,class ForwardIterator>
inline ForwardIterator 
uninitialized_copy(InputIterator first,
								   InputIterator last,
								   ForwardIterator result) {
	return __unitialized_copy(first, last, value_type(result));
}

template<class InputIterator, class ForwardIterator,class T>
inline ForwardIterator
__uninitialized_copy(InputIterator first,
					 InputIterator last,
					 ForwardIterator result,
					 T*) {
	typedef typename __type_traits<T>::is_POD_type is_POD;
	return __uninitialized_copy_aux(first, last, result, is_POD());
}

template<class InputIterator,class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first,
						 InputIterator last,
						 ForwardIterator result,
						 __true_type) {
	return copy(first, last, result);
}

//由于还没写算法，所以copy先写出来了
template<class InputIterator,class ForwardIterator>
ForwardIterator copy(InputIterator first,
					 InputIterator last,
					 ForwardIterator result) {
	while (first != last) {
		*result = *first;
		++result;
		++first;
	}
}

template< class BidirIt1, class BidirIt2 >
BidirIt2 copy_backward(BidirIt1 first, 
					   BidirIt1 last,
					   BidirIt2 d_last)
{
	while (first != last) {
		*(--d_last) = *(--last);
	}
	return d_last;
}

template<class InputIterator, class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first,
						 InputIterator last,
						 ForwardIterator result,
						 __false_type) {
	ForwardIterator cur = result;
	for (; first != last; ++first, ++result) {
		construct(&*cur, *first);
	}
	return cur;
}

//char*特化版，直接移动
inline char* __uninitialized_copy(const char* first,
								  const char* last,
								  char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

//FILL
template<class ForwardIterator,class T>
void unitialized_fill(ForwardIterator first,
					  ForwardIterator last,
					  const T& x) {
	__unitialized_fill(first, last, x, value_type(first));
}

template<class ForwardIterator, class T,class T1>
void __unitialized_fill(ForwardIterator first,
						ForwardIterator last,
						const T& x,
						T1*) {
	typedef typename __type_traits<T>::is_POD_type is_POD;
	__unitialized_fill_aux(first, last, x, is_POD);
}

template<class ForwardIterator,class T>
inline void
__unitialized_fill_aux(ForwardIterator first,
					   ForwardIterator last,
					   const T&x,
					   __true_type) {
	fill(first, last, x);
}

template<class ForwardIterator,class T>
void fill(ForwardIterator first,
		  ForwardIterator last,
		  const T& val) {
	while (first != last) {
		*first = val;
		++first;
	}
}

template<class ForwardIterator, class T>
inline void
__unitialized_fill_aux(ForwardIterator first,
					   ForwardIterator last,
					   const T& x,
					   __false_type) {
	ForwardIterator cur = first;
	for (; cur != last; ++cur) construct(&*cur, x);
}

//FILL_N
template<class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator
__unitialized_fill_n(ForwardIterator first,
	Size n,
	const T& x,
	T1*);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
template<class ForwardIterator,class Size,class T>
ForwardIterator
unitialized_fill_n(ForwardIterator first,
				   Size n,
				   const T& x) {
	return __unitialized_fill_n(first, n, x, value_type(first));
}

template<class ForwardIterator,class Size,class T,class T1>
inline ForwardIterator
__unitialized_fill_n(ForwardIterator first,
					 Size n,
					 const T&x,
					 T1*) {
	typedef typename __type_traits<T>::is_POD_type is_POD;
	is_POD temp;
	return __uninitialized_fill_n_aux(first, n, x, temp );//is_POD原地爆炸
}

template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& val);

template<class ForwardIterator,class Size,class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first,
						   Size n,
						   const T&x,
						   __true_type) {
	return fill_n(first, n, x);//报错找不着标识符fill_n
}

//stl算法fill_n我先写了
template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& val)
{
	while (n > 0) {
		*first = val;
		++first; --n;
	}
	return first; 
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first,
						   Size n,
						   const T& x,
						   __false_type) {
	ForwardIterator cur = first;
	for (; n > 0; --n, ++cur) construct(&*cur, x);
	return cur;
}

__STL_END_NAMESPACE
#endif