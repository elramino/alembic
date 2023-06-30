#include <stdlib.h>

#include "alembic_proptypes.h"

void* abc_alloc_scalar(struct abc_scalar_t* s, size_t size)
{
    switch(s->dtype)
    {
    case ABC_DTYPE_BOOL:     s->boolean    = malloc(size); return (char*) s->boolean;
    case ABC_DTYPE_U8:       s->u8         = malloc(size); return (char*) s->u8;
    case ABC_DTYPE_U16:      s->u16        = malloc(size); return (char*) s->u16;
    case ABC_DTYPE_U32:      s->u32        = malloc(size); return (char*) s->u32;
    case ABC_DTYPE_U64:      s->u64        = malloc(size); return (char*) s->u64;
    case ABC_DTYPE_I8:       s->i8         = malloc(size); return (char*) s->i8;
    case ABC_DTYPE_I16:      s->i16        = malloc(size); return (char*) s->i16;
    case ABC_DTYPE_I32:      s->i32        = malloc(size); return (char*) s->i32;
    case ABC_DTYPE_I64:      s->i64        = malloc(size); return (char*) s->i64;
    case ABC_DTYPE_F16:      s->f16        = malloc(size); return (char*) s->f16;
    case ABC_DTYPE_F32:      s->f32        = malloc(size); return (char*) s->f32;
    case ABC_DTYPE_F64:      s->f64        = malloc(size); return (char*) s->f64;
    case ABC_DTYPE_U8STR:    s->u8str      = malloc(size); return (char*) s->u8str;
    case ABC_DTYPE_WSTR:     s->wstr       = malloc(size); return (char*) s->wstr;
    case ABC_DTYPE_VECTOR2C: s->vector2c   = malloc(size); return (char*) s->vector2c;
    case ABC_DTYPE_VECTOR2S: s->vector2s   = malloc(size); return (char*) s->vector2s;
    case ABC_DTYPE_VECTOR2I: s->vector2i   = malloc(size); return (char*) s->vector2i;
    case ABC_DTYPE_VECTOR2H: s->vector2h   = malloc(size); return (char*) s->vector2h;
    case ABC_DTYPE_VECTOR2F: s->vector2f   = malloc(size); return (char*) s->vector2f;
    case ABC_DTYPE_VECTOR2D: s->vector2d   = malloc(size); return (char*) s->vector2d;
    case ABC_DTYPE_VECTOR3C: s->vector3c   = malloc(size); return (char*) s->vector3c;
    case ABC_DTYPE_VECTOR3S: s->vector3s   = malloc(size); return (char*) s->vector3s;
    case ABC_DTYPE_VECTOR3I: s->vector3i   = malloc(size); return (char*) s->vector3i;
    case ABC_DTYPE_VECTOR3H: s->vector3h   = malloc(size); return (char*) s->vector3h;
    case ABC_DTYPE_VECTOR3F: s->vector3f   = malloc(size); return (char*) s->vector3f;
    case ABC_DTYPE_VECTOR3D: s->vector3d   = malloc(size); return (char*) s->vector3d;
    case ABC_DTYPE_VECTOR4C: s->vector4c   = malloc(size); return (char*) s->vector4c;
    case ABC_DTYPE_VECTOR4H: s->vector4h   = malloc(size); return (char*) s->vector4h;
    case ABC_DTYPE_VECTOR4F: s->vector4f   = malloc(size); return (char*) s->vector4f;
    case ABC_DTYPE_BOX2S:    s->box2s      = malloc(size); return (char*) s->box2s;
    case ABC_DTYPE_BOX2I:    s->box2i      = malloc(size); return (char*) s->box2i;
    case ABC_DTYPE_BOX2F:    s->box2f      = malloc(size); return (char*) s->box2f;
    case ABC_DTYPE_BOX2D:    s->box2d      = malloc(size); return (char*) s->box2d;
    case ABC_DTYPE_BOX3S:    s->box3s      = malloc(size); return (char*) s->box3s;
    case ABC_DTYPE_BOX3I:    s->box3i      = malloc(size); return (char*) s->box3i;
    case ABC_DTYPE_BOX3F:    s->box3f      = malloc(size); return (char*) s->box3f;
    case ABC_DTYPE_BOX3D:    s->box3d      = malloc(size); return (char*) s->box3d;
    case ABC_DTYPE_MTX33F:   s->matrix3x3f = malloc(size); return (char*) s->matrix3x3f;
    case ABC_DTYPE_MTX33D:   s->matrix3x3d = malloc(size); return (char*) s->matrix3x3d;
    case ABC_DTYPE_MTX44F:   s->matrix4x4f = malloc(size); return (char*) s->matrix4x4f;
    case ABC_DTYPE_MTX44D:   s->matrix4x4d = malloc(size); return (char*) s->matrix4x4d;
    case ABC_DTYPE_QUATF:    s->quatf      = malloc(size); return (char*) s->quatf;
    case ABC_DTYPE_QUATD:    s->quatd      = malloc(size); return (char*) s->quatd;
    }
}

