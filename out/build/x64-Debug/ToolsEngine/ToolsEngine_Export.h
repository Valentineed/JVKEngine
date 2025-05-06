
#ifndef ToolsEngine_API_H
#define ToolsEngine_API_H

#ifdef ToolsEngine_STATIC
#  define ToolsEngine_API
#  define TOOLSENGINE_NO_EXPORT
#else
#  ifndef ToolsEngine_API
#    ifdef ToolsEngine_EXPORTS
        /* We are building this library */
#      define ToolsEngine_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define ToolsEngine_API __declspec(dllimport)
#    endif
#  endif

#  ifndef TOOLSENGINE_NO_EXPORT
#    define TOOLSENGINE_NO_EXPORT 
#  endif
#endif

#ifndef TOOLSENGINE_DEPRECATED
#  define TOOLSENGINE_DEPRECATED __declspec(deprecated)
#endif

#ifndef TOOLSENGINE_DEPRECATED_EXPORT
#  define TOOLSENGINE_DEPRECATED_EXPORT ToolsEngine_API TOOLSENGINE_DEPRECATED
#endif

#ifndef TOOLSENGINE_DEPRECATED_NO_EXPORT
#  define TOOLSENGINE_DEPRECATED_NO_EXPORT TOOLSENGINE_NO_EXPORT TOOLSENGINE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef TOOLSENGINE_NO_DEPRECATED
#    define TOOLSENGINE_NO_DEPRECATED
#  endif
#endif

#endif /* ToolsEngine_API_H */
