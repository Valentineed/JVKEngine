
#ifndef libphys_API_H
#define libphys_API_H

#ifdef libphys_STATIC
#  define libphys_API
#  define LIBPHYS_NO_EXPORT
#else
#  ifndef libphys_API
#    ifdef libphys_EXPORTS
        /* We are building this library */
#      define libphys_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define libphys_API __declspec(dllimport)
#    endif
#  endif

#  ifndef LIBPHYS_NO_EXPORT
#    define LIBPHYS_NO_EXPORT 
#  endif
#endif

#ifndef LIBPHYS_DEPRECATED
#  define LIBPHYS_DEPRECATED __declspec(deprecated)
#endif

#ifndef LIBPHYS_DEPRECATED_EXPORT
#  define LIBPHYS_DEPRECATED_EXPORT libphys_API LIBPHYS_DEPRECATED
#endif

#ifndef LIBPHYS_DEPRECATED_NO_EXPORT
#  define LIBPHYS_DEPRECATED_NO_EXPORT LIBPHYS_NO_EXPORT LIBPHYS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef LIBPHYS_NO_DEPRECATED
#    define LIBPHYS_NO_DEPRECATED
#  endif
#endif

#endif /* libphys_API_H */
