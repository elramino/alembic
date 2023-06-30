
#ifndef ABC_ALEMBIC_H
#define ABC_ALEMBIC_H

// As defined in original alembic implementation
#define ABC_TPC_ACYCLIC_FLAG (DBL_MAX/32.0f)

#include <stdlib.h>

#include "ogawa.h"
#include "alembic_proptypes.h"

enum time_per_cycle_t
{
    TPC_UNIFORM,
    TPC_CYCLIC,
    TPC_ACYCLIC
};

struct abc_archive_t
{
    struct ogw_group_t     root;
    struct abc_metadata_t  md;
    FILE*                  fd;
};

struct abc_grouplist_t
{
    struct ogw_group_t* groups;
    uint64_t            num_groups;
};

int    abc_open(const char* file_name, struct abc_archive_t* out);
void   abc_close(struct abc_archive_t* archive);
int    abc_get_toplevel(const struct abc_archive_t* archive, struct ogw_group_t* out);
int    abc_get_children(const struct abc_archive_t* archive, const struct ogw_group_t* group, 
        struct abc_grouplist_t* out);
int    abc_get_child(const struct abc_archive_t* archive, const struct ogw_group_t* parent, 
        const char* name, struct ogw_group_t* out);
int    abc_get_object(const struct abc_archive_t* archive, const char* path, 
        struct ogw_group_t* out);
int    abc_get_props(const struct abc_archive_t* archive, struct ogw_group_t* group, 
        struct abc_proplist_t* out);

void abc_release_grouplist(struct abc_grouplist_t* grouplist);
void abc_release_proplist(struct abc_proplist_t* proplist);

int  abc_get_mdvalue(const struct abc_mdentry_t* entry, const char* key, char* out, size_t* buf_size);
int  abc_mdvalue_to_schema(const struct abc_mdentry_t* entry, const char* key, enum abc_schema* out);

int abc_parse_schema(const struct abc_archive_t* archive, const struct abc_proplist_t* list, 
        const struct abc_schema_def_t* schema_def, void* out);
int abc_parse_property(const struct abc_archive_t* archive, const struct abc_prop_t* prop, 
        const struct abc_prop_def_t* def, void* out);
int abc_parse_scalar(const struct abc_archive_t* archive, const struct abc_prop_t* prop, 
        const struct ogw_group_t* group, struct abc_scalar_t* out);
void abc_release_scalar(struct abc_scalar_t* scalar);
int  abc_parse_array_sample(FILE* f, const struct ogw_group_t* parent, enum abc_dtype dtype, 
        uint64_t sample_index, struct abc_array_sample_t* out);
void abc_release_array_sample(struct abc_array_sample_t* array);
int abc_parse_array(const struct abc_archive_t* archive, const struct abc_prop_t* prop,
        const struct ogw_group_t* group, struct abc_array_t* out);
void abc_release_array(struct abc_array_t* array);

#endif
