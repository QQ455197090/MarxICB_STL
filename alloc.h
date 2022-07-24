#ifndef MARXICB_STL_ALLOC
#define MARXICB_STL_ALLOC
#define __FIRST_MALLOC //Ĭ���õ�һ�����������ڶ����鷳���׳���...
#ifdef __FIRST_MALLOC
//class __malloc_alloc_template;//�������vs���Եģ�������������ͱ���gcc����û��
//typedef __malloc_alloc_template malloc_alloc;
//typedef malloc_alloc alloc;
#else
//ֻ�д����˲��ܿ����ĵڶ���������
#endif
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include"stl_config.h"
#include"stl_construct.h"
__STL_BEGIN_NAMESPACE
class __malloc_alloc_template;//�������vs���Եģ�������������ͱ���gcc����û��
typedef __malloc_alloc_template malloc_alloc;
typedef malloc_alloc alloc;
template<class T,class Alloc>
class simple_alloc {//ͨ��alloc
public:
	static T* allocate(size_t n) {
		return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	}
	static T* allocate(void) {
		return (T*)Alloc::allocate(sizeof(T));
	}
	static void deallocate(T* p, size_t n) {
		if (0 != n)Alloc::deallocate(p, n * sizeof(T));
	}
	static void deallocate(T* p) {
		Alloc::deallocate(p, sizeof(T));
	}
};
class __malloc_alloc_template {//��һ��alloc
private:
//	static void* oom_malloc(size_t);
//	static void* oom_realloc(void*, size_t);
	static void (*__malloc_alloc_oom_handler)();
public:
	static void* allocate(size_t n) {
		void* result = malloc(n);
		//�޷�����ʱ
		if (result == 0) {//result = oom_malloc(n);
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
			return result;
	}
	static void deallocate(void* p, size_t n) {
		free(p);
	}
	static void* reallocate(void* p, size_t old_sz, size_t new_sz) {
		void* result = realloc(p, new_sz);
		//�޷�����ʱ
		if (result == 0) { //result = oom_realloc(p, new_sz);
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
		return result;
	}
	static void(*set_malloc_handler(void(*f)()))(){//Ī�ţ����Ƿ��غ���ָ�룬�����Ǻ���ָ��
		void (*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return(old);
	}

};
/* ����ǹ����쳣����ģ�ֱ�Ӹĳ�fprintf error��,����ʹ�ÿɸ��ĵ�
template<int inst>
void(*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;
template<int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t n) {
	void (*my_malloc_handler)();
	void* result;
	for (;;) {
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (my_malloc_handler == 0) { __THROW_BAD_ALLOC; }
		result = malloc(n);
		if (result)return(result);
	}
}
*/
__STL_END_NAMESPACE
#endif
