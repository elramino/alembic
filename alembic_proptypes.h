
#ifndef ABC_ALEMBIC_PROPTYPES_H
#define ABC_ALEMBIC_PROPTYPES_H

#include <stddef.h>
#include <inttypes.h>
#include <assert.h>
#include <stdbool.h>

#include "utils.h"

enum abc_prop_class
{
    ABC_PROPCLASS_COMPOUND = 0,
    ABC_PROPCLASS_SCALAR   = 1,
    ABC_PROPCLASS_ARRAY    = 2
};
#define ABC_NUM_PROPCLASSES 3

static const char* propclass_strings[] = 
{
    /* ABC_PROPTYPE_COMPOUND */ "compound",
    /* ABC_PROPTYPE_SCALAR   */ "scalar",
    /* ABC_PROPTYPE_ARRAY    */ "array",
};
static_assert(ABC_NUM_PROPCLASSES == countof(propclass_strings), 
        "Prop class string translation array does not match \"abc_pod\" enum");

enum abc_prop_inttype
{
   ABC_PROPINFO_INTTYPE_U8, 
   ABC_PROPINFO_INTTYPE_U16, 
   ABC_PROPINFO_INTTYPE_U32 
};

enum abc_pod
{
    ABC_POD_BOOL   =  0,
    ABC_POD_U8     =  1,
    ABC_POD_I8     =  2,
    ABC_POD_U16    =  3,
    ABC_POD_I16    =  4,
    ABC_POD_U32    =  5,
    ABC_POD_I32    =  6,
    ABC_POD_U64    =  7,
    ABC_POD_I64    =  8,
    ABC_POD_F16    =  9,
    ABC_POD_F32    = 10,
    ABC_POD_F64    = 11,
    ABC_POD_U8STR  = 12,
    ABC_POD_WSTR   = 13,
};
#define ABC_NUM_PODS 14

static const char* abc_pod_strings[] = 
{
    /* ABC_PROPDATATYPE_BOOL  */ "bool",
    /* ABC_PROPDATATYPE_U8    */ "uint8",
    /* ABC_PROPDATATYPE_I8    */ "int8",
    /* ABC_PROPDATATYPE_U16   */ "uint16",
    /* ABC_PROPDATATYPE_I16   */ "int16",
    /* ABC_PROPDATATYPE_U32   */ "uint32",
    /* ABC_PROPDATATYPE_I32   */ "int32",
    /* ABC_PROPDATATYPE_U64   */ "uint64",
    /* ABC_PROPDATATYPE_I64   */ "int64",
    /* ABC_PROPDATATYPE_F16   */ "float16",
    /* ABC_PROPDATATYPE_F32   */ "float32",
    /* ABC_PROPDATATYPE_F64   */ "float64",
    /* ABC_PROPDATATYPE_U8STR */ "u8_string",
    /* ABC_PROPDATATYPE_WSTR  */ "wstring"
};
static_assert(ABC_NUM_PODS == countof(abc_pod_strings), 
        "POD string translation array does not match \"abc_pod\" enum");

static const size_t abc_pod_bytes[] = 
{
    /* ABC_PROPDATATYPE_BOOL   */ 1,
    /* ABC_PROPDATATYPE_U8     */ 1,
    /* ABC_PROPDATATYPE_I8     */ 1,
    /* ABC_PROPDATATYPE_U16    */ 2,
    /* ABC_PROPDATATYPE_I16    */ 2,
    /* ABC_PROPDATATYPE_U32    */ 4,
    /* ABC_PROPDATATYPE_I32    */ 4,
    /* ABC_PROPDATATYPE_U64    */ 8,
    /* ABC_PROPDATATYPE_I64    */ 8,
    /* ABC_PROPDATATYPE_F16    */ 2,
    /* ABC_PROPDATATYPE_F32    */ 4,
    /* ABC_PROPDATATYPE_F64    */ 8,
    /* ABC_PROPDATATYPE_U8STR  */ sizeof(void*),
    /* ABC_PROPDATATYPE_WSTR   */ sizeof(void*)
};
static_assert(ABC_NUM_PODS == countof(abc_pod_bytes), 
        "POD byte size array does not match \"abc_pod\" enum");

