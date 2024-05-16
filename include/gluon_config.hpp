#ifndef GLUON_GLUON_CONFIG_HPP
#define GLUON_GLUON_CONFIG_HPP

#ifdef GLUON_EXPORT
#define GLUON_API __declspec(dllexport)
#else
#define GLUON_API
#endif

#endif //GLUON_GLUON_CONFIG_HPP
