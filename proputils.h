
#ifndef ABC_PROPUTILS_H
#define ABC_PROPUTILS_H

#include <stdbool.h>
#include <string.h>

#include "alembic_types.h"
#include "alembic_proptypes.h"

bool               abc_hasprop(const char* prop_name, const struct abc_proplist_t* list);
struct abc_prop_t* abc_getprop(const char* prop_name, const struct abc_proplist_t* list);
bool               abc_valid_prop(const struct abc_prop_t* prop, const struct abc_prop_def_t* def);
enum abc_dtype     abc_to_dtype(enum abc_pod pod, size_t extent);

#endif
