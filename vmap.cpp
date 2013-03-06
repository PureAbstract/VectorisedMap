#include "vmap.h"
// For sanity's sake
template
class vmap<int,int>;

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

TEST_CASE( "vmap/default_construct", "Default construct a vmap" )
{
    typedef int key_type;
    typedef int mapped_type;
    vmap<key_type,mapped_type> thevmap;
    REQUIRE( thevmap.empty() );
    REQUIRE( thevmap.size() == 0 );
    REQUIRE( thevmap.begin() == thevmap.end() );
}

TEST_CASE( "vmap/from_empty_map", "Construct a vmap from an empty map" )
{
    typedef int key_type;
    typedef int mapped_type;
    std::map<key_type,mapped_type> themap;
    vmap<key_type,mapped_type> thevmap( thevmap );
    REQUIRE( thevmap.empty() );
    REQUIRE( thevmap.size() == 0 );
    REQUIRE( thevmap.empty() == themap.empty() );
    REQUIRE( thevmap.size() == themap.size() );
    REQUIRE( thevmap.begin() == thevmap.end() );
}

TEST_CASE( "vmap/from_small_map", "Construct a vmap from a small map" )
{
    typedef int key_type;
    typedef int mapped_type;
    std::map<key_type,mapped_type> themap;
    themap[0] = 0;
    themap[1] = 1;
    vmap<key_type,mapped_type> thevmap( themap );
    REQUIRE( !thevmap.empty() );
    REQUIRE( thevmap.empty() == themap.empty() );
    REQUIRE( thevmap.size() == themap.size() );
    REQUIRE( thevmap.begin() != thevmap.end() );
}
