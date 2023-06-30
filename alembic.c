#include "alembic.h"
#include "logging.h"
#include "utils.h"
#include "proputils.h"

// Each data node begins with 
// a 16 bytes hash called key
static const size_t KEY_BYTES  = 16;

static int abc_get_substrchild(const struct abc_archive_t* archive, 
        const struct ogw_group_t* parent, const char* str_start, 
        const char* str_end, struct ogw_group_t* out)
{
    size_t str_size = (size_t)str_end - (size_t)str_start;
    char*  str      = alloca(str_size + 1);
    str[str_size] = '\0';

    memcpy(str, str_start, str_size);

    return abc_get_child(archive, parent, str, out);
}

// TODO: Need to test reading dimensions, currently no test data available
static int abc_read_dimensions(FILE* f, uint64_t dims_loc, size_t dtype_size, 
        const struct ogw_data_t* val_data, struct abc_array_sample_t* out)
{
    assert(val_data != NULL);
    assert(out      != NULL);

    // Must compute 1D-array size from data size 
    // if there is no dimension node
    if (ogwnode_getoffset(dims_loc) == 0)
    {
        out->num_dims = 1;
        out->dims     = malloc(sizeof(uint64_t) * out->num_dims);
        out->dims[0]  = (val_data->num_bytes - KEY_BYTES) / dtype_size;
        return 0;
    }

    // There is a node with dimension data, 
    // process it and store dimensions
    struct ogw_data_t dims_data = { 0 };
    if (ogw_readdata(f, dims_loc, &dims_data) < 0)
    {
        log_error("Failed reading array sample dimension");
        return -1;
    }

    if (dims_data.num_bytes <= KEY_BYTES)
    {
        log_error("Couldn't read property array dimensions!");
        return -1;
    }       

    // Dimensions are stored as uint64_t values, 
    // check whether size is valid for data type
    const size_t dims_size = dims_data.num_bytes - KEY_BYTES;
    out->num_dims = (dims_size / 4);

    if (dims_size && 0x03u != 0)
    {
        log_error("Array dimensions couldn't be read, dimensions data "
                "node must have payload size which is a multiple of 4!");
        ogw_releasedata(&dims_data);
        return -1;
    }

    out->dims = malloc(dims_size);
    
    if (out->dims == NULL)
    {
        log_error("Failed allocating memory for dimension data");
        return -1;
    }

    ogw_releasedata(&dims_data);

    return 0;
}

int abc_open(const char* file_name, struct abc_archive_t* out)
{
    assert(out->fd == 0);

    out->fd = fopen(file_name, "rb");

    if (out->fd == NULL)
    {
        abc_close(out);
        log_error("Failed opening archive file \"%s\"", file_name);
        return -1;
    }

    struct ogw_fheader_t fheader = { 0 };
    if (ogw_readfheader(out->fd, &fheader) < 0)
    {
        log_error("Failed reading file header");
        return -1;
    }

    if (ogw_readgroup(out->fd, fheader.root_offset, &out->root) < 0)
    {
        abc_close(out);
        log_error("Failed reading root node");
        return -1;
    }

    struct ogw_data_t md_data = { 0 };
    ogw_readdata(out->fd, out->root.children[5], &md_data);
    ogw_parse_md(&md_data, &out->md);
        
    assert(out->root.num_children == 6);
    return 0;
}

void abc_close(struct abc_archive_t* archive)
{
    assert(archive != NULL);
    
    if (archive->fd != NULL)
    {
        fclose(archive->fd);
        archive->fd = NULL;
    }

    ogw_release_metadata(&archive->md);
    ogw_releasegroup(&archive->root);
}

int abc_get_toplevel(const struct abc_archive_t* archive, struct ogw_group_t* out)
{
    if(ogw_readgroup(archive->fd, archive->root.children[2], out) < 0)
    {
        log_error("Failed reading toplevel object node");
        ogw_releasegroup(out);
        return -1;
    }

    return 0;
}

int abc_get_children(const struct abc_archive_t* archive, const struct ogw_group_t* group, 
        struct abc_grouplist_t* out)
{
    assert(out   != NULL);
    assert(group != NULL);
    
    if (group == NULL || out == NULL)
    { 
        return -1;
    }

    // Group has as many child objects as there are child nodes
    // minus header and compound property child nodes
    const uint64_t num_objects = group->num_children - 2;