void abc_free_scalar(struct abc_scalar_t* s)
{
    switch(s->dtype)
    {
    case ABC_DTYPE_BOOL:     free_invalidate(s->boolean);    break;
    case ABC_DTYPE_U8:       free_invalidate(s->u8);         break;
    case ABC_DTYPE_U16:      free_invalidate(s->u16);        break;
    case ABC_DTYPE_U32:      free_invalidate(s->u32);        break;
    case ABC_DTYPE_U64:      free_invalidate(s->u64);        break;
    case ABC_DTYPE_I8:       free_invalidate(s->i8);         break;
    case ABC_DTYPE_I16:      free_invalidate(s->i16);        break;
    case ABC_DTYPE_I32:      free_invalidate(s->i32);        break;
    case ABC_DTYPE_I64:      free_invalidate(s->i64);        break;
    case ABC_DTYPE_F16:      free_invalidate(s->f16);        break;
    case ABC_DTYPE_F32:      free_invalidate(s->f32);        break;
    case ABC_DTYPE_F64:      free_invalidate(s->f64);        break;
    case ABC_DTYPE_U8STR:    free_invalidate(s->u8str);      break;
    case ABC_DTYPE_WSTR:     free_invalidate(s->wstr);       break;
    case ABC_DTYPE_VECTOR2C: free_invalidate(s->vector2c);   break;
    case ABC_DTYPE_VECTOR2S: free_invalidate(s->vector2s);   break; 
    case ABC_DTYPE_VECTOR2I: free_invalidate(s->vector2i);   break;
    case ABC_DTYPE_VECTOR2H: free_invalidate(s->vector2h);   break;
    case ABC_DTYPE_VECTOR2F: free_invalidate(s->vector2f);   break;
    case ABC_DTYPE_VECTOR2D: free_invalidate(s->vector2d);   break;
    case ABC_DTYPE_VECTOR3C: free_invalidate(s->vector3c);   break;
    case ABC_DTYPE_VECTOR3S: free_invalidate(s->vector3s);   break;
    case ABC_DTYPE_VECTOR3I: free_invalidate(s->vector3i);   break;
    case ABC_DTYPE_VECTOR3H: free_invalidate(s->vector3h);   break;
    case ABC_DTYPE_VECTOR3F: free_invalidate(s->vector3f);   break;
    case ABC_DTYPE_VECTOR3D: free_invalidate(s->vector3d);   break;
    case ABC_DTYPE_VECTOR4C: free_invalidate(s->vector4c);   break;
    case ABC_DTYPE_VECTOR4H: free_invalidate(s->vector4h);   break;
    case ABC_DTYPE_VECTOR4F: free_invalidate(s->vector4f);   break;
    case ABC_DTYPE_BOX2S:    free_invalidate(s->box2s);      break;
    case ABC_DTYPE_BOX2I:    free_invalidate(s->box2i);      break;
    case ABC_DTYPE_BOX2F:    free_invalidate(s->box2f);      break;
    case ABC_DTYPE_BOX2D:    free_invalidate(s->box2d);      break;
    case ABC_DTYPE_BOX3S:    free_invalidate(s->box3s);      break;
    case ABC_DTYPE_BOX3I:    free_invalidate(s->box3i);      break;
    case ABC_DTYPE_BOX3F:    free_invalidate(s->box3f);      break;
    case ABC_DTYPE_BOX3D:    free_invalidate(s->box3d);      break;
    case ABC_DTYPE_MTX33F:   free_invalidate(s->matrix3x3f); break;
    case ABC_DTYPE_MTX33D:   free_invalidate(s->matrix3x3d); break;
    case ABC_DTYPE_MTX44F:   free_invalidate(s->matrix4x4f); break;
    case ABC_DTYPE_MTX44D:   free_invalidate(s->matrix4x4d); break;
    case ABC_DTYPE_QUATF:    free_invalidate(s->quatf);      break;
    case ABC_DTYPE_QUATD:    free_invalidate(s->quatd);      break;
    }
}

