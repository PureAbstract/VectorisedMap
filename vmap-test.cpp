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
    while( miter != map.end() )
    {
        REQUIRE( viter != vmap.end() );
        //REQUIRE( *viter == *miter );
        REQUIRE( viter->first == miter->first );
        REQUIRE( viter->second == miter->second );
        ++miter;
        ++viter;
    }
    REQUIRE( viter == vmap.end() );
    REQUIRE( miter == map.end() );
    return true;
}



TEST_CASE( "vmap/ctor/default", "Default construct a vmap" )
{
    typedef int key_type;
    typedef int mapped_type;
    vmap<key_type,mapped_type> vmap1;
    REQUIRE( vmap1.empty() );
    REQUIRE( vmap1.size() == 0 );
    REQUIRE( vmap1.begin() == vmap1.end() );
}

TEST_CASE( "vmap/ctor/copy_default", "Copy a default-constructed vmap" )
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

TEST_CASE( "vmap/ctor/from_empty_map", "Construct a vmap from an empty map" )
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


TEST_CASE( "vmap/ctor/from_small_map", "Construct a vmap from a small map" )
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

TEST_CASE( "vmap/ctor/copy_small", "Copy a vmap constructed from a small map" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type>  vmap_type;

    map_type map;
    map[-1] = -10;
    map[ 0] =  10;
    map[ 1] =  20;
    // Sanity:
    REQUIRE( map.size() == 3 );
    REQUIRE( map.begin()->first == -1 );

    // Copy the map...
    vmap_type vmap1( map );
    REQUIRE( maps_equal( vmap1, map ) );
    // Copy the vmap
    vmap_type vmap2( vmap1 );
    REQUIRE( maps_equal( vmap1, map ) );
    REQUIRE( maps_equal( vmap2, map ) );
    REQUIRE( maps_equal( vmap1, vmap2 ) );
    
}


TEST_CASE( "vmap/ctor/from_small_reverse_map", "Construct a vmap from a reversed map" )
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

// Construct a std::map for the lower/upper bound tests
std::map<int,int> bounds_map()
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

TEST_CASE( "vmap/ctor/move", "Ensure the move ctor actually moves!" )
{
#ifdef VMAP_CONFIG_MOVE
    std::map<int,int> amap(bounds_map());
    vmap<int,int> vec1(amap);
    REQUIRE( maps_equal( vec1, amap ) );
    REQUIRE(!vec1.empty());
    vmap<int,int> vec2(std::move(vec1));
    REQUIRE( maps_equal( vec2, amap ) );
    REQUIRE(vec1.empty());
#else
    WARN( "Move semantics not enabled" );
#endif
}


TEST_CASE( "vmap/lower_bound/less", "Check lower_bound, key less than all values" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( bounds_map() );
    vmap_type vmap1( amap );
    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.lower_bound(-15) != amap.end() );
    REQUIRE( amap.lower_bound(-15)->first == -10 );
    REQUIRE( vmap1.lower_bound(-15) != vmap1.end() );
    REQUIRE( vmap1.lower_bound(-15)->first == -10 );
}

TEST_CASE( "vmap/lower_bound/begin", "Check lower_bound, key equal to smallest values" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( bounds_map() );
    vmap_type vmap1( amap );
    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.lower_bound(-10) != amap.end() );
    REQUIRE( amap.lower_bound(-10)->first == -10 );
    REQUIRE( vmap1.lower_bound(-10) != vmap1.end() );
    REQUIRE( vmap1.lower_bound(-10)->first == -10 );
}

TEST_CASE( "vmap/lower_bound/inside", "Check lower_bound, key in range, but not present" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( bounds_map() );
    vmap_type vmap1( amap );

    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.lower_bound(-7) != amap.end() );
    REQUIRE( amap.lower_bound(-7)->first == -5 );
    REQUIRE( vmap1.lower_bound(-7) != vmap1.end() );
    REQUIRE( vmap1.lower_bound(-7)->first == -5 );
}

TEST_CASE( "vmap/lower_bound/present", "Check lower_bound, key in map" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( bounds_map() );
    vmap_type vmap1( amap );

    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.lower_bound(5) != amap.end() );
    REQUIRE( amap.lower_bound(5)->first == 5 );
    REQUIRE( vmap1.lower_bound(5) != vmap1.end() );
    REQUIRE( vmap1.lower_bound(5)->first == 5 );
}

