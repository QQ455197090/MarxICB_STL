#ifndef MARXICB_VECTOR
#define MARXICB_VECTOR
#include"stl_config.h"
#include"type_traits.h"
#include"stl_iterator_base.h"
#include"stl_construct.h"
#include"alloc.h"
#include"stl_uninitialized.h"
#include<stddef.h>
__STL_BEGIN_NAMESPACE
template<class T,class Alloc=alloc>
class vector {
public:
	//vector的嵌套型别定义
	typedef T            value_type;
    typedef value_type*  pointer;
	typedef value_type*  iterator;
	typedef value_type&  reference;
	typedef size_t       size_type;
	typedef ptrdiff_t    difference_type;
protected:
	typedef simple_alloc<value_type, Alloc> data_allocator;
	iterator start;
	iterator finish;
	iterator end_of_storage;

	void insert_aux(iterator position, const T& x);
	void deallocate() {
		if (start)
			//data_allocator::deallocate(start, size_t(end_of_storage - start));
			__malloc_alloc_template::deallocate(start, size_t(end_of_storage - start));
	}
	void fill_initialize(size_type n, const T& value) {
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}
public:
	iterator begin() { return start; }
	iterator end() { return finish; }
	size_type size()const { return size_type(end() - begin()); }
	size_type capacity()const {
		return size_type(end_of_storage - start);
	}
	bool empty()const { return begin() == end(); }
	reference operator[](size_type n) { return *(start + n); }
	vector():start(0),finish(0),end_of_storage(0){}
	vector(size_type n, const T& value) { fill_initialize(n, value); }
	vector(int n, const T& value) { fill_initialize(n, value); }
	vector(long n, const T& value) { fill_initialize(n, value); }
	explicit vector(size_type n) { fill_initialize(n, T()); }
	~vector() {
		destroy(start, finish);
		deallocate();
	}
	reference front() { return *begin(); }
	reference back() { return *(end() - 1); }
	void push_bake(const T& x) {
		if (finish != end_of_storage) {
			costruct(finish, x);
			++finish;
		}
		else insert_aux(end(), x);//class外定义，直接用insert
	}
	//插入数据
	void insert(iterator position, const T& x) { insert_aux(position, x); }
	void insert(iterator position, size_type n, const T& x);
	void pop_back() {
		--finish;
		destroy(finish);
	}
	iterator erase(iterator position) {
		if (position + 1 != end()) copy(position + 1, finish, position);
		--finish;
		destroy(finish);
		return position;
	}
	iterator erase(iterator first, iterator last) {//删除[first,last)
		iterator i = copy(last, finish, first);
		destroy(i, finish);
		finish = finish - (last - first);
		return finish;
	}
	void resize(size_type new_size, const T& x) {
		if (new_size < size())
			erase(begin() + new_size, end());
		else
			insert(end(), new_size, end());
	}
	void resize(size_type new_size) { resize(new_size, T()); }
	void clear() { erase(begin(), end()); }
protected:
	iterator allocate_and_fill(size_type n, const T& x) {
		iterator result = data_allocator::allocate(n);
		unitialized_fill_n(result, n, x);
		return result;
	}
};

template<class T,class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& x) {
	if (finish != end_of_storage) {
		construct(finish, *(finish - 1));
		++finish;
		T x_copy = x;//先往后挪，再赋值
		copy_backward(position, finish - 2, finish - 1);//还没定义
		*position = x_copy;
	}
	else {//无可用空间时,创建一个二倍大小空间，把原来的copy进来再赋最后一个
		const size_type old_size = size();
		const size_type len = old_size != 0 ? 2 * old_size : 1;
		iterator new_start = data_allocator::allocate(len);
		iterator new_finish = new_start;
		new_finish = uninitialized_copy(start, position, new_start);
		construct(new_finish, x);
		++new_finish;
		new_finish = uninitialized_copy(position, finish, new_finish);
		destroy(begin(), end());
		deallocate();
		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + len;
	}
}
template<class T,class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T& x) {
	if (n == 0)return;//如果要的是0个，直接return
	if(size_type(end_of_storage - finish )>= n) {   //备用空间足够的话
		T x_copy = x;
		const size_type elems_after = finish - position;
		iterator old_finish = finish;
		if (elems_after > n) {
			uninitialized_copy(finish - n, finish, finish);
			finish += n;
			copy_backward(position, old_finish - n, old_finish);
			fill(position, old_finish, x_copy);
		}
		else {
			unitialized_fill_n(finish, n - elems_after, x_copy);
			finish += n - elems_after;
			uninitialized_copy(position, old_finish, finish);
			finish += elems_after;
			fill(position, old_finish, x_copy);
		}
	}
	//如果空间不够
	else {      
		const size_type old_size = size();
		const size_type len = old_size + (old_size > n ? old_size : n);
		iterator new_start = data_allocator::allocate(len);
		iterator new_finish = new_start;
		new_finish = uninitialized_copy(start, position, new_start);
		new_finish = unitialized_fill_n(new_finish, n, x);
		new_finish = uninitialized_copy(position, finish, new_finish);
		destroy(start, finish);
		deallocate();
		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + len;
	}
}
__STL_END_NAMESPACE
#endif