    // Fill num_groups in the loop below so that the release
    // function does not delete uninitialised objects when
    // returning due to an error while reading group nodes
    out->num_groups = 0;
    out->groups     = calloc(sizeof(struct ogw_group_t), out->num_groups);

    if (out->groups == NULL)
    {
        log_error("Failed allocating memory for child objects!");
        abc_release_grouplist(out);
        return -1;
    }

    for (uint64_t i = 0; i < num_objects; ++i)
    {
        const uint64_t object_index = i + 1;
        const uint64_t object_loc   = group->children[object_index];
        
        if (ogw_readgroup(archive->fd, object_loc, &out->groups[i]) < 0)
        {
            abc_release_grouplist(out);
            return -1;
        }

        out->num_groups++;
    }

    return 0;
}

int abc_get_child(const struct abc_archive_t* archive, const struct ogw_group_t* parent, 
        const char* name, struct ogw_group_t* out)
{
    assert(parent != NULL);
    assert(out    != NULL);

    if (parent == NULL || out == NULL)
    {
        return -1;
    }
    
    const uint64_t header_index = ogw_header_index(parent);

    if (header_index == ogw_invalid_index)
    {
        log_error("Failed finding object header");
        return -1;
    }

    const uint64_t header_location = parent->children[header_index];

    struct ogw_data_t header_data = { 0 };
    ogw_readdata(archive->fd, header_location, &header_data);

    struct ogw_ohdr_t ohdr = { 0 };
    ogw_parse_ohdr(&header_data, &ohdr);

    uint64_t group_location = -1; 
    for (uint64_t i = 0; i < ohdr.num_names; ++i)
    {
        if (strcmp(name, ohdr.names[i]) == 0)
        {
            // Need to offset child by one to skip 
            // properties group of parent object!
            group_location = parent->children[i + 1];
            break;
        }
    }

    ogw_release_ohdr(&ohdr);
    ogw_releasedata(&header_data);

    if (group_location == -1)
    {
        log_error("Failed finding child object");
        return -1;
    }

    ogw_readgroup(archive->fd, group_location, out);
    return 0;
}

int abc_get_object(const struct abc_archive_t* archive, const char* path, struct ogw_group_t* out)
{
    const size_t str_size = strlen(path) + 1;

    if (str_size == 0)
    {
        return -1;
    }

    const char* sub_start = path;
    if (sub_start[0] == '/')
    {
        sub_start += 1;
    }

    struct ogw_group_t current = { 0 };
    if (abc_get_toplevel(archive, &current) < 0)
    {
        log_error("Failed retrieving toplevel object node");
        return -1;
    }

    const char* sub_end = strchr(sub_start, '/');

    while (sub_end != NULL)
    {
        struct ogw_group_t next = { 0 };

        if (abc_get_substrchild(archive, &current, sub_start, sub_end, &next) < 0)
        {
            log_error("Failed finding child element");
            ogw_releasegroup(&current);
            return -1;
        }

        ogw_releasegroup(&current);
        current = next;
        
        sub_start = sub_end + 1;
        sub_end   = strchr(sub_start, '/');
    }

    sub_end = path + str_size;
    if (abc_get_substrchild(archive, &current, sub_start, sub_end, out) < 0)
    {
        log_error("Failed finding child element");
        ogw_releasegroup(&current);
        return -1;
    }

    ogw_releasegroup(&current);

    return 0; 
}

int abc_get_props(const struct abc_archive_t* archive, struct ogw_group_t* group, 
        struct abc_proplist_t* out)
{
    assert(group->num_children > 1);
    assert(out->props        == NULL);

    if (group->num_children < 1)
    {
        log_error("Object group node malformed, does not contain a property node");
        return -1;
    }

    // The group's object properties are always stored 
    // in the first child as a compound property
    uint64_t compound_location = group->children[0];

    if (ogwnode_gettype(compound_location) != OGW_NODE_GROUP)
    {
        log_error("Tried reading property group node from a data node!");
        return -1;
    } 

    struct ogw_group_t compound_group = { 0 };
    if (ogw_readgroup(archive->fd, compound_location, &compound_group) < 0)
    {
        log_error("Failed reading property node");
        return -1;
    }

    uint64_t header_index    = ogw_header_index(&compound_group); 
    uint64_t header_location = compound_group.children[header_index];

    struct ogw_data_t phdr_data = { 0 };
    if (ogw_readdata(archive->fd, header_location, &phdr_data) < 0)
    {
        log_error("Failed reading property header");
        return -1;
    }
    