enum abc_schema
{
    ABC_SCHEMA_POLYMESHV1 = 0,
    ABC_SCHEMA_GEOMBASEV1 = 1
};
#define ABC_NUM_SCHEMAS 2

static const char* schema_strings[] =
{
    /* [ABC_SCHEMA_POLYMESHV1]*/ "AbcGeom_PolyMesh_v1",
    /* [ABC_SCHEMA_GEOMBASEV1]*/ "AbcGeom_GeomBase_v1"
};
static_assert(ABC_NUM_SCHEMAS == countof(schema_strings), 
        "Schema string translation table size doesn't match number of "
        "elements in \"schema_strings\"");

struct vector2c_t
{
    uint8_t x;
    uint8_t y;
};

struct vector2s_t
{
    int16_t x;
    int16_t y;
};

struct vector2i_t
{
    int32_t x;
    int32_t y;
};

struct vector2h_t
{
    // Using uints for storage
    uint16_t x;
    uint16_t y;
};

struct vector2f_t
{
    float x;
    float y;
};

struct vector2d_t
{
    double x;
    double y;
};

struct vector3c_t
{
    uint8_t x;
    uint8_t y;
    uint8_t z;
};

struct vector3s_t
{
    int16_t x;
    int16_t y;
    int16_t z;
};

struct vector3i_t
{
    int32_t x;
    int32_t y;
    int32_t z;
};

struct vector3h_t
{
    // Using uints for storage
    int16_t x;
    int16_t y;
    int16_t z;
};

struct vector3f_t
{
    float x;
    float y;
    float z;
};
struct vector3d_t
{
    double x;
    double y;
    double z;
};

struct vector4c_t
{
    int8_t x;
    int8_t y;
    int8_t z;
    int8_t w;
};

struct vector4h_t
{
    // Using uints for storage
    int16_t x;
    int16_t y;
    int16_t z;
    int16_t w;
};

struct vector4f_t
{
    float x;
    float y;
    float z;
    float w;
};

struct box2s_t
{
    int16_t x0;
    int16_t y0;

    int16_t x1;
    int16_t y1;
};

struct box2i_t
{
    int32_t x0;
    int32_t y0;

    int32_t x1;
    int32_t y1;
};

struct box2f_t
{
    float x0;
    float y0;

    float x1;
    float y1;
};

struct box2d_t
{
    double x0;
    double y0;

    double x1;
    double y1;
};

struct box3s_t
{
    int16_t x0;
    int16_t y0;
    int16_t z0;

    int16_t x1;
    int16_t y1;
    int16_t z1;
};

struct box3i_t
{
    int32_t x0;
    int32_t y0;
    int32_t z0;

    int32_t x1;
    int32_t y1;
    int32_t z1;
};

struct box3f_t
{
    float x0;
    float y0;
    float z0;

    float x1;
    float y1;
    float z1;
};

struct box3d_t
{
    double x0;
    double y0;
    double z0;

    double x1;
    double y1;
    double z1;
};

struct matrix3x3f_t
{
    float values[3][3];
};

struct matrix3x3d_t
{
    double values[3][3];
};

struct matrix4x4f_t
{
    float values[4][4];
};

struct matrix4x4d_t
{
    double values[4][4];
};

struct quatf_t
{
    float x;
    float y;
    float z;
    float w;
};

struct quatd_t
{
    double x;
    double y;
    double z;
    double w;
};

struct abc_dtype_def_t
{
    enum abc_pod pod;
    size_t       extent;
};

