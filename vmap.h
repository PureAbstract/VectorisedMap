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

    iterator               begin()          noexcept { return vector_.begin();  }
    iterator               end()            noexcept { return vector_.end();    }
    reverse_iterator       rbegin()         noexcept { return vector_.rbegin(); }
    reverse_iterator       rend()           noexcept { return vector_.rend();   }

    const_iterator         begin()    const noexcept { return vector_.begin();  }
    const_iterator         end()      const noexcept { return vector_.end();    }
    const_reverse_iterator rbegin()   const noexcept { return vector_.rbegin(); }
    const_reverse_iterator rend()     const noexcept { return vector_.rend();   }
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
        const_iterator start = begin();
        size_type length = size();
        while( length > 0 )
        {
            const size_type offset = length / 2;
            const const_iterator midpt = start + offset;
            const value_type& value = *midpt;
            if( compare_( value.first, key ) )
            {
                // value < key - search the upper half
                start = midpt + 1;
                length -= offset+1;
            }
            else
            {
                // value >= key; search the lower half
                length = offset;
            }
        }
        return start;
    }

    const_iterator upper_bound( const key_type& key ) const noexcept
    {
        const_iterator start = begin();
        size_type length = size();
        while( length > 0 )
        {
            const size_type offset = length / 2;
            const const_iterator midpt = start + offset;
            const value_type& value = *midpt;
            if( !compare_( key, value.first ) )
            {
                start = midpt+1;
                length -= offset+1;
            }
            else
            {
                length = offset;
            }
        }
        return start;
    }    

    std::pair<const_iterator,const_iterator> equal_range( const key_type& key ) const noexcept
    {
        const_iterator start = begin();
        size_type length = size();
        while( length > 0 )
        {
            const size_type offset = length/2;
            const const_iterator midpt = start+offset;
            const value_type& value = *midpt;
            if( compare_( value.first, key ) )
            {
                // value < key - go high
                start = midpt+1;
                length -= offset+1;
            }
            else if( compare_( key, value.first ) )
            {
                // key < value - go low
                length = offset;
            }
            else
            {
                // got it
                return std::make_pair(midpt,midpt+1);
            }
        }
        return std::make_pair(end(),end());
    }

    const_iterator find( const key_type& key ) const noexcept
    {
        const const_iterator iter = lower_bound(key);
        if( iter != end() )
        {
            if( !compare_(key,iter->first) )
                return iter;
        }
        return end();
    }

    // Return the mapped value, or throw std::out_of_range
    const mapped_type& at( const key_type& key ) const
    {
        const const_iterator iter = find(key);
        if( iter == end() )
        {
            throw std::out_of_range("vmap: key not found"); // FAIL
        }
        return iter->second;
    }

    // I find these to be handy:

    // Return the mapped value for key, or defalt if non present
    const mapped_type& get( const key_type& key, const mapped_type& defalt ) const noexcept
    {
        const_iterator iter = find(key);
        return ( iter == end() ) ? defalt : iter->second;
    }
    // Return the mapped value for key, or mapped_type() if non present
    mapped_type get( const key_type& key ) const noexcept
    {
        const_iterator iter = find(key);
        return ( iter == end() ) ? mapped_type() : iter->second;
    }
    
private:
    impl_type vector_;
    key_compare compare_;
};
#if __cplusplus < 201103L
#undef noexcept
#endif
#endif /* #ifndef vmap_h_included */