void* abc_alloc_array_sample(struct abc_array_sample_t* s, size_t size)
{
    switch(s->dtype)
    {
    case ABC_DTYPE_BOOL:     s->boolean    = malloc(size); return (char*) s->boolean;
    case ABC_DTYPE_U8:       s->u8         = malloc(size); return (char*) s->u8;
    case ABC_DTYPE_U16:      s->u16        = malloc(size); return (char*) s->u16;
    case ABC_DTYPE_U32:      s->u32        = malloc(size); return (char*) s->u32;
    case ABC_DTYPE_U64:      s->u64        = malloc(size); return (char*) s->u64;
    case ABC_DTYPE_I8:       s->i8         = malloc(size); return (char*) s->i8;
    case ABC_DTYPE_I16:      s->i16        = malloc(size); return (char*) s->i16;
    case ABC_DTYPE_I32:      s->i32        = malloc(size); return (char*) s->i32;
    case ABC_DTYPE_I64:      s->i64        = malloc(size); return (char*) s->i64;
    case ABC_DTYPE_F16:      s->f16        = malloc(size); return (char*) s->f16;
    case ABC_DTYPE_F32:      s->f32        = malloc(size); return (char*) s->f32;
    case ABC_DTYPE_F64:      s->f64        = malloc(size); return (char*) s->f64;
    case ABC_DTYPE_U8STR:    s->u8str      = malloc(size); return (char*) s->u8str;
    case ABC_DTYPE_WSTR:     s->wstr       = malloc(size); return (char*) s->wstr;
    case ABC_DTYPE_VECTOR2C: s->vector2c   = malloc(size); return (char*) s->vector2c;
    case ABC_DTYPE_VECTOR2S: s->vector2s   = malloc(size); return (char*) s->vector2s;
    case ABC_DTYPE_VECTOR2I: s->vector2i   = malloc(size); return (char*) s->vector2i;
    case ABC_DTYPE_VECTOR2H: s->vector2h   = malloc(size); return (char*) s->vector2h;
    case ABC_DTYPE_VECTOR2F: s->vector2f   = malloc(size); return (char*) s->vector2f;
    case ABC_DTYPE_VECTOR2D: s->vector2d   = malloc(size); return (char*) s->vector2d;
    case ABC_DTYPE_VECTOR3C: s->vector3c   = malloc(size); return (char*) s->vector3c;
    case ABC_DTYPE_VECTOR3S: s->vector3s   = malloc(size); return (char*) s->vector3s;
    case ABC_DTYPE_VECTOR3I: s->vector3i   = malloc(size); return (char*) s->vector3i;
    case ABC_DTYPE_VECTOR3H: s->vector3h   = malloc(size); return (char*) s->vector3h;
    case ABC_DTYPE_VECTOR3F: s->vector3f   = malloc(size); return (char*) s->vector3f;
    case ABC_DTYPE_VECTOR3D: s->vector3d   = malloc(size); return (char*) s->vector3d;
    case ABC_DTYPE_VECTOR4C: s->vector4c   = malloc(size); return (char*) s->vector4c;
    case ABC_DTYPE_VECTOR4H: s->vector4h   = malloc(size); return (char*) s->vector4h;
    case ABC_DTYPE_VECTOR4F: s->vector4f   = malloc(size); return (char*) s->vector4f;
    case ABC_DTYPE_BOX2S:    s->box2s      = malloc(size); return (char*) s->box2s;
    case ABC_DTYPE_BOX2I:    s->box2i      = malloc(size); return (char*) s->box2i;
    case ABC_DTYPE_BOX2F:    s->box2f      = malloc(size); return (char*) s->box2f;
    case ABC_DTYPE_BOX2D:    s->box2d      = malloc(size); return (char*) s->box2d;
    case ABC_DTYPE_BOX3S:    s->box3s      = malloc(size); return (char*) s->box3s;
    case ABC_DTYPE_BOX3I:    s->box3i      = malloc(size); return (char*) s->box3i;
    case ABC_DTYPE_BOX3F:    s->box3f      = malloc(size); return (char*) s->box3f;
    case ABC_DTYPE_BOX3D:    s->box3d      = malloc(size); return (char*) s->box3d;
    case ABC_DTYPE_MTX33F:   s->matrix3x3f = malloc(size); return (char*) s->matrix3x3f;
    case ABC_DTYPE_MTX33D:   s->matrix3x3d = malloc(size); return (char*) s->matrix3x3d;
    case ABC_DTYPE_MTX44F:   s->matrix4x4f = malloc(size); return (char*) s->matrix4x4f;
    case ABC_DTYPE_MTX44D:   s->matrix4x4d = malloc(size); return (char*) s->matrix4x4d;
    case ABC_DTYPE_QUATF:    s->quatf      = malloc(size); return (char*) s->quatf;
    case ABC_DTYPE_QUATD:    s->quatd      = malloc(size); return (char*) s->quatd;
    } 
}

