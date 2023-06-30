#ifndef OGW_OGAWA_H
#define OGW_OGAWA_H

#include <stdio.h>
#include <assert.h>
#include <inttypes.h>

#include "alembic_types.h"

static const uint64_t ogw_invalid_index  = 0xffffffffffffffff;
static const uint8_t  ogw_inline_mdindex = 0xff;

struct ogw_group_t
{
    uint64_t  num_children;
    uint64_t* children;
    
    uint64_t file_offset;
};

struct ogw_fheader_t
{
    char     magic_number[5];
    uint8_t  frozen_byte;
    uint8_t  major;
    uint8_t  minor;
    uint64_t root_offset;
};

struct ogw_data_t
{
    uint64_t num_bytes;
    char*    bytes;

    uint64_t file_offset;
};

struct ogw_sampleinfo_t
{
    uint32_t max_sample;
    double   time_per_cycle;
    uint32_t num_samples;
    void*    sample_data;
};

struct ogw_ohdr_t
{
    uint32_t num_names;
    char**   names;
    uint8_t  md_index;  // 0xff means inline
};

struct ogw_propinfo_t
{
    uint32_t prop_class       : 2;
    uint32_t int_size         : 2;
    uint32_t pod              : 4;
    uint32_t with_timesamples : 1;
    uint32_t changed_indices0 : 1;
    uint32_t homogeneous      : 1;
    uint32_t changed_indices1 : 1;
    uint32_t extent           : 8;
    uint32_t md_index         : 8; // 0xff means inline
    uint32_t pad0             : 4;
};

struct ogw_prop_t
{
    struct ogw_propinfo_t info;

    uint32_t next_sample_index;
    uint32_t first_changed_index;
    uint32_t last_changed_index;
    uint32_t time_sampling_index;
    uint64_t num_properties;
    char*    name;
};

struct ogw_phdr_t 
{
    struct ogw_prop_t* props;
    uint64_t           num_props;
};

static_assert(sizeof(struct ogw_propinfo_t) == sizeof(uint32_t), 
        "Ogawa Property Info must have size of uint32_t!");

enum ogwnode_type
{
    OGW_NODE_GROUP = 0,
    OGW_NODE_DATA  = 1
};

enum ogwnode_type ogwnode_gettype(uint64_t node_bytes);
uint64_t          ogwnode_getoffset(uint64_t node_bytes);
uint64_t          ogw_header_index(const struct ogw_group_t* group);

int  ogw_readfheader(FILE* f, struct ogw_fheader_t* fheader);
int  ogw_readdata(FILE* f, uint64_t node_location, struct ogw_data_t* out);
int  ogw_readgroup(FILE* f, uint64_t node_location, struct ogw_group_t* out);
void ogw_releasedata(struct ogw_data_t* data);
void ogw_releasegroup(struct ogw_group_t* group);
int  ogw_parse_md(const struct ogw_data_t* data, struct abc_metadata_t* out);
void ogw_release_metadata(struct abc_metadata_t* metadata);
int  ogw_parse_ohdr(const struct ogw_data_t* data, struct ogw_ohdr_t* out);
int  ogw_parse_phdr(const struct ogw_data_t* data, struct ogw_phdr_t* out);
void ogw_release_ohdr(struct ogw_ohdr_t* ohdr);
void ogw_release_phdr(struct ogw_phdr_t* phdr);

#endif
