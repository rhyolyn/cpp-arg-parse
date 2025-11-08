#pragma once

#if defined _WIN32 || defined __CYGWIN__

#ifdef API_DLL_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

#else // !_WIN32

#ifdef API_DLL_EXPORTS
#define API __attribute__((visibility("default")))
#else
#define API
#endif

#endif // _WIN32