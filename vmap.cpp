#include "vmap.h"
// For sanity's sake
template
class vmap<int,int>;

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

// Test a pair of map-like things for 'equality'
template<typename VmapT,
         typename MapT>
bool maps_equal( const VmapT& vmap, const MapT& map )
{
    REQUIRE( map.size() == vmap.size() );
    REQUIRE( vmap.empty() == map.empty() );
    REQUIRE( std::distance(vmap.begin(),vmap.end()) == vmap.size() );
    typename VmapT::const_iterator viter = vmap.begin();
    typename MapT::const_iterator miter = map.begin();
    for( typename MapT::size_type i = 0 ; i < map.size() ; ++i, ++viter, ++miter )
    {
        REQUIRE( viter != vmap.end() );
        REQUIRE( viter->first == miter->first );
        REQUIRE( viter->second == miter->second );
        //REQUIRE( *viter == *miter );
    }
    REQUIRE( viter == vmap.end() );
    REQUIRE( miter == map.end() );
    return true;
}



TEST_CASE( "vmap/default_construct", "Default construct a vmap" )
{
    typedef int key_type;
    typedef int mapped_type;
    vmap<key_type,mapped_type> vmap1;
    REQUIRE( vmap1.empty() );
    REQUIRE( vmap1.size() == 0 );
    REQUIRE( vmap1.begin() == vmap1.end() );
}

TEST_CASE( "vmap/copy_default", "Copy a default-constructed vmap" )
{
    typedef int key_type;
    typedef int mapped_type;
    vmap<key_type,mapped_type> vmap1;
    REQUIRE( vmap1.empty() );
    REQUIRE( vmap1.size() == 0 );
    vmap<key_type,mapped_type> vmap2( vmap1 );
    REQUIRE( vmap2.empty() );
    REQUIRE( vmap2.size() == 0 );
    REQUIRE( maps_equal( vmap1, vmap2 ) );
}

TEST_CASE( "vmap/from_empty_map", "Construct a vmap from an empty map" )
{
    typedef int key_type;
    typedef int mapped_type;
    std::map<key_type,mapped_type> themap;
    vmap<key_type,mapped_type> vmap1( themap );
    REQUIRE( vmap1.empty() );
    REQUIRE( vmap1.size() == 0 );
    REQUIRE( vmap1.empty() == themap.empty() );
    REQUIRE( vmap1.size() == themap.size() );
    REQUIRE( vmap1.begin() == vmap1.end() );
    REQUIRE( maps_equal( vmap1, themap ) );
}

TEST_CASE( "vmap/from_small_map", "Construct a vmap from a small map" )
{
    typedef int key_type;
    typedef int mapped_type;
    std::map<key_type,mapped_type> themap;
    themap[0] = 10;
    themap[1] = 11;
    // Sanity check:
    REQUIRE( themap.begin()->first == 0 );
    REQUIRE( themap.begin()->second == 10 );
    
    vmap<key_type,mapped_type> vmap1( themap );
    REQUIRE( !vmap1.empty() );
    REQUIRE( vmap1.empty() == themap.empty() );
    REQUIRE( vmap1.size() == themap.size() );
    REQUIRE( vmap1.size() == 2 );
    REQUIRE( vmap1.begin() != vmap1.end() );
    REQUIRE( std::distance(vmap1.begin(),vmap1.end()) == vmap1.size() );
    REQUIRE( maps_equal( vmap1, themap ) );
}

TEST_CASE( "vmap/from_small_reverse_map", "Construct a vmap from a reversed map" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::greater<key_type> key_compare;
    std::map<key_type,mapped_type,key_compare> themap;
    themap[0] = 10;
    themap[1] = 11;
    // Sanity check:
    REQUIRE( themap.begin()->first == 1 );
    REQUIRE( themap.begin()->second == 11 );
    vmap<key_type,mapped_type,key_compare> vmap1( themap );
    REQUIRE( !vmap1.empty() );
    REQUIRE( vmap1.empty() == themap.empty() );
    REQUIRE( vmap1.size() == themap.size() );
    REQUIRE( vmap1.size() == 2 );
    REQUIRE( vmap1.begin() != vmap1.end() );
    REQUIRE( std::distance(vmap1.begin(),vmap1.end()) == vmap1.size() );
    REQUIRE( maps_equal( vmap1, themap ) );
}
