# 记录踩过的坑

## 设置锁的attr的函数报警告
为了可重入锁, 用了`pthread_mutexattr_settype`函数, 报Warning:  
*implicit declaration of function ‘pthread_mutexattr_settype’*

由"pthread.h"中如下代码可知, 需要定义一个`__USE_UNIX98`的宏
```c
#ifdef __USE_UNIX98  
/* Return in *KIND the mutex kind attribute in *ATTR.  */  
extern int pthread_mutexattr_gettype (__const pthread_mutexattr_t *__restrict __attr, int *__restrict __kind)  
    __THROW;  
  
/* Set the mutex kind attribute in *ATTR to KIND (either PTHREAD_MUTEX_NORMAL, 
   PTHREAD_MUTEX_RECURSIVE, PTHREAD_MUTEX_ERRORCHECK, or 
   PTHREAD_MUTEX_DEFAULT).  */  
extern int pthread_mutexattr_settype (pthread_mutexattr_t *__attr, int __kind)  
    __THROW;  
  
/* Return in *PROTOCOL the mutex protocol attribute in *ATTR.  */  
extern int pthread_mutexattr_getprotocol (__const pthread_mutexattr_t *__restrict __attr, int *__restrict __protocol)  
    __THROW;  
  
/* Set the mutex protocol attribute in *ATTR to PROTOCOL (either 
   PTHREAD_PRIO_NONE, PTHREAD_PRIO_INHERIT, or PTHREAD_PRIO_PROTECT).  */  
extern int pthread_mutexattr_setprotocol (pthread_mutexattr_t *__attr, int __protocol)  
    __THROW;  
  
/* Return in *PRIOCEILING the mutex prioceiling attribute in *ATTR.  */  
extern int pthread_mutexattr_getprioceiling (__const pthread_mutexattr_t *__restrict __attr, int *__restrict __prioceiling)  
    __THROW;  
  
/* Set the mutex prioceiling attribute in *ATTR to PRIOCEILING.  */  
extern int pthread_mutexattr_setprioceiling (pthread_mutexattr_t *__attr, int __prioceiling)  
    __THROW;  
#endif 
```

由另一段`/usr/include/features.h`可知, 如果定义了_GNU_SOURCE, 就可以turn on这些feature.(不过可能会牺牲一些可移植性)
```c
/* If _GNU_SOURCE was defined by the user, turn on all the other features.  */  
#ifdef _GNU_SOURCE  
# undef  _ISOC99_SOURCE  
# define _ISOC99_SOURCE 1  
# undef  _POSIX_SOURCE  
# define _POSIX_SOURCE  1  
# undef  _POSIX_C_SOURCE  
# define _POSIX_C_SOURCE        200809L  
# undef  _XOPEN_SOURCE  
# define _XOPEN_SOURCE  700  
# undef  _XOPEN_SOURCE_EXTENDED  
# define _XOPEN_SOURCE_EXTENDED 1  
# undef  _LARGEFILE64_SOURCE  
# define _LARGEFILE64_SOURCE    1  
# undef  _BSD_SOURCE  
# define _BSD_SOURCE    1  
# undef  _SVID_SOURCE  
# define _SVID_SOURCE   1  
# undef  _ATFILE_SOURCE  
# define _ATFILE_SOURCE 1  
#endif  
```

结论: 要克服这个warning, 就加上`-D_GNU_SOURCE`编译选项. 

## PTHREAD_MUTEX_RECURSIVE 未定义
解决方法一:
`#define PTHREAD_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE_NP`  
原因:  PTHREAD_MUTEX_RECURSIVE 变量只是 POSIX 说明应该支持的变量，但是很多版本的 Linux 并没有定义此变量
解决方法二:
同`pthread_mutexattr_settype`, 加Flag `-D_GNU_SOURCE`