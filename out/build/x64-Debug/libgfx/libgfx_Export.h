
#ifndef libgfx_API_H
#define libgfx_API_H

#ifdef libgfx_STATIC
#  define libgfx_API
#  define LIBGFX_NO_EXPORT
#else
#  ifndef libgfx_API
#    ifdef libgfx_EXPORTS
        /* We are building this library */
#      define libgfx_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define libgfx_API __declspec(dllimport)
#    endif
#  endif

#  ifndef LIBGFX_NO_EXPORT
#    define LIBGFX_NO_EXPORT 
#  endif
#endif

#ifndef LIBGFX_DEPRECATED
#  define LIBGFX_DEPRECATED __declspec(deprecated)
#endif

#ifndef LIBGFX_DEPRECATED_EXPORT
#  define LIBGFX_DEPRECATED_EXPORT libgfx_API LIBGFX_DEPRECATED
#endif

#ifndef LIBGFX_DEPRECATED_NO_EXPORT
#  define LIBGFX_DEPRECATED_NO_EXPORT LIBGFX_NO_EXPORT LIBGFX_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef LIBGFX_NO_DEPRECATED
#    define LIBGFX_NO_DEPRECATED
#  endif
#endif

#endif /* libgfx_API_H */
