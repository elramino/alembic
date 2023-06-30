
#ifndef ABC_UTILS_H
#define ABC_UTILS_H

#define countof(X) (sizeof(X)/sizeof(X[0]))
#define free_invalidate(X) if (X != NULL) { free(X); X = NULL; }

#endif

