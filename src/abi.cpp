#include <cstddef>

#if FW_USE_RTOS
extern "C"
{
#include <FreeRTOS.h>
#include <task.h>
}
#endif

void *operator new(std::size_t size)
{
#if FW_USE_RTOS
    return pvPortMalloc(size);
#else
    return reinterpret_cast<void *>(0xffffffff);
#endif
}

void *operator new[](std::size_t size)
{
    return operator new(size);
}

void operator delete(void *ptr)
{
#if FW_USE_RTOS
    vPortFree(ptr);
#else
    while (1)
    {
    }
#endif
}

void operator delete(void *ptr, unsigned int)
{
    operator delete(ptr);
}

void operator delete[](void *ptr)
{
    operator delete(ptr);
}

void operator delete[](void *ptr, unsigned int)
{
    operator delete[](ptr);
}

extern "C"
{

    int __aeabi_atexit(void *, void (*)(void *), void *)
    {
        return 0;
    }

    /**
     * Many faulty implementations are available where __guard is defined as a 64-bit integer by
     *
     * __extension__ typedef int __guard __attribute__((mode(__DI__)));
     *
     * This is correct for the Generic C++ ABI (GC++ABI), but not for the ARM C++ ABI. See section
     * 3.1 "Summary of differences from and additions to the generic C++ ABI" of "C++ ABI for the
     * ARM Architectur" (version 2.10):
     *
     * ===
     *
     * GC++ABI §2.8 Initialization guard variables
     * Static initialization guard variables are 4 bytes long not 8, and there is a different
     * protocol for using them which allows a guard variable to implement a semaphore when used as
     * the target of ARM SWP or LDREX and STREX instructions. See §3.2.3.
     *
     * GC++ABI §3.3.2 One-time construction API
     * The type of parameters to __cxa_guard_acquire, __cxa_guard_release and __cxa_guard_abort is
     * 'int*' (not '__int64_t*'), and use of fields in the guard variable differs. See §3.2.3.
     *
     * ===
     *
     * Using a 64-bit here will lead to reading beyond the actual guard variable and can thus lead
     * to serious bugs.
     */
    typedef int __guard;

    void __cxa_atexit(void (*)(void *), void *, void *)
    {
    }

    int __cxa_guard_acquire(__guard *g)
    {
        return !*g;
    }

    void __cxa_guard_release(__guard *g)
    {
        *g = 1;
    }

    void __cxa_guard_abort(__guard *)
    {
    }

    void __cxa_pure_virtual()
    {
        while (1)
        {
        }
    }

    // some functions in cmath follow the compiler flag -fno-math-errno
    // but not so for std::pow...
    void __errno() {
        
    }
}