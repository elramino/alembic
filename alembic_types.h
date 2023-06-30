#ifndef ABC_ALEMBIC_TYPES_H
#define ABC_ALEMBIC_TYPES_H

#include <stdbool.h>
#include <inttypes.h>

#include "alembic_proptypes.h"

struct abc_mdentry_t
{
    char**   keys;
    char**   values;
    uint64_t num_pairs;
};

struct abc_metadata_t
{
    struct abc_mdentry_t* entries;
    uint64_t              num_entries;
    char*                 storage;
};

struct abc_prop_t
{
    enum abc_prop_class prop_class;
    enum abc_dtype      dtype;
    
    uint64_t group_loc;
    uint32_t num_samples;
    char*    name;
    uint8_t  int_size;
    bool     with_timesamples;
    bool     homogeneous;
};

struct abc_proplist_t
{
    char*              sotitle;   // Schema object title
    enum abc_schema    schema;    // Schema 
    enum abc_schema    sbtype;    // Schema base type
    struct abc_prop_t* props;     // Stores individual properties
    uint64_t           num_props; // Number of properties
};

struct abc_prop_def_t
{
    char*               name;
    enum abc_prop_class prop_class;
    enum abc_dtype      dtype;
    size_t              moffset;
};

struct abc_schema_def_t
{
    const enum abc_schema        schema;
    const enum abc_schema        sbtype;
    const struct abc_prop_def_t* schema_props;    
    const char**                 mandatory_props;
    const uint64_t               num_props;
    const uint64_t               num_mandatory;
};

#endif
