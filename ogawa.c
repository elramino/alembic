#include <stdlib.h>
#include <string.h>

#include "alembic_proptypes.h"
#include "ogawa.h"
#include "logging.h" 
#include "utils.h"

static char* strnchr(char* str, char ch, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        if (str[i] == ch)
        { return str + i; }
    }

    return (str + n);
}

static uint32_t read_varint(unsigned char** rd_ptr, uint32_t size)
{
    if (size == ABC_PROPINFO_INTTYPE_U8)
    {
        uint32_t r = ((uint8_t*) *rd_ptr)[0];
        *rd_ptr += sizeof(uint8_t);
        return r;
    }
    if (size == ABC_PROPINFO_INTTYPE_U16)
    {
        uint32_t r = ((uint16_t*) *rd_ptr)[0];
        *rd_ptr += sizeof(uint16_t);
        return r;
    }
    if (size == ABC_PROPINFO_INTTYPE_U32)
    {
        uint32_t r = ((uint32_t*) *rd_ptr)[0];
        *rd_ptr += sizeof(uint32_t);
        return r;
    }
    
    puts("Failed reading integer, invalid size value");
    return -1;
}

static char* ogw_parse_mdentry(char* read_ptr, char* write_ptr, uint8_t str_size, 
        struct abc_mdentry_t* out)
{
    out->num_pairs = 0;
    out->keys      = NULL;
    out->values    = NULL;

    char* data_end = read_ptr + str_size;

    while(read_ptr < data_end)
    {
        // Find key value pair
        char* key_ptr = read_ptr;
        char* key_end = strnchr(key_ptr, '=', data_end - key_ptr);
        read_ptr = key_end + 1;

        char* val_ptr = read_ptr;
        char* val_end = strnchr(val_ptr, ';', data_end - val_ptr);
        read_ptr = val_end + 1;

        size_t key_size = key_end - key_ptr;
        size_t val_size = val_end - val_ptr;

        // Copy key and value into storage
        char* key = write_ptr;
        memcpy(key, key_ptr, key_size);
        write_ptr[key_size] = '\0';
        write_ptr += key_size + 1;

        char* value = write_ptr;
        memcpy(value, val_ptr, val_size);
        write_ptr[val_size] = '\0';
        write_ptr += val_size + 1;

        size_t alloc_size = (out->num_pairs + 1) * sizeof(char*);
        char** k = realloc(out->keys,   alloc_size);
        char** v = realloc(out->values, alloc_size);

        if (k == NULL || v == NULL)
        {
            puts("Failed allocating space for key-value pair references");
            return NULL;
        }

        out->keys   = k;
        out->values = v;
        out->num_pairs++;

        out->keys[out->num_pairs - 1]   = key;    
        out->values[out->num_pairs - 1] = value;    
    }

    return write_ptr;
}

static void ogw_release_mdentry(struct abc_mdentry_t* md_entry)
{
    assert(md_entry->keys   != NULL);
    assert(md_entry->values != NULL);

    if (md_entry->keys)
    {
        free(md_entry->keys);
        md_entry->keys = NULL;
    }

    if (md_entry->values)
    {
        free(md_entry->values);
        md_entry->values = NULL;
    }

    md_entry->num_pairs = 0;
}



enum ogwnode_type ogwnode_gettype(uint64_t node_bytes)
{
    return (node_bytes >> 63);
}

uint64_t ogwnode_getoffset(uint64_t node_bytes)
{
    return node_bytes & ~(1ull << 63);
}

uint64_t ogw_header_index(const struct ogw_group_t* group)
{
    uint64_t header_index    = group->num_children - 1;    
    uint64_t header_location = group->children[header_index];

    while (header_index >= 1 && 
           ogwnode_gettype(header_location) != OGW_NODE_DATA)
    {
        header_index--;
        header_location = group->children[header_index];
    }

    if (ogwnode_gettype(header_location) != OGW_NODE_DATA)
    {
        return ~0;
    }

    return header_index;
}

int ogw_readfheader(FILE* f, struct ogw_fheader_t* archive)
{
    if (0 != fseek(f, 0, SEEK_SET))
    {
        log_error("Failed moving to position 0 in file");
        return -1;
    }

    if (1 != fread(&archive->magic_number, sizeof(archive->magic_number), 1, f))
    {
        log_error("Failed reading archive header\n");
        return -1;
    }

    if (1 != fread(&archive->frozen_byte, sizeof(archive->frozen_byte), 1, f))
    {
        log_error("Failed reading archive header\n");
        return -1;
    }

    if (1 != fread(&archive->major, sizeof(archive->major), 1, f))
    {
        log_error("Failed reading archive header\n");
        return -1;
    }

    if (1 != fread(&archive->minor, sizeof(archive->minor), 1, f))
    {
        log_error("Failed reading archive header\n");
        return -1;
    }

    if (1 != fread(&archive->root_offset, sizeof(archive->root_offset), 1, f))
    {
        log_error("Failed reading archive header\n");
        return -1;
    }
 
    return 0;
}

