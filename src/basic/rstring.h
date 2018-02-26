#ifndef __rstring_hpp__
#define __rstring_hpp__

#include <string>
#include "basicpublic.h"

#pragma warning(disable:4251)

RATEL_NAMESPACE_BEGIN

class RATEL_BASIC_API RString
{
public:
    typedef char char_type;
    typedef std::char_traits<char> traits_type;
    typedef std::allocator<char> allocator_type;    
    typedef typename allocator_type::value_type value_type;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;   

    class const_myiterator 
        : public std::iterator<std::random_access_iterator_tag,
                                RString::value_type,
                                RString::difference_type, 
                                RString::const_pointer, 
                                RString::const_reference>
    {
    public:
        typedef std::iterator<std::random_access_iterator_tag, 
                                RString::value_type, 
                                RString::difference_type, 
                                RString::const_pointer, 
                                RString::const_reference> basecls;
        typedef typename basecls::iterator_category iterator_category;
        typedef typename basecls::value_type value_type;
        typedef typename basecls::difference_type difference_type;
        typedef typename basecls::pointer pointer;
        typedef typename basecls::reference reference;        
        const_myiterator();// construct with null pointer    
        const_myiterator(pointer p); // construct with pointer p
        reference operator*() const;        
        pointer operator->() const;// return pointer to class object        
        const_myiterator& operator++();// preincrement        
        const_myiterator operator++(int);//postincrement
        const_myiterator& operator--();// predecrement
        const_myiterator operator--(int);//postdecrement        
        const_myiterator& operator+=(difference_type off); // increment by integer
        const_myiterator operator+(difference_type off) const; // return this + integer
        const_myiterator& operator-=(difference_type off);
        const_myiterator operator-(difference_type off) const;        
        difference_type operator-(const const_myiterator& rhs) const; // return difference of iterators        
        reference operator[](difference_type off) const; // subscript        
        bool operator==(const const_myiterator& rhs) const;
        bool operator!=(const const_myiterator& rhs) const;
        bool operator<(const const_myiterator& rhs) const;
        bool operator>(const const_myiterator& rhs) const;
        bool operator<=(const const_myiterator& rhs) const;
        bool operator>=(const const_myiterator& rhs) const;
    private:
        pointer curptr_;
    };

    class myiterator : public const_myiterator
    {
    public:
        typedef const_myiterator basecls;
        typedef typename basecls::iterator_category iterator_category;
        typedef typename basecls::value_type value_type;
        typedef typename basecls::difference_type difference_type;
        typedef typename basecls::pointer pointer;
        typedef typename basecls::reference reference;
        myiterator();// construct with null pointer    
        myiterator(pointer p); // construct with pointer p
        reference operator*() const;
        pointer operator->() const;// return pointer to class object        
        myiterator& operator++();// preincrement        
        myiterator operator++(int);//postincrement
        myiterator& operator--();// predecrement
        myiterator operator--(int);//postdecrement        
        myiterator& operator+=(difference_type off); // increment by integer
        myiterator operator+(difference_type off) const; // return this + integer
        myiterator& operator-=(difference_type off);
        myiterator operator-(difference_type off) const;
        difference_type operator-(const myiterator& rhs) const; // return difference of iterators        
        reference operator[](difference_type off) const; // subscript  
    };
    typedef myiterator iterator;
    typedef const_myiterator const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    static const size_type npos;

    size_t size()const;

    // return maximum possible length of sequence
    size_type maxsize() const;
    
    size_t capacity()const;
    
    void resize(size_type newsize);
    void resize(size_type newsize, char ch);

    void reserve(size_type newcap = 0);

    bool empty() const;

    iterator begin();// return iterator for beginning of mutable sequence        
    const_iterator begin() const; // return iterator for beginning of nonmutable sequence
    iterator end(); // return iterator for end of mutable sequence
    const_iterator end() const; // return iterator for end of nonmutable sequence                                                                
    reverse_iterator rbegin(); // return iterator for beginning of reversed mutable sequence
    const_reverse_iterator rbegin() const; // return iterator for beginning of reversed nonmutable sequence
    reverse_iterator rend(); // return iterator for end of reversed mutable sequence
    const_reverse_iterator rend() const; // return iterator for end of reversed nonmutable sequence
    const_iterator cbegin() const; // return iterator for beginning of nonmutable sequence
    const_iterator cend() const; // return iterator for end of nonmutable sequence
    const_reverse_iterator crbegin() const; // return iterator for beginning of reversed nonmutable sequence
    const_reverse_iterator crend() const; // return iterator for end of reversed nonmutable sequence