    struct ogw_phdr_t phdr = { 0 };
    if (ogw_parse_phdr(&phdr_data, &phdr) < 0)
    {
        log_error("Failed parsing property header");
        return -1;
    }

    assert(phdr.num_props == 1);
    assert(phdr.props[0].info.prop_class == ABC_PROPCLASS_COMPOUND);
    assert(compound_group.num_children == 2);

    if (phdr.num_props != 1 || phdr.props[0].info.prop_class != ABC_PROPCLASS_COMPOUND)
    {
        log_error("First child of object must be a compound property node!");
        return -1;
    }

    if (compound_group.num_children != 2)
    {
        log_error("Compound property must have at least 2 child nodes!");
        return -1;
    }

    // Load compound property metadata
    const uint8_t md_index = phdr.props[0].info.md_index;

    if (md_index == 0xff)
    {
        // TODO: Add suppport for inline metadata
        log_error("Inline metadata not supported yet");
        return -1;
    }
    else if (md_index >= archive->md.num_entries)
    {
        log_error("Failed getting metadata, index out of bounds");
        return -1;
    }

    struct abc_mdentry_t* entry = &archive->md.entries[md_index];

    size_t sotitle_size = 0;
    if (abc_get_mdvalue(entry, "schemaObjTitle", NULL, &sotitle_size) < 0)
    {
        log_error("No schemaObjTitle defined for object properties");
        return -1;
    }

    if (abc_mdvalue_to_schema(entry, "schema",         &out->schema) < 0 ||
        abc_mdvalue_to_schema(entry, "schemaBaseType", &out->sbtype) < 0)
    {
        log_error("Failed translating schema or schemaBaseType");
        return -1;
    }

    out->sotitle = malloc(sotitle_size);
    if (abc_get_mdvalue(entry, "schemaObjTitle", out->sotitle, &sotitle_size) < 0)
    {
        log_error("Failed reading schema object title");
        return -1;
    }

    // Compound property needs a single child which
    // contains the individual property nodes as
    // its children
    uint64_t props_location = compound_group.children[0];
    struct ogw_group_t props_group = { 0 };
    if (ogw_readgroup(archive->fd, props_location, &props_group) < 0)
    {
        log_error("Failed reading property group");
        return -1;
    }

    uint64_t pheader_index = ogw_header_index(&props_group);
    uint64_t pheader_loc   = props_group.children[pheader_index];
    struct ogw_data_t pheader_data = { 0 };
    if (ogw_readdata(archive->fd, pheader_loc, &pheader_data) < 0)
    {
        log_error("Failed reading property header data");
        return -1;
    }

    struct ogw_phdr_t props_phdr = { 0 };
    if (ogw_parse_phdr(&pheader_data, &props_phdr) < 0)
    {
        log_error("Failed parsing property header data");
        return -1;
    }

    out->num_props = props_phdr.num_props;
    out->props     = malloc(sizeof(struct abc_prop_t) * out->num_props);

    for (uint64_t i = 0; i < props_phdr.num_props; ++i)
    {
        struct abc_prop_t* prop = &out->props[i];
        memset(prop, 0, sizeof(struct abc_prop_t));

        // Copy all relevant information
        size_t str_size   = strlen(props_phdr.props[i].name);
        prop->prop_class  = props_phdr.props[i].info.prop_class;
        prop->group_loc   = props_group.children[i];
        prop->name        = malloc(str_size + 1);
        strcpy(prop->name, props_phdr.props[i].name);

        // TODO: Num samples only represents samples that have been 
        // serialised, there are usually props which have more samples 
        // than being serialised when these samples are constant for a 
        // series of samples (indicated by first_changed_index/last_changed 
        // index in ogw_prop_t
        if (props_phdr.props[i].last_changed_index == 0)
        {
            prop->num_samples = 1;
        }
        else
        {
            prop->num_samples = props_phdr.props[i].next_sample_index;
        }

        const enum abc_pod pod    = props_phdr.props[i].info.pod;
        const size_t       extent = props_phdr.props[i].info.extent;

        prop->dtype = abc_to_dtype(pod, extent);
    }

    ogw_releasedata(&pheader_data);     
    ogw_releasegroup(&props_group);

    ogw_release_phdr(&phdr);
    ogw_releasedata(&phdr_data);
    ogw_releasegroup(&compound_group);

    return 0;
}