int ogw_readdata(FILE* f, uint64_t node_location, struct ogw_data_t* out)
{
    if (OGW_NODE_DATA != ogwnode_gettype(node_location))
    {
        puts("Tried reading a group node as data node");
        return -1;
    }

    size_t fpos = ogwnode_getoffset(node_location);
    
    if (0 != fseek(f, fpos, SEEK_SET))
    {
        printf("Failed moving to position %zu in file", fpos);
        return -1;
    } 
    
    if (1 != fread(&out->num_bytes, sizeof(out->num_bytes), 1, f))
    {
        puts("Failed reading size of data node");
        return -1;
    }

    out->bytes = malloc(out->num_bytes);

    if (out->bytes == NULL)
    {
        printf("Failed allocating %" PRIu64 " bytes for data node\n", out->num_bytes);
        return -1;
    }

    if (1 != fread(out->bytes, out->num_bytes, 1, f))
    {
        puts("Failed reading content of data node\n");
        return -1;
    }

    out->file_offset = fpos;
    return 0;
}

int ogw_readgroup(FILE* f, uint64_t node_location, struct ogw_group_t* out)
{
    // Ensure node is empty
    assert(out->num_children == 0);
    assert(out->children     == NULL);

    if (OGW_NODE_GROUP != ogwnode_gettype(node_location))
    {
        log_error("Tried reading a data node as group node");
        ogw_releasegroup(out);
        return -1;
    }

    size_t fpos = ogwnode_getoffset(node_location);

    if (0 != fseek(f, fpos, SEEK_SET))
    {
        log_error("Failed moving to position %zu in file", fpos);
        ogw_releasegroup(out);
        return -1;
    }   

     uint64_t node_header;
    if (1 != fread(&node_header, sizeof(node_header), 1, f))
    {
        log_error("Failed reading Ogawa group at position %zu\n", fpos);
        ogw_releasegroup(out);
        return -1;
    }

    uint64_t  num_children = ogwnode_getoffset(node_header);
    uint64_t* children     = malloc(sizeof(uint64_t) * num_children);

    if (children == NULL)
    {
        log_error("Failed allocating memory for child offset!");
        ogw_releasegroup(out);
        return -1;
    }
 
    if (num_children != fread(children, sizeof(uint64_t), num_children, f))
    {
        log_error("Failed reading all child header bytes!");
        ogw_releasegroup(out);
        return -1;
    }

    out->num_children = num_children;
    out->children     = children;

    return 0;
}

void ogw_releasedata(struct ogw_data_t* data)
{
    if (data->bytes != NULL)
    {
        free(data->bytes);
        data->bytes = NULL;
    }
}

void ogw_releasegroup(struct ogw_group_t* group)
{
    if (group->children != NULL)
    {
        free(group->children);
        group->children = NULL;
    }

    group->num_children = 0;
}

int ogw_parse_md(const struct ogw_data_t* data, struct abc_metadata_t* out)
{
    assert(out->storage     == NULL);
    assert(out->entries     == NULL);
    assert(out->num_entries == 0);

    // Cannot need more storage than data stream size
    size_t storage_size = data->num_bytes;
    out->storage        = malloc(storage_size);
    out->num_entries    = 0;

    if (out->storage == NULL)
    {
        puts("Failed allocating memory for file metadata");
        return -1;
    }

    char* storage_ptr = out->storage;
    char* read_ptr    = data->bytes;
    char* data_end    = read_ptr + data->num_bytes;
    
    while (read_ptr < data_end)
    {
        // Read string size
        uint8_t entry_size = ((uint8_t*)read_ptr)[0];
        read_ptr += sizeof(uint8_t);

        uint64_t index_entry = out->num_entries;
        out->num_entries += 1;
        size_t entry_bytes = sizeof(struct abc_mdentry_t) * out->num_entries;

        char* entries = realloc(out->entries, entry_bytes);
        if (entries == NULL)
        {
            puts("Failed allocating space for metadata entries");
            return -1;
        }
        out->entries = (struct abc_mdentry_t*) entries;

        struct abc_mdentry_t* entry = &out->entries[index_entry];
        storage_ptr = ogw_parse_mdentry(read_ptr, storage_ptr, entry_size, entry);
        read_ptr += entry_size;
 
        if (storage_ptr == NULL)
        {
            puts("Failed parsing metadata!");
            return -1;
        }

        size_t written_size = storage_ptr - out->storage; 
        assert(written_size <= storage_size);
    }

    return 0;
}

void ogw_release_metadata(struct abc_metadata_t* md)
{
    for (uint64_t i = 0; i < md->num_entries; ++i)
    {
        ogw_release_mdentry(&md->entries[i]);
    }

    if (md->storage != NULL)
    {
        free(md->storage);
        md->storage = NULL;
    }
    
    if (md->entries != NULL)
    {
        free(md->entries);
        md->entries = NULL;
    }

    md->num_entries = 0;
}