enum abc_dtype
{
    ABC_DTYPE_BOOL     =  0,
    ABC_DTYPE_U8       =  1,
    ABC_DTYPE_U16      =  2,
    ABC_DTYPE_U32      =  3,
    ABC_DTYPE_U64      =  4,
    ABC_DTYPE_I8       =  5,
    ABC_DTYPE_I16      =  6,
    ABC_DTYPE_I32      =  7,
    ABC_DTYPE_I64      =  8,
    ABC_DTYPE_F16      =  9,
    ABC_DTYPE_F32      = 10,
    ABC_DTYPE_F64      = 11,
    ABC_DTYPE_U8STR    = 12,
    ABC_DTYPE_WSTR     = 13,
    ABC_DTYPE_VECTOR2C = 14,
    ABC_DTYPE_VECTOR2S = 15,
    ABC_DTYPE_VECTOR2I = 16,
    ABC_DTYPE_VECTOR2H = 17,
    ABC_DTYPE_VECTOR2F = 18,
    ABC_DTYPE_VECTOR2D = 19,
    ABC_DTYPE_VECTOR3C = 20,
    ABC_DTYPE_VECTOR3S = 21,
    ABC_DTYPE_VECTOR3I = 22,
    ABC_DTYPE_VECTOR3H = 23,
    ABC_DTYPE_VECTOR3F = 24,
    ABC_DTYPE_VECTOR3D = 25,
    ABC_DTYPE_VECTOR4C = 26,
    ABC_DTYPE_VECTOR4H = 27,
    ABC_DTYPE_VECTOR4F = 28,
    ABC_DTYPE_BOX2S    = 39,
    ABC_DTYPE_BOX2I    = 30,
    ABC_DTYPE_BOX2F    = 31,
    ABC_DTYPE_BOX2D    = 32,
    ABC_DTYPE_BOX3S    = 33,
    ABC_DTYPE_BOX3I    = 34,
    ABC_DTYPE_BOX3F    = 35,
    ABC_DTYPE_BOX3D    = 36,
    ABC_DTYPE_MTX33F   = 37,
    ABC_DTYPE_MTX33D   = 38,
    ABC_DTYPE_MTX44F   = 49,
    ABC_DTYPE_MTX44D   = 40,
    ABC_DTYPE_QUATF    = 41,
    ABC_DTYPE_QUATD    = 42,
};
#define ABC_NUM_DTYPES 43

static const struct abc_dtype_def_t abc_dtype_def[] = 
{
    /* ABC_DTYPE_BOOL     */ { ABC_POD_BOOL,  1 },
    /* ABC_DTYPE_U8       */ { ABC_POD_U8,    1 },
    /* ABC_DTYPE_U16      */ { ABC_POD_U16,   1 },
    /* ABC_DTYPE_U32      */ { ABC_POD_U32,   1 },
    /* ABC_DTYPE_U64      */ { ABC_POD_U64,   1 },
    /* ABC_DTYPE_I8       */ { ABC_POD_I8,    1 },
    /* ABC_DTYPE_I16      */ { ABC_POD_I16,   1 },
    /* ABC_DTYPE_I32      */ { ABC_POD_I32,   1 },
    /* ABC_DTYPE_I64      */ { ABC_POD_I64,   1 },
    /* ABC_DTYPE_F16      */ { ABC_POD_F16,   1 },
    /* ABC_DTYPE_F32      */ { ABC_POD_F32,   1 },
    /* ABC_DTYPE_F64      */ { ABC_POD_U8,    1 },
    /* ABC_DTYPE_U8STR    */ { ABC_POD_U8STR, 0 }, // Need to ignore extent for strings
    /* ABC_DTYPE_WSTR     */ { ABC_POD_WSTR,  0 }, // Need to ignore extent for strings
    /* ABC_DTYPE_VECTOR2C */ { ABC_POD_U8,    2 },
    /* ABC_DTYPE_VECTOR2S */ { ABC_POD_I16,   2 },
    /* ABC_DTYPE_VECTOR2I */ { ABC_POD_I32,   2 },
    /* ABC_DTYPE_VECTOR2H */ { ABC_POD_F16,   2 },
    /* ABC_DTYPE_VECTOR2F */ { ABC_POD_F32,   2 },
    /* ABC_DTYPE_VECTOR2D */ { ABC_POD_F64,   2 },
    /* ABC_DTYPE_VECTOR3C */ { ABC_POD_U8,    3 },
    /* ABC_DTYPE_VECTOR3S */ { ABC_POD_I16,   3 },
    /* ABC_DTYPE_VECTOR3I */ { ABC_POD_I32,   3 },
    /* ABC_DTYPE_VECTOR3H */ { ABC_POD_F16,   3 },
    /* ABC_DTYPE_VECTOR3F */ { ABC_POD_F32,   3 },
    /* ABC_DTYPE_VECTOR3D */ { ABC_POD_F64,   3 },
    /* ABC_DTYPE_VECTOR4C */ { ABC_POD_U8,    4 },
    /* ABC_DTYPE_VECTOR4H */ { ABC_POD_F16,   4 },
    /* ABC_DTYPE_VECTOR4F */ { ABC_POD_F32,   4 },
    /* ABC_DTYPE_BOX2S    */ { ABC_POD_I16,   4 },
    /* ABC_DTYPE_BOX2I    */ { ABC_POD_I32,   4 },
    /* ABC_DTYPE_BOX2F    */ { ABC_POD_F32,   4 },
    /* ABC_DTYPE_BOX2D    */ { ABC_POD_F64,   4 },
    /* ABC_DTYPE_BOX3S    */ { ABC_POD_I16,   6 },
    /* ABC_DTYPE_BOX3I    */ { ABC_POD_I32,   6 },
    /* ABC_DTYPE_BOX3F    */ { ABC_POD_F32,   6 },
    /* ABC_DTYPE_BOX3D    */ { ABC_POD_F64,   6 },
    /* ABC_DTYPE_MTX33F   */ { ABC_POD_F32,   9 },
    /* ABC_DTYPE_MTX33D   */ { ABC_POD_F64,   9 },
    /* ABC_DTYPE_MTX44F   */ { ABC_POD_F32,  16 },
    /* ABC_DTYPE_MTX44D   */ { ABC_POD_F64,  16 },
    /* ABC_DTYPE_QUATF    */ { ABC_POD_F32,   4 },
    /* ABC_DTYPE_QUATD    */ { ABC_POD_F64,   4 }
};
static_assert(ABC_NUM_DTYPES == countof(abc_dtype_def), 
        "Prop data type definition table does not match \"abc_dtype\" enum");