TEST_CASE( "vmap/lower_bound/upper", "Check lower_bound, key is highest value in map" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( bounds_map() );
    vmap_type vmap1( amap );

    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.lower_bound(10) != amap.end() );
    REQUIRE( amap.lower_bound(10)->first == 10 );
    REQUIRE( vmap1.lower_bound(10) != vmap1.end() );
    REQUIRE( vmap1.lower_bound(10)->first == 10 );
}

TEST_CASE( "vmap/lower_bound/above", "Check lower_bound, key is greater than values in map" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( bounds_map() );
    vmap_type vmap1( amap );

    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.lower_bound(15) == amap.end() );
    REQUIRE( vmap1.lower_bound(15) == vmap1.end() );
}
    
TEST_CASE( "vmap/upper_bound/less", "Check upper_bound, key less than all values" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( bounds_map() );
    vmap_type vmap1( amap );
    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.upper_bound(-15) != amap.end() );
    REQUIRE( amap.upper_bound(-15)->first == -10 );
    REQUIRE( vmap1.upper_bound(-15) != vmap1.end() );
    REQUIRE( vmap1.upper_bound(-15)->first == -10 );
}

TEST_CASE( "vmap/upper_bound/begin", "Check upper_bound, key equal to smallest values" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( bounds_map() );
    vmap_type vmap1( amap );
    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.upper_bound(-10) != amap.end() );
    REQUIRE( amap.upper_bound(-10)->first == -5 );
    REQUIRE( vmap1.upper_bound(-10) != vmap1.end() );
    REQUIRE( vmap1.upper_bound(-10)->first == -5 );
}

TEST_CASE( "vmap/upper_bound/inside", "Check upper_bound, key in range, but not present" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( bounds_map() );
    vmap_type vmap1( amap );

    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.upper_bound(-7) != amap.end() );
    REQUIRE( amap.upper_bound(-7)->first == -5 );
    REQUIRE( vmap1.upper_bound(-7) != vmap1.end() );
    REQUIRE( vmap1.upper_bound(-7)->first == -5 );
}

TEST_CASE( "vmap/upper_bound/present", "Check upper_bound, key in map" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( bounds_map() );
    vmap_type vmap1( amap );

    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.upper_bound(5) != amap.end() );
    REQUIRE( amap.upper_bound(5)->first == 10 );
    REQUIRE( vmap1.upper_bound(5) != vmap1.end() );
    REQUIRE( vmap1.upper_bound(5)->first == 10 );
}

TEST_CASE( "vmap/upper_bound/upper", "Check upper_bound, key is highest value in map" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( bounds_map() );
    vmap_type vmap1( amap );

    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.upper_bound(10) == amap.end() );
    REQUIRE( vmap1.upper_bound(10) == vmap1.end() );
}

TEST_CASE( "vmap/upper_bound/above", "Check upper_bound, key is greater than values in map" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap( bounds_map() );
    vmap_type vmap1( amap );

    REQUIRE( maps_equal( vmap1, amap ) );
    REQUIRE( amap.upper_bound(15) == amap.end() );
    REQUIRE( vmap1.upper_bound(15) == vmap1.end() );
}

TEST_CASE( "vmap/find/fail", "find : key not in map" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap(bounds_map());
    vmap_type vmap(amap);

    REQUIRE( maps_equal(amap,vmap) );

    REQUIRE( amap.find(-20) == amap.end() );
    REQUIRE( vmap.find(-20) == vmap.end() );

    REQUIRE( amap.find(-7) == amap.end() );
    REQUIRE( vmap.find(-7) == vmap.end() );

    REQUIRE( amap.find(7) == amap.end() );
    REQUIRE( vmap.find(7) == vmap.end() );

    REQUIRE( amap.find(20) == amap.end() );
    REQUIRE( vmap.find(20) == vmap.end() );
}

TEST_CASE( "vmap/find/success", "find : key in map" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap(bounds_map());
    vmap_type vmap(amap);

    REQUIRE( maps_equal(amap,vmap) );
    for( map_type::const_iterator iter = amap.begin() ;
         iter != amap.end() ;
         ++iter )
    {
        const key_type& key = iter->first;
        REQUIRE( amap.find(key) != amap.end() );
        REQUIRE( amap.find(key) == iter );
        REQUIRE( amap.find(key)->first == key );
        REQUIRE( vmap.find(key) != vmap.end() );
        REQUIRE( vmap.find(key)->first == key );
    }
}

