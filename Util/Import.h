#ifndef IMPORT_H
#define IMPORT_H

#ifndef LIB_IMPORT
#define LIB_IMPORT
#endif

#ifdef __linux__
    #include <dlfcn.h>
    #define LIBTYPE void*
    #define LIBOPEN(lib) dlopen((lib), RTLD_LAZY)
    #define LIBFUNC(lib, fn) dlsym((lib), (fn))
    #define LIBCLOSE(lib) dlclose((lib))
#elif defined(_WIN32) || defined(WIN32)
    #include <windows.h>
    #define LIBTYPE HINSTANCE
    #define LIBOPEN(lib) LoadLibrary(lib)
    #define LIBFUNC(lib, fn) GetProcAddress((lib), (fn))
    #define LIBCLOSE(lib) FreeLibrary((lib))
#endif

#endif /* IMPORT_H */