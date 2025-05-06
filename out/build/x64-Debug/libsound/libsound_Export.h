
#ifndef libsound_API_H
#define libsound_API_H

#ifdef libsound_STATIC
#  define libsound_API
#  define LIBSOUND_NO_EXPORT
#else
#  ifndef libsound_API
#    ifdef libsound_EXPORTS
        /* We are building this library */
#      define libsound_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define libsound_API __declspec(dllimport)
#    endif
#  endif

#  ifndef LIBSOUND_NO_EXPORT
#    define LIBSOUND_NO_EXPORT 
#  endif
#endif

#ifndef LIBSOUND_DEPRECATED
#  define LIBSOUND_DEPRECATED __declspec(deprecated)
#endif

#ifndef LIBSOUND_DEPRECATED_EXPORT
#  define LIBSOUND_DEPRECATED_EXPORT libsound_API LIBSOUND_DEPRECATED
#endif

#ifndef LIBSOUND_DEPRECATED_NO_EXPORT
#  define LIBSOUND_DEPRECATED_NO_EXPORT LIBSOUND_NO_EXPORT LIBSOUND_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef LIBSOUND_NO_DEPRECATED
#    define LIBSOUND_NO_DEPRECATED
#  endif
#endif

#endif /* libsound_API_H */
