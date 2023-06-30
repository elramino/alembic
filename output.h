
#ifndef OGW_OUTPUT_H
#define OGW_OUTPUT_H

#include <stdlib.h>
#include <string.h>

#include "alembic_types.h"
#include "alembic_proptypes.h"
#include "alembic.h"
#include "ogawa.h"

void print_fheader(const struct ogw_fheader_t* fheader);
void print_scalar(const struct abc_scalar_t* scalar);
void print_array(const struct abc_array_t* array, uint64_t sample_id);
void print_data(const char* title, const struct ogw_data_t* data);
void print_group(const char* title, const struct ogw_group_t* group);
void print_ohdr(uint64_t header_index, uint64_t header_location,
        const struct ogw_data_t* data, const struct ogw_ohdr_t* header);
void print_phdr(uint64_t header_index, uint64_t header_location,
        const struct ogw_data_t* data, const struct ogw_phdr_t* header);
void print_proplist(const struct abc_proplist_t* proplist);
int print_md(const char* title, const struct abc_metadata_t* md);

#endif
