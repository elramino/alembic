#ifndef ABC_SCHEMA_POLYMESHV1_H
#define ABC_SCHEMA_POLYMESHV1_H

#include "alembic_proptypes.h"

struct abc_proplist_t;
struct abc_archive_t;

struct abc_schema_polymeshv1_t
{
    uint64_t num_samples;

    // GeomBase props
    struct abc_scalar_t self_bounds;
    struct abc_scalar_t child_bounds;
    //struct abc_scalar_t arb_geom_params; // Unsupported for now
    //struct abc_scalar_t user_props;      // Unsupported for now
                                         
    // Polymeshv1 props
    struct abc_array_t positions;
    struct abc_array_t normals;
    struct abc_array_t uvs;
    struct abc_array_t velocities;
    struct abc_array_t face_indices;
    struct abc_array_t face_counts;
};

extern const struct abc_schema_def_t schema_def_polymeshv1;

int abc_parse_schema_polymeshv1(const struct abc_archive_t* archive, 
        const struct abc_proplist_t* list, struct abc_schema_polymeshv1_t* out);
void abc_release_schema_polymeshv1(struct abc_schema_polymeshv1_t* polymesh);

#endif