TEST_CASE( "vmap/get/fail", "get : key not present" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap(bounds_map());
    vmap_type vmap(amap);

    REQUIRE( maps_equal( vmap, amap ) );
    REQUIRE( vmap.get(-99) == key_type() );
    REQUIRE( vmap.get(-99,-99) == -99 );
}

TEST_CASE( "vmap/get/pass", "get : key not present" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap(bounds_map());
    vmap_type vmap(amap);

    REQUIRE( maps_equal( vmap, amap ) );
    REQUIRE( vmap.get(-10) == 1 );
    REQUIRE( vmap.get(-10,-99) == 1 );
    REQUIRE( vmap.get(-5) == 2 );
    REQUIRE( vmap.get(-5,-99) == 2 );
    REQUIRE( vmap.get(0) == 3 );
    REQUIRE( vmap.get(0,-99) == 3 );
    REQUIRE( vmap.get(5) == 4 );
    REQUIRE( vmap.get(5,-99) == 4 );
    REQUIRE( vmap.get(10) == 5 );
    REQUIRE( vmap.get(10,-99) == 5 );
}


TEST_CASE( "vmap/equal_range/none", "equal_range : key not present" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    typedef std::pair<vmap_type::const_iterator,vmap_type::const_iterator> range_type;

    map_type amap(bounds_map());
    vmap_type vmap(amap);

    REQUIRE( maps_equal( vmap, amap ) );
    range_type r = vmap.equal_range(-42);
    REQUIRE( r.first == r.second );
    r = vmap.equal_range(-7);
    REQUIRE( r.first == r.second );
    r = vmap.equal_range(7);
    REQUIRE( r.first == r.second );
    r = vmap.equal_range(42);
    REQUIRE( r.first == r.second );
}

TEST_CASE( "vmap/equal_range/found", "equal_range : key present" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    typedef std::pair<vmap_type::const_iterator,vmap_type::const_iterator> range_type;

    map_type amap(bounds_map());
    vmap_type vmap(amap);

    REQUIRE( maps_equal( vmap, amap ) );
    range_type r = vmap.equal_range(-10);
    REQUIRE( r.first != r.second );
    REQUIRE( (r.first+1) == r.second );
    REQUIRE( r.first->first == -10 );

    r = vmap.equal_range(-5);
    REQUIRE( r.first != r.second );
    REQUIRE( (r.first+1) == r.second );
    REQUIRE( r.first->first == -5 );

    r = vmap.equal_range(0);
    REQUIRE( r.first != r.second );
    REQUIRE( (r.first+1) == r.second );
    REQUIRE( r.first->first == 0 );

    r = vmap.equal_range(5);
    REQUIRE( r.first != r.second );
    REQUIRE( (r.first+1) == r.second );
    REQUIRE( r.first->first == 5 );

    r = vmap.equal_range(10);
    REQUIRE( r.first != r.second );
    REQUIRE( (r.first+1) == r.second );
    REQUIRE( r.first->first == 10 );

}


TEST_CASE( "vmap/at/pass", "at: Key is present" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap(bounds_map());
    vmap_type vmap(amap);

    REQUIRE( vmap.at(-10) == 1 );
    REQUIRE( vmap.at(-5) == 2 );
    REQUIRE( vmap.at(0) == 3 );
    REQUIRE( vmap.at(5) == 4 );
    REQUIRE( vmap.at(10) == 5 );
}


TEST_CASE( "vmap/at/fail", "at: Key not present" )
{
    typedef int key_type;
    typedef int mapped_type;
    typedef std::map<key_type,mapped_type> map_type;
    typedef vmap<key_type,mapped_type> vmap_type;

    map_type amap(bounds_map());
    vmap_type vmap(amap);

    REQUIRE_THROWS_AS( vmap.at(-42), std::out_of_range );
    REQUIRE_THROWS_AS( vmap.at(-7), std::out_of_range );
    REQUIRE_THROWS_AS( vmap.at(25), std::out_of_range );
}
