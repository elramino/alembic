#include "proputils.h"
#include "logging.h"

bool abc_hasprop(const char* prop_name, const struct abc_proplist_t* list)
{
    const uint64_t num_props = list->num_props;

    for (uint64_t i = 0; i < num_props; ++i)
    {
        if (strcmp(prop_name, list->props[i].name) == 0)
        {
            return true;
        }
    }

    return false;
}

struct abc_prop_t* abc_getprop(const char* prop_name, const struct abc_proplist_t* list)
{
    const uint64_t num_props = list->num_props;

    for (uint64_t i = 0; i < num_props; ++i)
    {
        if (strcmp(list->props[i].name, prop_name) == 0)
        {
            return &list->props[i];
        }
    }

    return NULL;
}

bool abc_valid_prop(const struct abc_prop_t* prop, const struct abc_prop_def_t* def)
{
    assert(prop != NULL);

    if (def->prop_class != prop->prop_class) { return false; }
    if (def->dtype      != prop->dtype)      { return false; }

    return true;
}

enum abc_dtype abc_to_dtype(enum abc_pod pod, size_t extent)
{
    for (size_t i = 0; i < ABC_NUM_DTYPES; ++i)
    {
        if (pod == ABC_POD_U8STR)
        {
            return ABC_DTYPE_U8STR;
        }
        if (pod == ABC_POD_WSTR)
        {
            return ABC_DTYPE_WSTR;
        }

        if (pod    == abc_dtype_def[i].pod && 
            extent == abc_dtype_def[i].extent)
        {
            return (enum abc_dtype) i;
        }
    }

    log_warn("Could not find property data type based on POD type %s and extent %zu",
            abc_pod_strings[pod], extent);
    return -1;
}
