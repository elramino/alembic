#include "output.h"

void print_fheader(const struct ogw_fheader_t* fheader)
{
    printf("Magic Number: ");

    putc(fheader->magic_number[0], stdout);
    putc(fheader->magic_number[1], stdout);
    putc(fheader->magic_number[2], stdout);
    putc(fheader->magic_number[3], stdout);
    putc(fheader->magic_number[4], stdout);
    putc('\n', stdout);

    printf("Frozen byte: %#04x\n", fheader->frozen_byte);
    printf("Version: %d.%d\n", fheader->major, fheader->minor);
    printf("Root group offset: %" PRIu64 "\n", fheader->root_offset);
}

void print_scalar(const struct abc_scalar_t* scalar)
{
    puts("=================================================");
    puts("                     SCALAR                      ");
    puts("=================================================");
    printf("Data Type: %s\n", abc_dtype_strings[scalar->dtype]);
    puts("=================================================");

    for (uint64_t i = 0; i < scalar->num_samples; ++i)
    {
        switch(scalar->dtype)
        {
        case ABC_DTYPE_VECTOR3F:
            printf("(%f, %f, %f)\n", 
                    scalar->vector3f[i].x, 
                    scalar->vector3f[i].y, 
                    scalar->vector3f[i].z);
            break;
        case ABC_DTYPE_BOX3D:
            printf("(%f, %f, %f), (%f, %f, %f)\n", 
                    scalar->box3d[i].x0, 
                    scalar->box3d[i].y0, 
                    scalar->box3d[i].z0,
                    scalar->box3d[i].x1, 
                    scalar->box3d[i].y1, 
                    scalar->box3d[i].z1);
            break;
        case ABC_DTYPE_U32:
            printf("%"PRIu32"\n", scalar->u32[i]);
            break;
        case ABC_DTYPE_I32:
            printf("%"PRIi32"\n", scalar->u32[i]);
            break;
        default:
            puts("Output of actual value not supported yet");
            break;
        } 
    }
}

void print_group(const char* title, const struct ogw_group_t* group)
{
    uint64_t header_index    = ogw_header_index(group);
    uint64_t header_location = group->children[header_index];

    puts("=================================================");
    printf("Group node %s:\n", title);
    puts("=================================================");
    printf("Position in File: %"PRIu64"\n", ogwnode_getoffset(header_location));
    printf("Children:         %"PRIu64"\n", group->num_children);
    printf("Header Index:     %"PRIu64"\n\n", header_index);

    for (uint64_t i = 0; i < group->num_children; i++)
    {
        const uint64_t    child = group->children[i];
        enum ogwnode_type type  = ogwnode_gettype(child);
        printf("Child #%"PRIu64" [%s] Start %"PRIu64"\n", i, 
                (type == OGW_NODE_DATA) ?  "Data Node " : "Group Node",
                ogwnode_getoffset(child));
    }

    puts("=================================================");

}

void print_array(const struct abc_array_t* array, uint64_t sample_id)
{
    if (sample_id >= array->num_samples)
    {
        puts("Cannot print array sample, index out of bounds");
        return;
    }

    struct abc_array_sample_t* sample = &array->samples[sample_id];

    puts("=================================================");
    puts("                  ARRAY SAMPLE                    ");
    puts("=================================================");
    printf("Data Type:  %s\n",                  abc_dtype_strings[sample->dtype]);
    printf("Sample:     %"PRIu64"/%"PRIu64"\n", sample_id, array->num_samples);
    printf("Dimensions: %"PRIu64"\n",           sample->num_dims);
    puts("=================================================");

    if (sample->num_dims != 1)
    {
        puts("Supporting only one dimensional arrays for now");
        return;
    }

    uint64_t num_entries = sample->dims[0];

    for (uint64_t i = 0; i < num_entries; ++i)
    {
        switch(sample->dtype)
        {
        case ABC_DTYPE_VECTOR3F:
            printf("(%f, %f, %f)\n", 
                    sample->vector3f[i].x, 
                    sample->vector3f[i].y, 
                    sample->vector3f[i].z);
            break;
        case ABC_DTYPE_BOX3D:
            printf("(%f, %f, %f), (%f, %f, %f)\n", 
                    sample->box3d[i].x0, 
                    sample->box3d[i].y0, 
                    sample->box3d[i].z0,
                    sample->box3d[i].x1, 
                    sample->box3d[i].y1, 
                    sample->box3d[i].z1);
            break;
        case ABC_DTYPE_U32:
            printf("%"PRIu32"\n", sample->u32[i]);
            break;
        case ABC_DTYPE_I32:
            printf("%"PRIi32"\n", sample->u32[i]);
            break;
        default:
            puts("Output of actual value not supported yet");
            break;
        } 
    }
}