void abc_release_grouplist(struct abc_grouplist_t* grouplist)
{
    if (grouplist->groups != NULL)
    {
        const uint64_t num_groups = grouplist->num_groups;

        for (uint64_t i = 0; i < num_groups; ++i)
        {
            ogw_releasegroup(&grouplist->groups[i]);
        }
    
        free(grouplist->groups);
        grouplist->groups = NULL;
    }

    grouplist->num_groups = 0;
}

void abc_release_proplist(struct abc_proplist_t* proplist)
{
    assert(proplist != NULL);

    for (uint64_t i = 0; i < proplist->num_props; ++i)
    {
        if (proplist->props[i].name != NULL)
        {
            free(proplist->props[i].name);
            proplist->props[i].name = NULL;
        }
    }

    if (proplist->sotitle != NULL)
    {
        free(proplist->sotitle);
        proplist->sotitle = NULL;
    }

    free_invalidate(proplist->props);
    proplist->num_props = 0;
}

int abc_get_mdvalue(const struct abc_mdentry_t* entry, const char* key, 
        char* out, size_t* buf_size)
{
    if (key == NULL)
    {
        assert(key != NULL);
        log_error("No key for lookup provided!");
        return -1;
    }

    uint64_t pair_index = 0xffffffffffffffff;
    for (uint64_t i = 0; i < entry->num_pairs; ++i)
    {
        if (strcmp(entry->keys[i], key) == 0)
        {
            pair_index = i;
            break;
        }
    }

    if (pair_index == 0xffffffffffffffff)
    {
        log_error("Key %s not found", key);
        return -1;
    }

    size_t value_size = strlen(entry->values[pair_index]) + 1;
    if (out == NULL)
    {
        *buf_size = value_size;
        return 0;
    }
    else if (*buf_size >= value_size)
    {
        memcpy(out, entry->values[pair_index], value_size);
        return 0;
    }
    else
    {
        log_error("Destination buffer too small!");
        return -1;
    }
}

int abc_mdvalue_to_schema(const struct abc_mdentry_t* entry, 
        const char* key, enum abc_schema* out)
{
    assert(entry != NULL);
    assert(out   != NULL);
    assert(key   != NULL);

    if (entry == NULL || out == NULL || key == NULL)
    {
        log_error("Failed translating metadata value into schema");
        return -1;
    }

    size_t valstr_size = 0;
    if (abc_get_mdvalue(entry, key, NULL, &valstr_size) < 0)
    {
        log_error("Failed reading value for key \"%s\"", key);
        return -1;
    }

    char* valstr = alloca(valstr_size);
    abc_get_mdvalue(entry, key, valstr, &valstr_size);

    const uint64_t num_schemas = countof(schema_strings);
    for (uint64_t i = 0; i < num_schemas; ++i)
    {
        if (strcmp(valstr, schema_strings[i]) == 0)
        {
            *out = (enum abc_schema)i;
            return 0;
        }
    }

    // Fall through in no schema was found
    log_error("Schema value \"%s\" is either invalid or not supported", valstr);
    return -1;
}

int abc_parse_schema(const struct abc_archive_t* archive, const struct abc_proplist_t* list, 
        const struct abc_schema_def_t* schema_def, void* out)
{
    // Check whether schema name and base name are correct
    if (list->schema != schema_def->schema)
    {
        log_error("Cannot parse as \"%s\", schema name is \"%s\"", 
                schema_strings[schema_def->schema], 
                schema_strings[list->schema]);
        return -1;
    }

    if (list->sbtype != schema_def->sbtype)
    {
        log_error("Cannot parse schema with base type \"%s\", schema base type is \"%s\"", 
                schema_strings[schema_def->sbtype],
                schema_strings[list->sbtype]);
        return -1;
    }
    
    // Check that all mandatory properties are available
    const size_t num_mandatory = schema_def->num_mandatory;
    for (size_t i = 0; i < num_mandatory; ++i)
    {
        if (abc_hasprop(schema_def->mandatory_props[i], list) == false)
        {
            log_error("Failed parsing schema Polymeshv1, properties lack"
                    " \"%s\" which is required", schema_def->mandatory_props[i]);
            return -1;
        }
    }    
    
    const size_t num_props = schema_def->num_props;
    for (size_t i = 0; i < num_props; ++i)
    {
        const struct abc_prop_def_t* def  = &schema_def->schema_props[i];
        const struct abc_prop_t*     prop = abc_getprop(def->name, list); 

        // At this point only optional properties 
        // can be missing, checked presence of the
        // mandatory props above
        if (prop == NULL)
        {
            continue;
        }

        if (abc_valid_prop(prop, def) == false)
        {
            log_warn("Property \"%s\" not well defined, skipping", prop->name);
            continue;
        }

        if (abc_parse_property(archive, prop, def, out) < 0)
        {
            log_error("Failed parsing schema %s", schema_strings[schema_def->schema]);
            return -1;
        }
    }

    return 0;
}

