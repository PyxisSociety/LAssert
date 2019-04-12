#include <stdlib.h>

extern void *__libc_malloc(size_t size);
extern void *__libc_calloc(size_t nmemb, size_t size);
extern void *__libc_realloc(void * ptr, size_t size);
extern void *__libc_reallocarray(void * ptr, size_t nmemb, size_t size);

int _lassert_malloc_lock = 0;

void * malloc(size_t size){
    if(_lassert_malloc_lock){
        return NULL;
    }    
    return __libc_malloc(size);
}

void * calloc(size_t nmemb, size_t size){
    if(_lassert_malloc_lock){
        return NULL;
    }    
    return __libc_calloc(nmemb, size);
}

void * realloc(void * ptr, size_t size){
    if(_lassert_malloc_lock){
        return NULL;
    }    
    return __libc_realloc(ptr, size);
}

#ifdef _GNU_SOURCE
void * reallocarray(void * ptr, size_t nmemb, size_t size){
    if(_lassert_malloc_lock){
        return NULL;
    }    
    return __libc_reallocarray(ptr, nmemb, size);
}
#endif