void print_data(const char* title, const struct ogw_data_t* data)
{
    puts("=================================================");
    printf("Data node %s:\n", title);
    puts("=================================================");

    puts("Content as string:");
    for (uint64_t i = 0; i < data->num_bytes; i++)
    {
        putc(data->bytes[i], stdout);
    }
    putc('\n', stdout);

    puts("=================================================");

}


void print_ohdr(uint64_t header_index, uint64_t header_location,
        const struct ogw_data_t* data, const struct ogw_ohdr_t* header)
{
    puts("=================================================");
    puts("Object header node:");
    puts("=================================================");
    printf("Position in File: %"PRIu64"\n", ogwnode_getoffset(header_location));
    printf("Total Size:       %"PRIu64"bytes\n\n", data->num_bytes);
 
    puts("Chunk Data:");
    const uint64_t max_print = 16 * 15;
    for (uint64_t i = 0; i < data->num_bytes && i < max_print; ++i)
    {
        if (i > 0 && i % 16 == 0)
        {
            putc('\n', stdout);
        }
        else if (i > 0 && i % 8 == 0)
        {
            putc(' ', stdout);
        }

        printf("%02x ", (unsigned char) data->bytes[i]);
    }
    puts("\n");

    if (data->num_bytes >= max_print)
    {
        puts("...");
    }

    puts("Parsed info");
    puts("-------------------------------------------------");
    //printf("Name size:     %"PRIu32" bytes\n", header->name_size);

    for (uint64_t i = 0; i < header->num_names; ++i)
    {
        putc('\n', stdout);
        printf("Object #%"PRIu64" name:   %s\n", i, header->names[i]); 

        if (header->md_index == 0xff)
        {
            puts("Inline Metadata"); 
        }
        else
        {
            printf("Metadata index: %d\n", header->md_index); 
        }
    }
    puts("=================================================");
}