int abc_parse_property(const struct abc_archive_t* archive, const struct abc_prop_t* prop, 
        const struct abc_prop_def_t* def, void* out)
{
    if (prop->dtype != def->dtype)
    {
        log_error("Failed parsing property \"%s\", property data type not correct.\n"
                  "Expected type %s but received %s\n", def->name, 
                  abc_dtype_strings[def->dtype], abc_dtype_strings[prop->dtype]);

        return -1;
    }

    if (prop->prop_class != def->prop_class)
    {
        log_error("Failed parsing \"%s\" property, incorrect property class!", prop->name);
        return -1;
    }

    struct ogw_group_t prop_group = { 0 };
    ogw_readgroup(archive->fd, prop->group_loc, &prop_group);

    if (prop->prop_class == ABC_PROPCLASS_ARRAY)
    {
        // Retrieve destination array
        char*               memloc    = ((char*) out) + def->moffset;
        struct abc_array_t* array_out = (struct abc_array_t*) memloc;

        if (abc_parse_array(archive, prop, &prop_group, array_out) < 0)
        {
            log_error("Failed parsing \"%s\" property", prop->name);
            ogw_releasegroup(&prop_group);
            return -1;
        }
    }
    else if (prop->prop_class == ABC_PROPCLASS_SCALAR)
    {
        // Retrieve destination scalar
        char*                memloc     = ((char*) out) + def->moffset;
        struct abc_scalar_t* scalar_out = (struct abc_scalar_t*) memloc;

        if (abc_parse_scalar(archive, prop, &prop_group, scalar_out) < 0)
        {
            log_error("Failed parsing \"%s\" property", prop->name);
            ogw_releasegroup(&prop_group);
            return -1;
        }
    }
    else
    {
        log_error("Failed parsing \"%s\" property, property class invalid. "
                "This is a library bug", prop->name);
    }

    ogw_releasegroup(&prop_group);
    return 0;
}


int abc_parse_scalar(const struct abc_archive_t* archive, const struct abc_prop_t* prop, 
        const struct ogw_group_t* group, struct abc_scalar_t* out)
{
    if (prop->prop_class != ABC_PROPCLASS_SCALAR)
    {
        log_error("Failed parsing scalar property, supplied property is not scalar");
        return -1;
    }

    if (group->num_children != prop->num_samples)
    {
        log_error("Property group has less children than samples expected by property");
        return -1;
    }

    // Check destination buffer size
    const size_t sample_size  = abc_dtype_sizes[prop->dtype];
    const size_t num_samples  = prop->num_samples;
    const size_t data_size    = sample_size * num_samples;

    out->num_samples = num_samples;
    out->dtype       = prop->dtype;

    char* dst = abc_alloc_scalar(out, data_size); 

    if (dst == NULL)
    {
        log_error("Failed allocating memory for scalar samples");
        return -1;
    }

    for (size_t i = 0; i < num_samples; ++i)
    {
        uint64_t sample_loc = group->children[i];
        struct ogw_data_t sample_data = { 0 };
        ogw_readdata(archive->fd, sample_loc, &sample_data);

        const char*  src          = sample_data.bytes + KEY_BYTES;
        const size_t data_size    = sample_data.num_bytes;
        const size_t payload_size = (data_size < KEY_BYTES) ? 0 : data_size - KEY_BYTES;

        if (payload_size == 0)
        {
            log_error("Failed reading scalar sample, data node was empty or currupted!");
            ogw_releasedata(&sample_data);
            abc_release_scalar(out);
            return -1;
        }

        // TODO: Need to test reading of string properties
        if (out->dtype == ABC_DTYPE_U8STR)
        {
            out->u8str = malloc(payload_size);
            dst = out->u8str;
        }
        else if (out->dtype == ABC_DTYPE_WSTR)
        {
            out->wstr = malloc(payload_size);
            dst = out->wstr;
        }
        else if (payload_size != sample_size)
        {
            log_error("Unexpected scalar sample size");
            ogw_releasedata(&sample_data);
            abc_release_scalar(out);
            return -1;
        }

        if (dst == NULL)
        {
            log_error("Failed allocating memory for scalar property");
            ogw_releasedata(&sample_data);
            abc_release_scalar(out);
        }
        else
        {
            memcpy(dst, src, payload_size);
            dst += sample_size;
        }

        ogw_releasedata(&sample_data);
    }

    return 0;
}

