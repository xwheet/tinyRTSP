#ifndef TINYRTSP_CONFIG_H
#define TINYRTSP_CONFIG_H

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

// Windows (XP/Vista/7/CE and Windows Mobile) macro definition
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define TRTSP_UNDER_WINDOWS	1
#	if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP || WINAPI_FAMILY == WINAPI_FAMILY_APP)
#		define TRTSP_UNDER_WINDOWS_RT		1
#	endif
#endif


#if (TRTSP_UNDER_WINDOWS || defined(__SYMBIAN32__)) && defined(TINYRTSP_EXPORTS)
# 	define TINYRTSP_API		__declspec(dllexport)
# 	define TINYRTSP_GEXTERN extern __declspec(dllexport)
#elif (TRTSP_UNDER_WINDOWS || defined(__SYMBIAN32__)) && !defined(TINYRTSP_IMPORTS_IGNORE)
# 	define TINYRTSP_API __declspec(dllimport)
# 	define TINYRTSP_GEXTERN __declspec(dllimport)
#else
#	define TINYRTSP_API
#	define TINYRTSP_GEXTERN	extern
#endif

/* Guards against C++ name mangling
*/
#ifdef __cplusplus
#	define TRTSP_BEGIN_DECLS extern "C" {
#	define TRTSP_END_DECLS }
#else
#	define TRTSP_BEGIN_DECLS
#	define TRTSP_END_DECLS
#endif

/* Disable some well-known warnings
*/
#ifdef _MSC_VER
#	if !defined(_CRT_SECURE_NO_WARNINGS)
#		define _CRT_SECURE_NO_WARNINGS
#	endif /* _CRT_SECURE_NO_WARNINGS */
#endif

/* Detecting C99 compilers
 */
#if (__STDC_VERSION__ == 199901L) && !defined(__C99__)
#	define __C99__
#endif

#include <stdint.h>
#ifdef __SYMBIAN32__
#include <stdlib.h>
#endif

#if HAVE_CONFIG_H
#include <config.h>
#endif

#endif // TINYRTSP_CONFIG_H
