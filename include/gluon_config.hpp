#ifndef GLUON_GLUON_CONFIG_HPP
#define GLUON_GLUON_CONFIG_HPP

#ifdef GLUON_EXPORT
#define GLUON_API __attribute__((visibility("default")))
#else
#define GLUON_API __declspec(dllimport)
#endif

#define GLUON_HIDDEN __attribute__((visibility("hidden")))

#endif //GLUON_GLUON_CONFIG_HPP
