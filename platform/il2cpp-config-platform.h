#ifndef IL2CPP_CONFIG_PLATFORM_H_
#define IL2CPP_CONFIG_PLATFORM_H_

/**
 * The IL2CPP headers dont play nicely if you try to build for Windows using Clang, as it excepects MSVC as the
 * compiler. Before Unity 6, we could define IL2CPP_TARGET_CUSTOM=1 and any other config macros required. However this
 * is no longer the case, instead we need to define IL2CPP_USE_PLATFORM_CONFIG=1; This acts the same way as
 * IL2CPP_TARGET_CUSTOM=1 except it requires this header to be defined, which is then included in the IL2CPP platform
 * config header.
 */

#ifdef UNITY_6000

#define IL2CPP_TARGET_CUSTOM 1
#define IL2CPP_TARGET_WINDOWS 1
#define IL2CPP_TARGET_WINDOWS_DESKTOP 1

#define IL2CPP_PLATFORM_SUPPORTS_SYSTEM_CERTIFICATES 1
#define IL2CPP_PLATFORM_SUPPORTS_CPU_INFO 1

#define _UNICODE 1
#define UNICODE 1
#define STRICT 1

#endif // UNITY_6000

#endif // IL2CPP_CONFIG_PLATFORM_H_