static const size_t abc_dtype_sizes[] = 
{
    /* ABC_DTYPE_BOOL     */ 1 *  1,
    /* ABC_DTYPE_U8       */ 1 *  1,
    /* ABC_DTYPE_U16      */ 2 *  1,
    /* ABC_DTYPE_U32      */ 4 *  1,
    /* ABC_DTYPE_U64      */ 8 *  1,
    /* ABC_DTYPE_I8       */ 1 *  1,
    /* ABC_DTYPE_I16      */ 2 *  1,
    /* ABC_DTYPE_I32      */ 4 *  1,
    /* ABC_DTYPE_I64      */ 8 *  1,
    /* ABC_DTYPE_F16      */ 2 *  1,
    /* ABC_DTYPE_F32      */ 4 *  1,
    /* ABC_DTYPE_F64      */ 1 *  1,
    /* ABC_DTYPE_U8STR    */ 0,      // Special, size is dynamic
    /* ABC_DTYPE_WSTR     */ 0,      // Special, size is dynamic
    /* ABC_DTYPE_VECTOR2C */ 1 *  2,
    /* ABC_DTYPE_VECTOR2S */ 2 *  2,
    /* ABC_DTYPE_VECTOR2I */ 4 *  2,
    /* ABC_DTYPE_VECTOR2H */ 2 *  2,
    /* ABC_DTYPE_VECTOR2F */ 4 *  2,
    /* ABC_DTYPE_VECTOR2D */ 8 *  2,
    /* ABC_DTYPE_VECTOR3C */ 1 *  3,
    /* ABC_DTYPE_VECTOR3S */ 2 *  3,
    /* ABC_DTYPE_VECTOR3I */ 4 *  3,
    /* ABC_DTYPE_VECTOR3H */ 2 *  3,
    /* ABC_DTYPE_VECTOR3F */ 4 *  3,
    /* ABC_DTYPE_VECTOR3D */ 8 *  3,
    /* ABC_DTYPE_VECTOR4C */ 1 *  4,
    /* ABC_DTYPE_VECTOR4H */ 2 *  4,
    /* ABC_DTYPE_VECTOR4F */ 4 *  4,
    /* ABC_DTYPE_BOX2S    */ 2 *  4,
    /* ABC_DTYPE_BOX2I    */ 4 *  4,
    /* ABC_DTYPE_BOX2F    */ 4 *  4,
    /* ABC_DTYPE_BOX2D    */ 8 *  4,
    /* ABC_DTYPE_BOX3S    */ 2 *  6,
    /* ABC_DTYPE_BOX3I    */ 4 *  6,
    /* ABC_DTYPE_BOX3F    */ 4 *  6,
    /* ABC_DTYPE_BOX3D    */ 8 *  6,
    /* ABC_DTYPE_MTX33F   */ 4 *  9,
    /* ABC_DTYPE_MTX33D   */ 8 *  9,
    /* ABC_DTYPE_MTX44F   */ 4 * 16,
    /* ABC_DTYPE_MTX44D   */ 8 * 16,
    /* ABC_DTYPE_QUATF    */ 4 *  4,
    /* ABC_DTYPE_QUATD    */ 8 *  4,
};
static_assert(ABC_NUM_DTYPES == countof(abc_dtype_sizes), 
        "Prop data type byte size array does not match \"abc_dtype\" enum");

