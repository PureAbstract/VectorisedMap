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
#if __cplusplus < 201103L
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
    // FIXME: This should be const key_type
    typedef std::pair<key_type,mapped_type> value_type;

    // FIXME: Implementation details: Probably ought to privatise these...
    typedef vmap<key_type,mapped_type,key_compare,allocator_type> this_type;
    typedef std::vector<value_type,allocator_type> impl_type;
    
    typedef typename impl_type::size_type size_type;

    typedef typename impl_type::const_iterator         iterator;
    typedef typename impl_type::const_iterator         const_iterator;
    typedef typename impl_type::const_reverse_iterator reverse_iterator;
    typedef typename impl_type::const_reverse_iterator const_reverse_iterator;

    // defaults are ok for:
    // - dtor
    // - copy ctor
    // - move ctor
    // - assign
    // - move assign

    // - default ctor (but what use is it?)
    vmap() {}

    // TODO: templatize on map's Allocator
    // Q: Should be allow that to differ from vmap's allocator?
    // A: Yes.
    explicit vmap( const std::map<key_type,mapped_type,key_compare>& map )
      : compare_( map.key_comp() )
    {
        vector_.reserve(map.size());
        vector_.insert(vector_.end(),map.begin(),map.end());
    }


    // The usual suspects...
    size_type size() const noexcept
    { return vector_.size(); }
    bool empty() const noexcept
    { return vector_.empty(); }
    size_type max_size() const noexcept
    { return vector_.max_size(); }

    allocator_type get_allocator() const noexcept
    { return vector_.get_allocator(); }
    key_compare key_comp() const noexcept
    { return compare_; }

    iterator               begin()    const noexcept { return vector_.begin();  }
    iterator               end()      const noexcept { return vector_.end();    }
    reverse_iterator       rbegin()   const noexcept { return vector_.rbegin(); }
    reverse_iterator       rend()     const noexcept { return vector_.rend();   }
#if __cplusplus >= 201103L
    const_iterator         cbegin()   const noexcept { return vector_.cbegin();  }
    const_iterator         cend()     const noexcept { return vector_.cend();    }
    const_reverse_iterator crbegin()  const noexcept { return vector_.crbegin(); }
    const_reverse_iterator crend()    const noexcept { return vector_.crend();   }
#else
    const_iterator         cbegin()   const noexcept { return vector_.begin();  }
    const_iterator         cend()     const noexcept { return vector_.end();    }
    const_reverse_iterator crbegin()  const noexcept { return vector_.rbegin(); }
    const_reverse_iterator crend()    const noexcept { return vector_.rend();   }
#endif

    const_iterator lower_bound( const key_type& key ) const noexcept
    {
        return end(); // FAIL
    }
    const_iterator upper_bound( const key_type& key ) const noexcept
    {
        return end(); // FAIL
    }    
    std::pair<const_iterator,const_iterator> equal_range( const key_type& key ) const noexcept
    {
        // This is workable, but non optimal.
        return std::make_pair( lower_bound(key),
                               upper_bound(key) );
    }
    const_iterator find( const key_type& key ) const noexcept
    {
        return end(); // Fail
    }

    const mapped_type& at( const key_type& key ) const
    {
        throw std::out_of_range("vmap: key not found"); // FAIL
    }

    // I find these to be handy:

    // Return the mapped value for key, or defalt if non present
    const mapped_type& get( const key_type& key, const mapped_type& defalt ) const noexcept
    {
        return defalt; // FAIL
    }
    // Return the mapped value for key, or mapped_type() if non present
    mapped_type get( const key_type& key ) const noexcept
    {
        return mapped_type(); // FAIL
    }
    
private:
    impl_type vector_;
    key_compare compare_;
};
#if __cplusplus < 201103L
#undef noexcept
#endif
#endif /* #ifndef vmap_h_included */