    // reduce capacity
    //void shrinkToFit();
    // subscript mutable sequence with checking
    reference at(size_type off);
    // subscript nonmutable sequence with checking
    const_reference at(size_type off) const;

    // copy [off, off + cnt) to [dest, dest + cnt)
    size_type copys(char* dest, size_type dsize, size_type cnt, size_type off = 0) const;

    // look for rhs beginning at or after off
    size_type find(const RString& rhs, size_type off = 0) const;
    // look for [strptr, strptr + cnt) beginning at or after off
    size_type find(const char* strptr, size_type off, size_type cnt) const;
    // look for [strptr, <null>) beginning at or after off
    size_type find(const char* strptr, size_type off = 0) const;
    // look for ch at or after off
    size_type find(char ch, size_type off = 0) const;
    // look for rhs beginning before off
    size_type rfind(const RString& rhs, size_type off = npos) const;
    // look for [strptr, strptr + cnt) beginning before off
    size_type rfind(const char* strptr, size_type off, size_type cnt) const;
    // look for [strptr, <null>) beginning before off
    size_type rfind(const char *strptr, size_type off = npos) const;
    // look for ch before off
    size_type rfind(char ch, size_type off = npos) const;
    // look for one of rhs at or after off
    size_type findFirstOf(const RString& rhs, size_type off = 0) const;
    // look for one of [strptr, strptr + cnt) at or after off
    size_type findFirstOf(const char* strptr, size_type off, size_type cnt) const;
    // look for one of [strptr, <null>) at or after off
    size_type findFirstOf(const char *strptr, size_type off = 0) const;
    // look for ch at or after off
    size_type findFirstOf(char ch, size_type off = 0) const;
    // look for one of rhs before off
    size_type findLastOf(const RString& rhs, size_type off = npos) const;
    // look for one of [strptr, strptr + cnt) before off
    size_type findLastOf(const char* strptr, size_type off, size_type cnt) const;
    // look for one of [strptr, <null>) before off
    size_type findLastOf(const char* strptr, size_type off = npos) const;
    // look for ch before off
    size_type findLastOf(char ch, size_type off = npos) const;
    // look for none of rhs at or after off
    size_type findFirstNotOf(const RString& rhs, size_type off = 0) const;
    // look for none of [strptr, strptr + cnt) at or after off
    size_type findFirstNotOf(const char *strptr, size_type off, size_type cnt) const;
    // look for one of [strptr, <null>) at or after off
    size_type findFirstNotOf(const char *strptr, size_type off = 0) const;
    // look for non ch at or after off
    size_type findFirstNotOf(char ch, size_type off = 0) const;
    // look for none of rhs before off
    size_type findLastNotOf(const RString& rhs, size_type off = npos) const;
    // look for none of [strptr, strptr + cnt) before off
    size_type findLastNotOf(const char *strptr, size_type off, size_type cnt) const;
    // look for none of [strptr, <null>) before off
    size_type findLastNotOf(const char *strptr, size_type off = npos) const;
    // look for non ch before off
    size_type findLastNotOf(char ch, size_type off = npos) const;

    RString& assign(const RString& rhs);
    RString& assign(const RString& rhs, size_type roff, size_type cnt = npos);
    // assign [strptr, strptr + cnt)
    RString& assign(const char* strptr, size_type cnt);
    // assign [strptr, <null>)
    RString& assign(const char* strptr);
    // assign cnt * ch
    RString& assign(size_type cnt, char ch);
    // assign [first, last), const pointers
    RString& assign(const_pointer first, const_pointer last);
    // assign [first, last), const_iterators
    RString& assign(const_iterator first, const_iterator last);      

    // return [off, off + cnt) as new string
    RString substr(size_type off = 0, size_type cnt = npos) const;

