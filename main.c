#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "alembic.h"
#include "ogawa.h"
#include "output.h"
#include "schema_polymeshv1.h"

void printn(char* str, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        putc(str[i], stdout);
    }
}

void print_chobjects(struct abc_archive_t* ar, struct ogw_group_t* g)
{
    uint64_t header_index = ogw_header_index(g);
    uint64_t header_loc   = g->children[header_index];

    struct ogw_data_t header_data = { 0 };
    ogw_readdata(ar->fd, header_loc, &header_data);

    struct ogw_ohdr_t ohdr = { 0 };
    ogw_parse_ohdr(&header_data, &ohdr);

    puts("Group child objects:");
    for (uint64_t i = 0; i < ohdr.num_names; ++i)
    {
        puts(ohdr.names[i]);
    }

    ogw_release_ohdr(&ohdr);
    ogw_releasedata(&header_data);
}

int main()
{
    puts("Parsing Alembic/Ogawa format");

    struct abc_archive_t archive = { 0 };
    abc_open("test.abc", &archive);

    struct ogw_group_t toplevel = { 0 };
    abc_get_toplevel(&archive, &toplevel);

    struct abc_grouplist_t list = { 0 };
    abc_get_children(&archive, &toplevel, &list);

    print_group("Toplevel object node:", &toplevel);

    for (uint64_t i = 0; i < list.num_groups; ++i)
    {
        print_group("Child", &list.groups[i]);
    }

    struct ogw_group_t knight_geom = { 0 };
    abc_get_child(&archive, &toplevel, "knight_rig:Geometry", &knight_geom);

/*
    // If there's a child with provided name
    if (knight_geom.num_children != 0)
    {
        print_group("Knight Geometry", &knight_geom);
    }

    ogw_releasegroup(&knight_geom);

    // Test finding by path
    {
        struct ogw_group_t path_group = { 0 };
        abc_get_object(&archive, "/knight_rig:Geometry/knight_rig:Modeled_knight_OLD"
                "/knight_rig:Sword/knight_rig:SwordShape", &path_group);

        print_chobjects(&archive, &path_group);

        struct abc_proplist_t proplist = { 0 };
        abc_get_props(&archive, &path_group, &proplist);

        print_proplist(&proplist);

        struct abc_schema_polymeshv1_t polymesh = { 0 };
        abc_parse_schema_polymeshv1(&archive, &proplist, &polymesh);
        //print_array(&polymesh.face_indices, 0);
        abc_release_schema_polymeshv1(&polymesh);

        abc_release_proplist(&proplist);
    }
*/
    // Test xform
    {
        struct ogw_group_t path_group = { 0 };
        abc_get_object(&archive, "/knight_rig:Geometry/knight_rig:Modeled_knight_OLD/"
                "knight_rig:BODY/knight_rig:polySurface583/knight_rig:polySurfaceShape591", 
                &path_group);

        print_chobjects(&archive, &path_group);
        struct abc_proplist_t proplist = { 0 };
        abc_get_props(&archive, &path_group, &proplist);

        print_proplist(&proplist);
        abc_release_proplist(&proplist);
    }

    abc_close(&archive);

    return EXIT_SUCCESS;
}
