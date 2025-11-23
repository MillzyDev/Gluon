#ifndef GLUON_CONFIG_HPP_
#define GLUON_CONFIG_HPP_

#include <cstddef>

#ifdef GLUON_EXPORT

/*
 *  GLUON_API - Export and imports API functions (what it says on the tin)
 */
//#ifdef _WIN32
//#define GLUON_API __declspec(dllexport)
//#else
#define GLUON_API __attribute__((visibility("default")))
//#endif // _WIN32

#else // GLUON_EXPORT

// _WIN32
//#define GLUON_API __declspec(dllimport)
//#else // _WIN32
#define GLUON_API
//#endif // _WIN32

#endif // GLUON_EXPORT

/*
 *  GLUON_HIDDEN - Never export, never import
 */
//#ifdef _WIN32
//#define GLUON_HIDDEN
//#else
#define GLUON_HIDDEN __attribute__((visibility("hidden")))
//#endif // _WIN32

#ifndef HANDLE
#define HANDLE void *
#endif // HANDLE

#ifndef DWORD
#define DWORD ::std::uint32_t
#endif // DWORD

#endif // GLUON_CONFIG_HPP_