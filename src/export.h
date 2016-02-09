#ifndef EXPORT
#define EXPORT

#ifdef SSQ_LIB_STATIC
#  define SSQ_EXPORT
#elif defined(SSQ_LIBRARY)
#  define SSQ_EXPORT Q_DECL_EXPORT
#else
#  define SSQ_EXPORT Q_DECL_IMPORT
#endif
#endif // EXPORT