    // compare [0, size()) with rhs
    size_type compare(const RString& rhs) const;
    // compare [off, off + cnt) with rhs
    size_type compare(size_type off, size_type cnt, const RString& rhs) const;
    // compare [off, off + cnt) with rhs [roff, roff + rcnt)
    size_type compare(size_type off, size_type cnt, const RString& rhs, size_type roff, size_type rcnt = npos) const;
    // compare [0, size()) with [strptr, <null>)
    size_type compare(const char* strptr) const;
    // compare [off, off + cnt) with [strptr, <null>)
    size_type compare(size_type off, size_type cnt, const char* strptr) const;
    // compare [off, off + cnt) with [lhs, lhs + rcnt)
    size_type compare(size_type off, size_type cnt, const char* strptr, size_type rcnt) const;

    // insert rhs at off
    RString& insert(size_type off, const RString& rhs);
    // insert rhs [roff, roff + rcnt) at off
    RString& insert(size_type off, const RString& rhs, size_type roff, size_type rcnt = npos);
    // insert [strptr, strptr + cnt) at off
    RString& insert(size_type off, const char* strptr, size_type cnt);
    // insert [strptr, <null>) at off
    RString& insert(size_type off, const char* strptr);
    // insert cnt * ch at off
    RString& insert(size_type off, size_type cnt, char ch);
    // insert <null> at _Where
    iterator insert(const_iterator where);
    // insert ch at where
    iterator insert(const_iterator where, char ch);
    // insert cnt * ch at _Where
    iterator insert(const_iterator where, size_type cnt, char ch);
    // insert [first, last) at where, iterators
    iterator insert(const_iterator where, iterator first, iterator last);
    // insert [first, last) at where, const pointers
    iterator insert(const_iterator where, const_pointer first, const_pointer last);
    // insert [first, last) at where, const_iterators
    iterator insert(const_iterator where, const_iterator first, const_iterator last);

    // append initializer_list
    RString& append(std::initializer_list<char> chars);
    // append rhs
    RString& append(const RString& rhs);
    // append rhs [roff, roff + cnt)
    RString& append(const RString& rhs, size_type roff, size_type cnt = npos);
    // append [strptr, strptr + cnt)
    RString& append(const char* strptr, size_type cnt);
    // append [strptr, <null>)
    RString& append(const char* strptr);
    // append cnt * ch
    RString& append(size_type cnt, char ch);
    // append [first, last), const pointers
    RString& append(const_pointer first, const_pointer last);
    // append [first, last), const_iterators
    RString& append(const_iterator first, const_iterator last);

    RString& erase(size_type off = 0);
    // erase elements [off, off + cnt)
    RString& erase(size_type off, size_type cnt);
    // erase element at w
    iterator erase(const_iterator w);
    // erase substring [first, last)
    iterator erase(const_iterator first, const_iterator last);

    // replace [off, off + cnt) with _Right
    RString& replace(size_type off, size_type cnt, const RString& rhs);
    // replace [lhs_off, lhs_off + lhs_cnt) with rhs [rhs_off, rhs_off + rhs_cnt)
    RString& replace(size_type lhs_off, size_type lhs_cnt, const RString& rhs, size_type rhs_off, size_type rhs_cnt = npos);
    // replace [off, off + cnt) with [strptr, strptr + cnt)
    RString& replace(size_type off, size_type cnt, const char* strptr, size_type rcnt);
    // replace [off, off + cnt) with [strptr, <null>)
    RString& replace(size_type off, size_type cnt, const char* strptr);
    // replace [off, off + cnt) with ch * chcnt
    RString& replace(size_type off, size_type cnt, size_type chcnt, char ch);
    // replace [first, last) with rhs
    RString& replace(const_iterator first, const_iterator last, const RString& rhs);
    // replace [first, last) with [strptr, strptr + cnt)
    RString& replace(const_iterator first, const_iterator last, const char* strptr, size_type cnt);
    // replace [first, last) with [strptr, <null>)
    RString& replace(const_iterator first, const_iterator last, const char* strptr);
    // replace [first, last) with cnt * ch
    RString& replace(const_iterator first, const_iterator last, size_type cnt, char ch);
    // replace [first, last) with [first2, last2), const pointers
    RString& replace(const_iterator first, const_iterator last, const_pointer first2, const_pointer last2);
    // replace [first, last) with [first2, last2), const pointers
    RString& replace(const_iterator first, const_iterator last, pointer first2, pointer last2);
    // replace [first, last) with [first2, last2), const_iterators
    RString& replace(const_iterator first, const_iterator last, const_iterator first2, const_iterator last2);
    // replace [first, last) with [first2, last2), const_iterators
    RString& replace(const_iterator first, const_iterator last, iterator first2, iterator last2);
        
