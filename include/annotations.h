#ifndef ATTRIBUTE
#define ATTRIBUTE(x) __attribute__((x))
#endif

#ifndef GUARDED_BY
#define GUARDED_BY(x) ATTRIBUTE(guarded_by(x))
#endif