static const char* abc_dtype_strings[] =
{
    /* ABC_DTYPE_BOOL     */ "bool",
    /* ABC_DTYPE_U8       */ "uint8",
    /* ABC_DTYPE_U16      */ "uint16",
    /* ABC_DTYPE_U32      */ "uint32",
    /* ABC_DTYPE_U64      */ "uint64",
    /* ABC_DTYPE_I8       */ "int8",
    /* ABC_DTYPE_I16      */ "int16",
    /* ABC_DTYPE_I32      */ "int32",
    /* ABC_DTYPE_I64      */ "int64",
    /* ABC_DTYPE_F16      */ "float16",
    /* ABC_DTYPE_F32      */ "float32",
    /* ABC_DTYPE_F64      */ "float64",
    /* ABC_DTYPE_U8STR    */ "u8str",
    /* ABC_DTYPE_WSTR     */ "wstr",
    /* ABC_DTYPE_VECTOR2C */ "vector2c",
    /* ABC_DTYPE_VECTOR2S */ "vector2s",
    /* ABC_DTYPE_VECTOR2I */ "vector2i",
    /* ABC_DTYPE_VECTOR2H */ "vector2h",
    /* ABC_DTYPE_VECTOR2F */ "vector2f",
    /* ABC_DTYPE_VECTOR2D */ "vector2d",
    /* ABC_DTYPE_VECTOR3C */ "vector3c",
    /* ABC_DTYPE_VECTOR3S */ "vector3s",
    /* ABC_DTYPE_VECTOR3I */ "vector3i",
    /* ABC_DTYPE_VECTOR3H */ "vector3h",
    /* ABC_DTYPE_VECTOR3F */ "vector3f",
    /* ABC_DTYPE_VECTOR3D */ "vector3d",
    /* ABC_DTYPE_VECTOR4C */ "vector4c",
    /* ABC_DTYPE_VECTOR4H */ "vector4h",
    /* ABC_DTYPE_VECTOR4F */ "vector4f",
    /* ABC_DTYPE_BOX2S    */ "box2s",
    /* ABC_DTYPE_BOX2I    */ "box2i",
    /* ABC_DTYPE_BOX2F    */ "box2f",
    /* ABC_DTYPE_BOX2D    */ "box2d",
    /* ABC_DTYPE_BOX3S    */ "box3s",
    /* ABC_DTYPE_BOX3I    */ "box3i",
    /* ABC_DTYPE_BOX3F    */ "box3f",
    /* ABC_DTYPE_BOX3D    */ "box3d",
    /* ABC_DTYPE_MTX33F   */ "matrix3x3f",
    /* ABC_DTYPE_MTX33D   */ "matrix3x3d",
    /* ABC_DTYPE_MTX44F   */ "matrix4x4f",
    /* ABC_DTYPE_MTX44D   */ "matrix4x4d",
    /* ABC_DTYPE_QUATF    */ "quatf",
    /* ABC_DTYPE_QUATD    */ "quatd"
};
static_assert(ABC_NUM_DTYPES == countof(abc_dtype_strings), 
        "Prop string translation array does not match \"abc_dtype\" enum");