    // assign [strptr, <null>)
    RString& operator=(const char* strptr);
    RString& operator=(char ch);
    // assign initializer_list
    RString& operator=(std::initializer_list<char> ilist);
    // assign rhs
    RString& operator=(const RString& rhs);
    // append rhs
    RString& operator+=(const RString& rhs);
    // append [strptr, <null>)
    RString& operator+=(const char* strptr);
    // append ch
    RString& operator+=(char ch);
    // append initializer_list
    RString& operator+=(std::initializer_list<char> ilist);
    // subscript mutable sequence
    reference operator[](size_type off);
    // subscript nonmutable sequence
    const_reference operator[](size_type off) const;
        
    // construct empty string
    RString();
    // construct by copying rhs
    RString(const RString& rhs); 
    // construct empty string with allocator                                                
    explicit RString(const allocator_type& al); 
    // construct from rhs [roff, roff + cnt)
    RString(const RString& rhs, size_type roff, size_type cnt = npos);
    // construct from rhs [roff, roff + cnt) with allocator
    RString(const RString& rhs, size_type roff, size_type cnt, const allocator_type& al);        
    // construct from [strptr, <null>)
    RString(const char* strptr);
    // construct from [strptr, <null>) with allocator
    RString(const char* strptr, const allocator_type& al);
    // construct from [strptr, strptr + cnt)
    RString(const char* strptr, size_type cnt);
    // construct from [strptr, strptr + cnt) with allocator
    RString(const char* strptr, size_type cnt, const allocator_type& al);
    // construct from cnt * ch
    RString(size_type cnt, char ch);
    // construct from cnt * ch with allocator
    RString(size_type cnt, char ch, const allocator_type& al);
    // construct by moving rhs
    RString(RString&& rhs);
    // construct by moving rhs, allocator
    RString(RString&& rhs, const allocator_type& al);    
    ~RString();

private:
    // ensure buffer is big enough, trim to size if trim is true
    bool grow(size_type newsize, bool trim = false);
    // copy oldsize elements to newly allocated buffer
    void copy(size_type newsize, size_type oldsize);    
    // test if p points inside string
    bool inside(const char* p)const { return !(p == nullptr_t{} || p < val_.dataptr() || val_.dataptr() + val_.cur_size <= p); }
    // checks whether off is in the bounds of [0, val_.cur_size]
    void checkOffset(const size_type off) const;
    // trims source_size to the longest it can be assuming a string starting at off
    size_type getSuffixSize(const size_type off, const size_type source_size) const;
    // assign cnt copies of ch beginning at off
    void charAssign(size_type off, size_type cnt, char ch);
    // initialize buffer, deallocating any storage
    void tidy(bool built = false, size_type newsize = 0);
    // set new length and null terminator
    void truncate(size_type newsize);

    void assignMove(RString&& rhs);

    // compare [lhs, lhs + lsize) to [rhs, rhs + rsize) using traits
    size_type traitsCompare(const char* const lhs, const size_type lsize, const char* const rhs, const size_type rsize)const;

    char* dataptr();
    const char* dataptr()const;

    struct RStringVal
    {
        char* dataptr(); // determine current pointer to buffer for mutable string
        const char* dataptr() const; // determine current pointer to buffer for nonmutable string        
        enum { BUFFER_SIZE = 16 }; // length of internal buffer:16        
        enum { ALLOC_MASK = 15 }; //{	// roundup mask for allocated buffers: 15
        RStringVal();
        union _DataTag // storage for small buffer or pointer to larger one
        {
            _DataTag() {}
            ~_DataTag() {}
            char buffer[BUFFER_SIZE];
            char* ptr;
            char alias[BUFFER_SIZE];	// to permit aliasing
        }data;
        size_t cur_size;	// current length of string
        size_t reserve_size;	// current storage reserved for string
    }val_;    
    allocator_type allocator_;
};

inline RString operator+(const RString& lhs, const RString& rhs)
{	// return string + string
    RString res;
    res.reserve(lhs.size() + rhs.size());
    res += lhs;
    res += rhs;
    return res;
}

inline RString operator+(const char* lhs, const RString& rhs)
{	// return NTCS + string
    RString res;
    res.reserve(RString::traits_type::length(lhs) + rhs.size());
    res += lhs;
    res += rhs;
    return res;
}

