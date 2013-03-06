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
    REQUIRE( vmap1.begin()->first == 1 );
    REQUIRE( vmap1.begin()->second == 11 );
    REQUIRE( vmap1.rbegin()->first == 0 );
    REQUIRE( vmap1.rbegin()->second == 10 );
    REQUIRE( std::distance(vmap1.begin(),vmap1.end()) == vmap1.size() );
    REQUIRE( maps_equal( vmap1, themap ) );
}

// Construct a std::map for the lb tests
std::map<int,int> lb_map()
{
    std::map<int,int> map;
    map[-10] = 1;
    map[ -5] = 2;
    map[  0] = 3;
    map[  5] = 4;
    map[ 10] = 5;
    REQUIRE( map.size() == 5 );
    return map;
}

TEST_CASE( "vmap/lower_bound_less", "Check lower_bound, key less than all values" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( lb_map() );
    vmap_type vmap1( amap );
    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.lower_bound(-15) != amap.end() );
    REQUIRE( amap.lower_bound(-15)->first == -10 );
    REQUIRE( vmap1.lower_bound(-15) != vmap1.end() );
    REQUIRE( vmap1.lower_bound(-15)->first == -10 );
}

TEST_CASE( "vmap/lower_bound_begin", "Check lower_bound, key equal to smallest values" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( lb_map() );
    vmap_type vmap1( amap );
    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.lower_bound(-10) != amap.end() );
    REQUIRE( amap.lower_bound(-10)->first == -10 );
    REQUIRE( vmap1.lower_bound(-10) != vmap1.end() );
    REQUIRE( vmap1.lower_bound(-10)->first == -10 );
}

TEST_CASE( "vmap/lower_bound_inside", "Check lower_bound, key in range, but not present" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( lb_map() );
    vmap_type vmap1( amap );

    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.lower_bound(-7) != amap.end() );
    REQUIRE( amap.lower_bound(-7)->first == -5 );
    REQUIRE( vmap1.lower_bound(-7) != vmap1.end() );
    REQUIRE( vmap1.lower_bound(-7)->first == 0 );
}

TEST_CASE( "vmap/lower_bound_present", "Check lower_bound, key in map" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( lb_map() );
    vmap_type vmap1( amap );

    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.lower_bound(5) != amap.end() );
    REQUIRE( amap.lower_bound(5)->first == 5 );
    // REQUIRE( vmap1.lower_bound(5) != vmap1.end() );
    // REQUIRE( vmap1.lower_bound(5)->first == 5 );
}

TEST_CASE( "vmap/lower_bound_upper", "Check lower_bound, key is highest value in map" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( lb_map() );
    vmap_type vmap1( amap );

    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.lower_bound(10) != amap.end() );
    REQUIRE( amap.lower_bound(10)->first == 10 );
    REQUIRE( vmap1.lower_bound(10) != vmap1.end() );
    REQUIRE( vmap1.lower_bound(10)->first == 0 );
}

TEST_CASE( "vmap/lower_bound_above", "Check lower_bound, key is greater than values in map" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( lb_map() );
    vmap_type vmap1( amap );

    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.lower_bound(15) == amap.end() );
    REQUIRE( vmap1.lower_bound(15) == vmap1.end() );
}
