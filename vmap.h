#ifndef vmap_h_included
#define vmap_h_included
/*
  A vectorize version of std::map

  Has the read-only functionality of std::map, most things are still
  O(logN), but because the underlying implementation is a vector,
  things go a little bit quicker.

  Primary use-case is:
  1) Build a std::map in the usual fashion
  2) Convert it to a vmap
  3) use vmap for lookups
*/
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <stdexcept>
#ifndef cpp_0x
#define noexcept
#endif

template<typename KeyType
        ,typename MappedType
        ,typename Predicate = std::less<KeyType>
        ,typename Allocator = std::allocator<std::pair<KeyType,MappedType> >
        >
class vmap
{
public:
    typedef KeyType key_type;
    typedef MappedType mapped_type;
    typedef Predicate key_compare;
    typedef Allocator allocator_type;
    typedef std::pair<key_type,mapped_type> value_type;
    typedef vmap<key_type,mapped_type,key_compare,allocator_type> this_type;
    typedef std::vector<value_type,allocator_type> impl_type;
    typedef typename impl_type::size_type size_type;

    typedef typename impl_type::const_iterator         iterator;
    typedef typename impl_type::const_iterator         const_iterator;
    typedef typename impl_type::const_reverse_iterator reverse_iterator;
    typedef typename impl_type::const_reverse_iterator const_reverse_iterator;

    // defaults are ok for:
    // - dtor
    // - default ctor (but what use is it?)
    // - copy ctor
    // - move ctor
    // - assign
    // - move assign
    
    // TODO: templatize on map's Allocator
    // Q: Should be allow that to differ from vmap's allocator?
    explicit vmap( const std::map<key_type,mapped_type,key_compare>& map );

    // The usual suspects...
    size_type size() const noexcept;
    bool empty() const noexcept;
    size_type max_size() const noexcept;

    allocator_type get_allocator() const noexcept;
    key_compare key_comp() const noexcept;

    iterator               begin()    const noexcept;
    iterator               end()      const noexcept;
    reverse_iterator       rbegin()   const noexcept;
    reverse_iterator       rend()     const noexcept;
#ifdef cpp_0x
    const_iterator         cbegin()   const noexcept;
    const_iterator         cend()     const noexcept;
    const_reverse_iterator crbegin()  const noexcept;
    const_reverse_iterator crend()    const noexcept;
#endif

    const_iterator lower_bound( const key_type& key ) const noexcept;
    const_iterator upper_bound( const key_type& key ) const noexcept;
    std::pair<const_iterator,const_iterator> equal_range( const key_type& key ) const noexcept;
    const_iterator find( const key_type& key ) const noexcept;

    const mapped_type& at( const key_type& key ) const;

    // I find these to be handy
    const mapped_type& get( const key_type& key, const mapped_type& defalt ) const noexcept;
    mapped_type get( const key_type& key ) const noexcept;
    
private:
    impl_type vector_;
    key_compare compare_;
};

#endif /* #ifndef vmap_h_included */