void abc_release_scalar(struct abc_scalar_t* scalar)
{
    abc_free_scalar(scalar);
}


int abc_parse_array_sample(FILE* f, const struct ogw_group_t* parent, enum abc_dtype dtype, 
        uint64_t sample_index, struct abc_array_sample_t* out)
{
    uint64_t vals_loc = parent->children[2 * sample_index + 0];
    uint64_t dims_loc = parent->children[2 * sample_index + 1];

    const size_t dtype_size = abc_dtype_sizes[dtype];

    struct ogw_data_t vals_data = { 0 };
    ogw_readdata(f, vals_loc, &vals_data);

    if (abc_read_dimensions(f, dims_loc, dtype_size, &vals_data, out) < 0)
    {
        log_error("Failed determining array sample dimensions");
        return -1;
    } 

    // Count number of all elements 
    // across all array dimensions
    size_t total_elements = 0;
    for (uint64_t i = 0; i < out->num_dims; ++i) { total_elements += out->dims[i]; }

    const size_t sample_size  = total_elements * dtype_size;
    const size_t data_size    = vals_data.num_bytes;
    const size_t payload_size = (data_size < KEY_BYTES) ? 0 : data_size - KEY_BYTES;

    if (payload_size == 0)
    {
        log_error("Failed reading array sample, data node was empty or currupted!");
        ogw_releasedata(&vals_data);
        abc_release_array_sample(out);
        return -1;
    }

    // TODO: Not clear how string arrays would be stored
    if (out->dtype == ABC_DTYPE_U8STR)
    {
        log_error("UTF8 string array properties not supported!");
        ogw_releasedata(&vals_data);
        abc_release_array_sample(out);
        return -1;
    }
    else if (out->dtype == ABC_DTYPE_WSTR)
    {
        log_error("UTF32 string array properties not supported!");
        ogw_releasedata(&vals_data);
        abc_release_array_sample(out);
        return -1;
    }
    else if (payload_size != sample_size)
    {
        log_error("Data node does not match expected data size");
        ogw_releasedata(&vals_data);
        abc_release_array_sample(out);
        return -1;
    }

    char* src = vals_data.bytes + KEY_BYTES;
    char* dst = 0;

    out->dtype = dtype;
    dst = abc_alloc_array_sample(out, payload_size);

    if (dst == NULL)
    {
        log_error("Failed allocating space for array sample");
        abc_release_array_sample(out);
        return -1;
    }

    memcpy(dst, src, sample_size);
    ogw_releasedata(&vals_data);

    return 0;
}

void abc_release_array_sample(struct abc_array_sample_t* sample)
{
    abc_free_array_sample(sample);

    free_invalidate(sample->dims);
    sample->num_dims = 0;
}

int abc_parse_array(const struct abc_archive_t* archive, const struct abc_prop_t* prop, 
        const struct ogw_group_t* group, struct abc_array_t* out)
{
    if (out == NULL)
    {
        log_error("Output parameter must not be null!");
        abc_release_array(out);
        return -1;
    }

    if (group->num_children != 2 * prop->num_samples)
    {
        log_error("Property group has less children than samples expected by property");
        abc_release_array(out);
        return -1;
    }

    out->samples = malloc(sizeof(struct abc_array_sample_t) * prop->num_samples);

    if (out->samples == 0)
    {
        log_error("Failed allocating space for array samples");
        abc_release_array(out);
        return -1;
    }

    // Need to increase samples in the following loop
    // so that release only frees valid samples in 
    // case of an error
    out->num_samples = 0;

    for (size_t i = 0; i < prop->num_samples; ++i)
    {
        abc_parse_array_sample(archive->fd, group, prop->dtype, i, &out->samples[i]);
        out->num_samples++;
    }

    return 0;
}

void abc_release_array(struct abc_array_t* array)
{
    const uint64_t num_samples = array->num_samples;

    for (uint64_t i = 0; i < num_samples; ++i)
    {
        abc_release_array_sample(&array->samples[i]);  
    }

    free_invalidate(array->samples);
    array->num_samples = 0;
}