void print_phdr(uint64_t header_index, uint64_t header_location,
        const struct ogw_data_t* data, const struct ogw_phdr_t* header)
{
    puts("=================================================");
    puts("Prop header node:");
    puts("=================================================");
    printf("Position in File: %"PRIu64"\n", ogwnode_getoffset(header_location));
    printf("Total Size:       %"PRIu64"bytes\n\n", data->num_bytes);
 
    puts("Chunk Data:");
    const uint64_t max_print = 16 * 15;
    for (uint64_t i = 0; i < data->num_bytes && i < max_print; ++i)
    {
        if (i > 0 && i % 16 == 0)
        {
            putc('\n', stdout);
        }
        else if (i > 0 && i % 8 == 0)
        {
            putc(' ', stdout);
        }

        printf("%02x ", (unsigned char) data->bytes[i]);
    }
    puts("\n");

    if (data->num_bytes >= max_print)
    {
        puts("...");
    }

    puts("Parsed info");
    puts("-------------------------------------------------");

    for (uint64_t i = 0; i < header->num_props; ++i)
    {
        struct ogw_prop_t* prop = &header->props[i];

        printf("\nProperty #%"PRIu64", Name: %s\n", i, prop->name);

        switch (prop->info.prop_class)
        {
        case ABC_PROPCLASS_COMPOUND: puts("Property Class:     Compound"); break;
        case ABC_PROPCLASS_SCALAR:   puts("Property Class:     Scalar"); break;
        case ABC_PROPCLASS_ARRAY:    puts("Property Class:     Array"); break;
        default: puts("Property data type:  Invalid"); break;
        }

        switch (prop->info.int_size)
        {
        case ABC_PROPINFO_INTTYPE_U8:  puts("Integer Size:          8bit"); break;
        case ABC_PROPINFO_INTTYPE_U16: puts("Integer Size:         16bit"); break;
        case ABC_PROPINFO_INTTYPE_U32: puts("Integer Size:         32bit"); break;
        default: puts("Property data type:  Invalid"); break;
        }

        switch (prop->info.pod)
        {
        case ABC_POD_BOOL:  puts("Property POD Type:    bool"); break;
        case ABC_POD_U8:    puts("Property POD Type:   uint8"); break;
        case ABC_POD_I8:    puts("Property POD Type:    int8"); break;
        case ABC_POD_U16:   puts("Property POD Type:  uint16"); break;
        case ABC_POD_I16:   puts("Property POD Type:   int16"); break;
        case ABC_POD_U32:   puts("Property POD Type:  uint32"); break;
        case ABC_POD_I32:   puts("Property POD Type:   int32"); break;
        case ABC_POD_U64:   puts("Property POD Type:  uint64"); break;
        case ABC_POD_I64:   puts("Property POD Type:   int64"); break;
        case ABC_POD_F16:   puts("Property POD Type: float16"); break;
        case ABC_POD_F32:   puts("Property POD Type: float32"); break;
        case ABC_POD_F64:   puts("Property POD Type: float64"); break;
        case ABC_POD_U8STR: puts("Property POD Type:   u8str"); break;
        case ABC_POD_WSTR:  puts("Property POD Type: wstring"); break;
        }

        if (prop->info.md_index == 0xff)
        {
            puts("Inline Metadata"); 
        }
        else
        {
            printf("Metadata index: %d\n", prop->info.md_index); 
        }

        printf("Extent: %d\n", prop->info.extent);
        printf("First Changed Index:    %d\n", prop->first_changed_index);
        printf("Last Changed Index:     %d\n", prop->last_changed_index);
        printf("Next Sample Index:      %d\n", prop->next_sample_index);
    }

    puts("=================================================");
}

void print_proplist(const struct abc_proplist_t* proplist)
{
    puts("=================================================");
    puts("Properties:");
    puts("=================================================");

    printf("Schema: %s\n", schema_strings[proplist->schema]);
    printf("SchemaObjTitle: %s\n", proplist->sotitle);
    printf("SchemaBaseType: %s\n", schema_strings[proplist->sbtype]);

    printf("Num Properties: %"PRIu64"\n", proplist->num_props);
    for (uint64_t i = 0; i < proplist->num_props; ++i)
    {
        struct abc_prop_t* prop = &proplist->props[i];
        printf("%s\t%s\n", prop->name, propclass_strings[prop->prop_class]);        
    }

    puts("=================================================");
}

int print_md(const char* title, const struct abc_metadata_t* md)
{
    puts("=================================================");
    printf("Metadata (%s):\n", title);
    printf("Num Entries: %"PRIu64"\n", md->num_entries);

    for (uint64_t i = 0; i < md->num_entries; ++i)
    {
        struct abc_mdentry_t* entry = &md->entries[i];

        puts("-------------------------------------------------");
        printf("Entry #%"PRIu64":\n", i);
        puts("-------------------------------------------------");

        for (uint64_t j = 0; j < entry->num_pairs; ++j)
        {
            printf("%s: %s\n", entry->keys[j], entry->values[j]);
        } 

        putc('\n', stdout);
    }
    puts("=================================================");

    return 0;
}