// return character + string
inline RString operator+(const char lhs, const RString& rhs)
{	
    RString res;
    res.reserve(1 + rhs.size());
    res += lhs;
    res += rhs;
    return res;
}

inline RString operator+(const RString& lhs, const char* rhs)
{	// return string + NTCS
    RString res;
    res.reserve(lhs.size() + RString::traits_type::length(rhs));
    res += lhs;
    res += rhs;
    return res;
}

inline RString operator+(const RString& lhs, const char ch)
{	// return string + character
    RString res;
    res.reserve(lhs.size() + 1);
    res += lhs;
    res += ch;
    return res;
}

inline RString operator+(const RString& lhs, RString&& rhs)
{	// return string + string
    return std::move(rhs.insert(0, lhs));
}

inline RString operator+(RString&& lhs, const RString& rhs)
{	// return string + string
    return std::move(lhs.append(rhs));
}

inline RString operator+(RString&& lhs, RString&& rhs)
{	// return string + string
    if (rhs.size() <= lhs.capacity() - lhs.size() || rhs.capacity() - rhs.size() < lhs.size())
        return std::move(lhs.append(rhs));
    else
        return std::move(rhs.insert(0, lhs));
}

inline RString operator+(const char* lhs, RString&& rhs)
{	// return NTCS + string
    return std::move(rhs.insert(0, lhs));
}

inline RString operator+(const char ch, RString&& rhs)
{	// return character + string
    typedef typename RString::size_type
        size_type;
    return std::move(rhs.insert((size_type)0, (size_type)1, ch));
}

inline RString operator+(RString&& lhs, const char* rhs)
{	// return string + NTCS
    return std::move(lhs.append(rhs));
}

inline RString operator+(RString&& lhs, const char ch)
{	// return string + character
    return std::move(lhs.append(1, ch));
}

inline bool operator==(const RString& lhs, const RString& rhs) 
{	// test for string equality
    return lhs.compare(rhs) == 0;
}

inline bool operator==(const char* lhs, const RString& rhs)
{	// test for NTCS vs. string equality
    return rhs.compare(lhs) == 0;
}

inline bool operator==(const RString& lhs, const char* rhs)
{	// test for string vs. NTCS equality
    return lhs.compare(rhs) == 0;
}

inline bool operator!=(const RString& lhs, const RString& rhs) 
{	// test for string inequality
    return !(lhs == rhs);
}

inline bool operator!=(const char* lhs, const RString& rhs)
{	// test for NTCS vs. string inequality
    return !(lhs == rhs);
}

inline bool operator!=(const RString& lhs, const char* rhs)
{	// test for string vs. NTCS inequality
    return !(lhs == rhs);
}

inline bool operator<(const RString& lhs, const RString& rhs) 
{	// test if string < string
    return lhs.compare(rhs) < 0;
}

inline bool operator<(const char* lhs, const RString& rhs)
{	// test if NTCS < string
    return rhs.compare(lhs) > 0;
}

inline bool operator<(const RString& lhs, const char* rhs)
{	// test if string < NTCS
    return lhs.compare(rhs) < 0;
}

inline bool operator>(const RString& lhs, const RString& rhs) 
{	// test if string > string
    return rhs < lhs;
}

inline bool operator>(const char* lhs, const RString& rhs)
{	// test if NTCS > string
    return rhs < lhs;
}

inline bool operator>(const RString& lhs, const char* rhs)
{	// test if string > NTCS
    return rhs < lhs;
}

inline bool operator<=(const RString& lhs, const RString& rhs) 
{	// test if string <= string
    return !(rhs < lhs);
}

inline bool operator<=(const char* lhs, const RString& rhs)
{	// test if NTCS <= string
    return !(rhs < lhs);
}

inline bool operator<=(const RString& lhs, const char* rhs)
{	// test if string <= NTCS
    return !(rhs < lhs);
}

inline bool operator>=(const RString& lhs, const RString& rhs) 
{	// test if string >= string
    return !(lhs < rhs);
}

inline bool operator>=(const char* lhs, const RString& rhs)
{	// test if NTCS >= string
    return !(lhs < rhs);
}

inline bool operator>=(const RString& lhs, const char* rhs)
{	// test if string >= NTCS
    return (!(lhs < rhs));
}

RATEL_NAMESPACE_END

#endif