void abc_free_array_sample(struct abc_array_sample_t* s)
{
    switch(s->dtype)
    {
    case ABC_DTYPE_BOOL:     free_invalidate(s->boolean);    break;
    case ABC_DTYPE_U8:       free_invalidate(s->u8);         break;
    case ABC_DTYPE_U16:      free_invalidate(s->u16);        break;
    case ABC_DTYPE_U32:      free_invalidate(s->u32);        break;
    case ABC_DTYPE_U64:      free_invalidate(s->u64);        break;
    case ABC_DTYPE_I8:       free_invalidate(s->i8);         break;
    case ABC_DTYPE_I16:      free_invalidate(s->i16);        break;
    case ABC_DTYPE_I32:      free_invalidate(s->i32);        break;
    case ABC_DTYPE_I64:      free_invalidate(s->i64);        break;
    case ABC_DTYPE_F16:      free_invalidate(s->f16);        break;
    case ABC_DTYPE_F32:      free_invalidate(s->f32);        break;
    case ABC_DTYPE_F64:      free_invalidate(s->f64);        break;
    case ABC_DTYPE_U8STR:    free_invalidate(s->u8str);      break;
    case ABC_DTYPE_WSTR:     free_invalidate(s->wstr);       break;
    case ABC_DTYPE_VECTOR2C: free_invalidate(s->vector2c);   break;
    case ABC_DTYPE_VECTOR2S: free_invalidate(s->vector2s);   break; 
    case ABC_DTYPE_VECTOR2I: free_invalidate(s->vector2i);   break;
    case ABC_DTYPE_VECTOR2H: free_invalidate(s->vector2h);   break;
    case ABC_DTYPE_VECTOR2F: free_invalidate(s->vector2f);   break;
    case ABC_DTYPE_VECTOR2D: free_invalidate(s->vector2d);   break;
    case ABC_DTYPE_VECTOR3C: free_invalidate(s->vector3c);   break;
    case ABC_DTYPE_VECTOR3S: free_invalidate(s->vector3s);   break;
    case ABC_DTYPE_VECTOR3I: free_invalidate(s->vector3i);   break;
    case ABC_DTYPE_VECTOR3H: free_invalidate(s->vector3h);   break;
    case ABC_DTYPE_VECTOR3F: free_invalidate(s->vector3f);   break;
    case ABC_DTYPE_VECTOR3D: free_invalidate(s->vector3d);   break;
    case ABC_DTYPE_VECTOR4C: free_invalidate(s->vector4c);   break;
    case ABC_DTYPE_VECTOR4H: free_invalidate(s->vector4h);   break;
    case ABC_DTYPE_VECTOR4F: free_invalidate(s->vector4f);   break;
    case ABC_DTYPE_BOX2S:    free_invalidate(s->box2s);      break;
    case ABC_DTYPE_BOX2I:    free_invalidate(s->box2i);      break;
    case ABC_DTYPE_BOX2F:    free_invalidate(s->box2f);      break;
    case ABC_DTYPE_BOX2D:    free_invalidate(s->box2d);      break;
    case ABC_DTYPE_BOX3S:    free_invalidate(s->box3s);      break;
    case ABC_DTYPE_BOX3I:    free_invalidate(s->box3i);      break;
    case ABC_DTYPE_BOX3F:    free_invalidate(s->box3f);      break;
    case ABC_DTYPE_BOX3D:    free_invalidate(s->box3d);      break;
    case ABC_DTYPE_MTX33F:   free_invalidate(s->matrix3x3f); break;
    case ABC_DTYPE_MTX33D:   free_invalidate(s->matrix3x3d); break;
    case ABC_DTYPE_MTX44F:   free_invalidate(s->matrix4x4f); break;
    case ABC_DTYPE_MTX44D:   free_invalidate(s->matrix4x4d); break;
    case ABC_DTYPE_QUATF:    free_invalidate(s->quatf);      break;
    case ABC_DTYPE_QUATD:    free_invalidate(s->quatd);      break;
    }
}