int ogw_parse_ohdr(const struct ogw_data_t* data, struct ogw_ohdr_t* out)
{
    assert(out->num_names  == 0);
    assert(out->names      == NULL);
    assert(out->md_index   == 0);

    const uint64_t hash_size    = UINT64_C(32);
    const uint64_t payload_size = data->num_bytes - hash_size;
    unsigned char* rd_ptr       = (unsigned char*) data->bytes; 
    unsigned char* end_ptr      = (unsigned char*) data->bytes + payload_size;

    while (rd_ptr < end_ptr)
    {
        uint32_t name_size = ((uint32_t*) rd_ptr)[0];
        rd_ptr += sizeof(name_size);

        if (rd_ptr >= end_ptr)
        {
            log_error("Failed reading object names, tried to read out of data node's bounds");
            return -1;
        }

        out->names = realloc(out->names, (out->num_names + 1) * sizeof(char*));

        if (out->names == NULL)
        {
            puts("Failed allocating memory for object header name pointer!");
            return -1;
        }

        // TODO: Use storage member to store the strings!
        out->names[out->num_names] = malloc(name_size + 1);

        if (out->names[out->num_names] == NULL)
        {
            puts("Failed allocating memory for object header name!");
            return -1;
        }
        
        memcpy(out->names[out->num_names], rd_ptr, name_size);
        out->names[out->num_names][name_size] = '\0';
        out->num_names++;
        rd_ptr += name_size;

        out->md_index = *((uint8_t*) rd_ptr);
        rd_ptr += sizeof(out->md_index);
     
        // TODO: Need to implement inline metadata processing!
        if (out->md_index == 0xff)
        {
            uint32_t inline_md_size = ((uint32_t*)rd_ptr)[0];
            rd_ptr += sizeof(inline_md_size);

            // TODO: Read metadata string following md_size and 
            // parse key value pairs using ogw_parse_md
            puts("Inline metadata not yet implemented!!!");
        }
    }

    return 0;
}

int ogw_parse_phdr(const struct ogw_data_t* data, struct ogw_phdr_t* out)
{
    assert(out->num_props    == 0);
    assert(out->props        == NULL);

    unsigned char* rd_ptr  = (unsigned char*) data->bytes; 
    unsigned char* end_ptr = rd_ptr + data->num_bytes;

    while (rd_ptr < end_ptr)
    {
        out->num_props += 1;

        const size_t new_size = sizeof(struct ogw_prop_t) * out->num_props;
        out->props = realloc(out->props, new_size);
        struct ogw_prop_t* prop = &out->props[out->num_props - 1];

        prop->info = ((struct ogw_propinfo_t*) rd_ptr)[0];
        rd_ptr += sizeof(struct ogw_propinfo_t);

        prop->next_sample_index = read_varint(&rd_ptr, prop->info.int_size);

        if (prop->info.changed_indices0 != 0)
        {
            prop->first_changed_index = read_varint(&rd_ptr, prop->info.int_size);
            prop->last_changed_index  = read_varint(&rd_ptr, prop->info.int_size);
        }
        else if (prop->info.changed_indices1 != 0)
        {
            prop->first_changed_index = 0;
            prop->last_changed_index  = 0;
        }
        else
        {
            prop->first_changed_index = 0;
            prop->last_changed_index  = prop->next_sample_index - 1;
        }

        if (prop->info.with_timesamples == 1)
        {
            prop->time_sampling_index = read_varint(&rd_ptr, prop->info.int_size);
        }

        uint64_t name_size = read_varint(&rd_ptr, prop->info.int_size);
        prop->name = malloc(name_size + 1);

        // Copy name and temporarily store offset
        // from storage allocation base pointer
        memcpy(prop->name, rd_ptr, name_size);
        prop->name[name_size] = '\0';
        rd_ptr += name_size;    

        if (prop->info.md_index == 0xff)
        {
            //TODO: Inline metadata
            puts("Inline metadata not yet implemented");
        }
    }

    return 0;
}

void ogw_release_phdr(struct ogw_phdr_t* phdr)
{
    assert(phdr != NULL);

    if (phdr == NULL)
    {
        return;
    }

    for (uint64_t i = 0; i < phdr->num_props; ++i)
    {
        free(phdr->props[i].name);
    }

    if (phdr->props != NULL)
    {
        free(phdr->props);
        phdr->props = NULL;
    }

    phdr->num_props = 0;
}

void ogw_release_ohdr(struct ogw_ohdr_t* ohdr)
{
    assert(ohdr != NULL);
    if (ohdr == NULL)
    {
        return;
    }

    if (ohdr->names != NULL)
    {
        free(ohdr->names);
        ohdr->names = NULL;
    }

    ohdr->num_names = 0;
}
