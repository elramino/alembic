#include <stddef.h>

#include "schema_polymeshv1.h"
#include "alembic.h"

static const struct abc_prop_def_t supported_props[] = 
{
    // Inherited from GeomBase_v1
    // The first two properties are not supported as of now, may be added later
    //{ ".arbGeomParams",  ABC_PROPCLASS_COMPOUND, ABC_POD_F64, ?, parse_arbgeomparams  },
    //{ ".userProperties", ABC_PROPCLASS_COMPOUND, ABC_POD_F64, ?, parse_userproperties },
    { ".childBnds",   ABC_PROPCLASS_SCALAR,  ABC_DTYPE_BOX3D,    offsetof(struct abc_schema_polymeshv1_t, child_bounds) },
    { ".selfBnds",    ABC_PROPCLASS_SCALAR,  ABC_DTYPE_BOX3D,    offsetof(struct abc_schema_polymeshv1_t, self_bounds)  },

    // PolyMesh_v1 properties
    { "P",            ABC_PROPCLASS_ARRAY,   ABC_DTYPE_VECTOR3F, offsetof(struct abc_schema_polymeshv1_t, positions)    },
    { "N",            ABC_PROPCLASS_ARRAY,   ABC_DTYPE_VECTOR3F, offsetof(struct abc_schema_polymeshv1_t, normals)      },
    { "uv",           ABC_PROPCLASS_ARRAY,   ABC_DTYPE_VECTOR2F, offsetof(struct abc_schema_polymeshv1_t, uvs)          },
    { ".velocities",  ABC_PROPCLASS_ARRAY,   ABC_DTYPE_VECTOR3F, offsetof(struct abc_schema_polymeshv1_t, velocities)   },
    { ".faceIndices", ABC_PROPCLASS_ARRAY,   ABC_DTYPE_I32,      offsetof(struct abc_schema_polymeshv1_t, face_indices) },
    { ".faceCounts",  ABC_PROPCLASS_ARRAY,   ABC_DTYPE_I32,      offsetof(struct abc_schema_polymeshv1_t, face_counts)  }
};

static const char* mandatory_props[] = 
{
    ".faceIndices",
    ".faceCounts"
};

const struct abc_schema_def_t schema_def_polymeshv1 = 
{
    .schema          = ABC_SCHEMA_POLYMESHV1,
    .sbtype          = ABC_SCHEMA_GEOMBASEV1,
    .schema_props    = supported_props,
    .mandatory_props = mandatory_props,
    .num_props       = countof(supported_props),
    .num_mandatory   = countof(mandatory_props)
};

// Just a convenience wrapper around the generic schema parse function
int abc_parse_schema_polymeshv1(const struct abc_archive_t* archive, 
        const struct abc_proplist_t* list, struct abc_schema_polymeshv1_t* out)
{
    return abc_parse_schema(archive, list, &schema_def_polymeshv1, out);
}


void abc_release_schema_polymeshv1(struct abc_schema_polymeshv1_t* polymesh)
{
    // Inherited properties from GeomBase_v1
    abc_release_scalar(&polymesh->self_bounds);
    abc_release_scalar(&polymesh->child_bounds);
    // These two properties are not supported for now
    //abc_release_scalar(&polymesh->arb_geom_params);
    //abc_release_scalar(&polymesh->user_props);

    // PolyMesh_v1 properties
    abc_release_array(&polymesh->positions);
    abc_release_array(&polymesh->normals);
    abc_release_array(&polymesh->uvs);
    abc_release_array(&polymesh->velocities);
    abc_release_array(&polymesh->face_indices);
    abc_release_array(&polymesh->face_counts);
}
