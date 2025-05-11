#ifndef GLUON_GLUON_CONFIG_HPP_
#define GLUON_GLUON_CONFIG_HPP_

#ifdef GLUON_EXPORT
// #define GLUON_API __declspec(dllexport)
#define GLUON_API __attribute__((visibility("default")))
#else // GLUON_EXPORT
// #define GLUON_API __declspec(dllimport)
#define GLUON_API
#endif // GLUON_EXPORT

#define GLUON_HIDDEN __attribute__((visibility("hidden")))

#endif //GLUON_GLUON_CONFIG_HPP_