struct abc_array_sample_t
{
    enum abc_dtype dtype;
    uint64_t*      dims;
    uint64_t       num_dims;

    union
    {
        bool*                boolean;
        uint8_t*             u8;
        uint16_t*            u16;
        uint32_t*            u32;
        uint64_t*            u64;
        int8_t*              i8;
        int16_t*             i16;
        int32_t*             i32;
        int64_t*             i64;
        uint16_t*            f16; // Using uint16_t for storage
        float*               f32;
        double*              f64;
        char*                u8str;
        char*                wstr;
        struct vector2c_t*   vector2c;
        struct vector2s_t*   vector2s;
        struct vector2i_t*   vector2i;
        struct vector2h_t*   vector2h;
        struct vector2f_t*   vector2f;
        struct vector2d_t*   vector2d;
        struct vector3c_t*   vector3c;
        struct vector3s_t*   vector3s;
        struct vector3i_t*   vector3i;
        struct vector3h_t*   vector3h;
        struct vector3f_t*   vector3f;
        struct vector3d_t*   vector3d;
        struct vector4c_t*   vector4c;
        struct vector4h_t*   vector4h;
        struct vector4f_t*   vector4f;
        struct box2s_t*      box2s;
        struct box2i_t*      box2i;
        struct box2f_t*      box2f;
        struct box2d_t*      box2d;
        struct box3s_t*      box3s;
        struct box3i_t*      box3i;
        struct box3f_t*      box3f;
        struct box3d_t*      box3d;
        struct matrix3x3f_t* matrix3x3f;
        struct matrix3x3d_t* matrix3x3d;
        struct matrix4x4f_t* matrix4x4f;
        struct matrix4x4d_t* matrix4x4d;
        struct quatf_t*      quatf;
        struct quatd_t*      quatd;
    };
};

struct abc_array_t
{
    uint64_t                   num_samples;
    struct abc_array_sample_t* samples;
};

struct abc_scalar_t
{
    enum abc_dtype dtype;
    uint64_t       num_samples;

    union
    {
        bool*                boolean;
        uint8_t*             u8;
        uint16_t*            u16;
        uint32_t*            u32;
        uint64_t*            u64;
        int8_t*              i8;
        int16_t*             i16;
        int32_t*             i32;
        int64_t*             i64;
        uint16_t*            f16; // Using uint16_t for storage
        float*               f32;
        double*              f64;
        char*                u8str;
        char*                wstr;
        struct vector2c_t*   vector2c;
        struct vector2s_t*   vector2s;
        struct vector2i_t*   vector2i;
        struct vector2h_t*   vector2h;
        struct vector2f_t*   vector2f;
        struct vector2d_t*   vector2d;
        struct vector3c_t*   vector3c;
        struct vector3s_t*   vector3s;
        struct vector3i_t*   vector3i;
        struct vector3h_t*   vector3h;
        struct vector3f_t*   vector3f;
        struct vector3d_t*   vector3d;
        struct vector4c_t*   vector4c;
        struct vector4h_t*   vector4h;
        struct vector4f_t*   vector4f;
        struct box2s_t*      box2s;
        struct box2i_t*      box2i;
        struct box2f_t*      box2f;
        struct box2d_t*      box2d;
        struct box3s_t*      box3s;
        struct box3i_t*      box3i;
        struct box3f_t*      box3f;
        struct box3d_t*      box3d;
        struct matrix3x3f_t* matrix3x3f;
        struct matrix3x3d_t* matrix3x3d;
        struct matrix4x4f_t* matrix4x4f;
        struct matrix4x4d_t* matrix4x4d;
        struct quatf_t*      quatf;
        struct quatd_t*      quatd;
    };
};

void* abc_alloc_scalar(struct abc_scalar_t* scalar, size_t size);
void  abc_free_scalar(struct abc_scalar_t* scalar);
void* abc_alloc_array_sample(struct abc_array_sample_t* sample, size_t size);
void  abc_free_array_sample(struct abc_array_sample_t* sample);

#endif
