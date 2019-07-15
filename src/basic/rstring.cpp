
/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: rstring.cpp 
=========================================================================*/
#include "rstring.h"
#include <cstdarg>
#include <codecvt>
#include <locale>
extern "C"{
#ifdef WIN32
#include <objbase.h>
#else
#include <uuid/uuid.h>
#endif
}
using namespace std;

RATEL_NAMESPACE_BEGIN

#define assert_pointer(ptr)\
    if(!ptr) \
        throw std::invalid_argument("invalid null pointer!");

#define assert_cond_pointer(cond, ptr)\
    if((cond) && !ptr) \
        throw std::invalid_argument("invalid null pointer!"); 

const RString::size_type RString::npos = -1;

namespace {
    const int32_t MAX_STR_BUFFER_SIZE = 250;
    using LocaleCodecvt = std::codecvt_byname<wchar_t, char, mbstate_t>;
}

char* RString::dataptr()
{
    return val_.dataptr();
}

const char* RString::dataptr() const
{
    return val_.dataptr();
}

size_t RString::size() const
{
    return val_.cur_size;
}

size_t RString::capacity() const
{
    return val_.reserve_size;
}

RString::size_type RString::maxsize() const
{
    const size_type max_alloc_num = allocator_.max_size();
    return (max_alloc_num <= 1 ? 1 : max_alloc_num - 1);
}

RString::const_myiterator::const_myiterator()
    :curptr_(nullptr)
{
}

RString::const_myiterator::const_myiterator(pointer p)
    :curptr_(p)
{
}

RString::const_myiterator::reference RString::const_myiterator::operator*() const
{
    assert_pointer(curptr_);
    return *curptr_;
}

RString::const_myiterator::pointer RString::const_myiterator::operator->() const
{
    return curptr_;
}

RString::const_myiterator& RString::const_myiterator::operator++()
{
    ++curptr_;
    return *this;
}

RString::const_myiterator RString::const_myiterator::operator++(int)
{
    const_myiterator tmp = *this;
    ++*this;
    return tmp;
}

RString::const_myiterator& RString::const_myiterator::operator--()
{
    --curptr_;
    return *this;
}

RString::const_myiterator RString::const_myiterator::operator--(int)
{
    const_myiterator tmp = *this;
    --*this;
    return tmp;
}

RString::const_myiterator& RString::const_myiterator::operator+=(difference_type off)
{
    curptr_ += off;
    return *this;
}

RString::const_myiterator RString::const_myiterator::operator+(difference_type off) const
{
    const_myiterator tmp = *this;
    return tmp += off;
}

RString::const_myiterator& RString::const_myiterator::operator-=(difference_type off)
{
    return *this += -off;
}

RString::const_myiterator RString::const_myiterator::operator-(difference_type off) const
{
    const_myiterator tmp = *this;
    return tmp -= off;
}

RString::const_myiterator::difference_type RString::const_myiterator::operator-(const const_myiterator& rhs) const
{
    return curptr_ - rhs.curptr_;
}

RString::const_myiterator::reference RString::const_myiterator::operator[](RString::const_myiterator::difference_type off) const // subscript
{
    return *(*this + off);
}

bool RString::const_myiterator::operator==(const const_myiterator& rhs) const
{
    // test for iterator equality
    return curptr_ == rhs.curptr_;
}

bool RString::const_myiterator::operator!=(const const_myiterator& rhs) const
{
    // test for iterator inequality
    return !(*this == rhs);
}

bool RString::const_myiterator::operator<(const const_myiterator& rhs) const
{
    // test if this < rhs
    return curptr_ < rhs.curptr_;
}

bool RString::const_myiterator::operator>(const const_myiterator& rhs) const
{
    // test if this > rhs
    return rhs < *this;
}

bool RString::const_myiterator::operator<=(const const_myiterator& rhs) const
{
    // test if this <= rhs
    return !(rhs < *this);
}

bool RString::const_myiterator::operator>=(const const_myiterator& rhs) const
{
    // test if this >= rhs
    return !(*this < rhs);
}

RString::myiterator::myiterator()
{
}

RString::myiterator::myiterator(pointer p)
    :basecls(p)
{
}

RString::myiterator::reference RString::myiterator::operator*() const
{
    return (reference)**(basecls*)this;
}

RString::myiterator::pointer RString::myiterator::operator->() const
{
    return std::pointer_traits<pointer>::pointer_to(**this);
}

RString::myiterator& RString::myiterator::operator++()
{
    ++*((basecls*)this);
    return *this;
}

RString::myiterator RString::myiterator::operator++(int)
{
    myiterator tmp = *this;
    ++*this;
    return tmp;
}

RString::myiterator& RString::myiterator::operator--()
{
    --*((basecls*)this);
    return *this;
}

RString::myiterator RString::myiterator::operator--(int)
{
    myiterator tmp = *this;
    --*this;
    return tmp;
}

RString::myiterator& RString::myiterator::operator+=(difference_type off)
{
    *(basecls*)this += off;
    return *this;
}

RString::myiterator RString::myiterator::operator+(difference_type off) const
{
    myiterator tmp = *this;
    return tmp += off;
}

RString::myiterator& RString::myiterator::operator-=(difference_type off)
{
    return *this += -off;
}

RString::myiterator RString::myiterator::operator-(difference_type off) const
{
    myiterator tmp = *this;
    return tmp -= off;
}

RString::myiterator::difference_type RString::myiterator::operator-(const myiterator& rhs) const
{
    return *(basecls*)this - rhs;
}

RString::myiterator::reference RString::myiterator::operator[](difference_type off) const
{
    return *(*this + off);
}

void RString::resize(size_type newsize, char ch)
{
    // determine new length, padding with ch elements as needed
    if (newsize <= size())
        truncate(newsize);
    else
        append(newsize - size(), ch);
}

void RString::resize(size_type newsize)
{
    // determine new length, padding with null elements as needed
    resize(newsize, char());
}

void RString::reserve(size_type newcap /*= 0*/)
{
    // determine new minimum length of allocated storage
    if (size() <= newcap && capacity() != newcap){
        // change reservation
        size_type s = size();
        if (grow(newcap, true))
            truncate(s);
    }
}

bool RString::empty() const
{
    // test if sequence is empty
    return size() == 0;
}

bool RString::null() const
{
    return capacity() == 0 && size() == 0;
}

const char* RString::cstr() const
{
    return dataptr();
}

RString::iterator RString::begin()
{
    // return iterator for beginning of mutable sequence        
    return iterator(allocator_.address(*val_.dataptr()));
}

RString::const_iterator RString::begin() const
{
    // return iterator for beginning of nonmutable sequence
    return const_iterator(allocator_.address(*val_.dataptr()));
}

RString::iterator RString::end() // return iterator for end of mutable sequence
{
    return iterator(allocator_.address(*val_.dataptr()) + val_.cur_size);
}

RString::const_iterator RString::end() const
{
    return const_iterator(allocator_.address(*val_.dataptr()) + val_.cur_size);
}

RString::reverse_iterator RString::rbegin() // return iterator for beginning of reversed mutable sequence
{
    return reverse_iterator(end());
}

RString::const_reverse_iterator RString::rbegin() const // return iterator for beginning of reversed nonmutable sequence
{
    return const_reverse_iterator(end());
}

RString::reverse_iterator RString::rend() // return iterator for end of reversed mutable sequence
{
    return reverse_iterator(begin());
}

RString::const_reverse_iterator RString::rend() const // return iterator for end of reversed nonmutable sequence
{
    return const_reverse_iterator(begin());
}

RString::const_iterator RString::cbegin() const // return iterator for beginning of nonmutable sequence
{
    return begin();
}

RString::const_iterator RString::cend() const // return iterator for end of nonmutable sequence
{
    return end();
}

RString::const_reverse_iterator RString::crbegin() const // return iterator for beginning of reversed nonmutable sequence
{
    return rbegin();
}

RString::const_reverse_iterator RString::crend() const // return iterator for end of reversed nonmutable sequence
{
    return rend();
}

// void RString::shrinkToFit()
// {
//     if ((size() | RStringVal::ALLOC_MASK) < capacity()) {	// worth shrinking, do it
//         RString tmp(*this);
//         swap(tmp);
//     }
// }

RString::reference RString::at(size_type off)
{
    if (off >= size())
        throw std::invalid_argument("invalid off position!");
    return dataptr()[off];
}

RString::const_reference RString::at(size_type off) const
{
    if (off >= size())
        throw std::invalid_argument("invalid off position!");
    return dataptr()[off];
}

RString::size_type RString::copys(char* dest, size_type dsize, size_type cnt, size_type off /*= 0*/) const
{
    assert_cond_pointer(cnt != 0, dest);
    checkOffset(off);
    cnt = getSuffixSize(off, cnt);
    traits_type::_Copy_s(dest, dsize, dataptr() + off, cnt);
    return cnt;
}

RString::size_type RString::find(const RString& rhs, size_type off /*= 0*/) const
{
    return find(rhs.dataptr(), off, rhs.size());
}

RString::size_type RString::find(const char* strptr, size_type off, size_type cnt) const
{
    assert_cond_pointer(cnt != 0, strptr);
    if (cnt == 0 && off <= size())
        return off;	// null string always matches (if inside string)
    size_type num = 0;
    if (off < size() && cnt <= (num = size() - off)) {
        // room for match, look for it
        const char* maybe_ptr = nullptr;
        const char* start_ptr = dataptr() + off;
        num -= cnt - 1;
        for (; (maybe_ptr = traits_type::find(start_ptr, num, *strptr)) != nullptr;) {
            if (traits_type::compare(maybe_ptr, strptr, cnt) == 0)
                return maybe_ptr - dataptr();	// found a match
            num -= maybe_ptr - start_ptr + 1;
            start_ptr = maybe_ptr + 1;
        }
    }
    return npos;	// no match
}

RString::size_type RString::find(const char* strptr, size_type off /*= 0*/) const
{
    assert_pointer(strptr);
    return find(strptr, off, traits_type::length(strptr));
}

RString::size_type RString::find(char ch, size_type off /*= 0*/) const
{
    return find((const char*)&ch, off, 1);
}

RString::size_type RString::rfind(const RString& rhs, size_type off /*= npos*/) const 
{
    return rfind(rhs.dataptr(), off, rhs.size());
}

RString::size_type RString::rfind(const char* strptr, size_type off, size_type cnt) const
{
    assert_cond_pointer(cnt != 0, strptr);
    if (cnt == 0)
        return off < size() ? off : size();	// null always matches
    if (cnt <= size()) {
        // room for match, look for it
        const char* uptr = dataptr() + (off < size() - cnt ? off : size() - cnt);
        for (; ; --uptr) {
            if (traits_type::eq(*uptr, *strptr) && traits_type::compare(uptr, strptr, cnt) == 0)
                return uptr - dataptr();	// found a match
            else if (uptr == dataptr())
                break;	// at beginning, no more chance for match
        }
    }
    return npos;	// no match
}

RString::size_type RString::rfind(const char *strptr, size_type off /*= npos*/) const
{
    assert_pointer(strptr);
    return rfind(strptr, off, traits_type::length(strptr));
}

RString::size_type RString::rfind(char ch, size_type off /*= npos*/) const
{
    return rfind((const char*)&ch, off, 1);
}

RString::size_type RString::findFirstOf(const RString& rhs, size_type off /*= 0*/) const
{
    return findFirstOf(rhs.dataptr(), off, rhs.size());
}

RString::size_type RString::findFirstOf(const char* strptr, size_type off, size_type cnt) const
{
    assert_cond_pointer(cnt != 0, strptr);
    if (cnt > 0 && size() > off) {
        // room for match, look for it
        const char *const maybe_ptr = dataptr() + size();
        for (const char* start_ptr = dataptr() + off;
            start_ptr < maybe_ptr; ++start_ptr)
            if (traits_type::find(strptr, cnt, *start_ptr) != 0)
                return start_ptr - dataptr();	// found a match
    }
    return npos;	// no match
}

RString::size_type RString::findFirstOf(const char *strptr, size_type off /*= 0*/) const
{
    assert_pointer(strptr);
    return findFirstOf(strptr, off, traits_type::length(strptr));
}

RString::size_type RString::findFirstOf(char ch, size_type off /*= 0*/) const
{
    return find((const char *)&ch, off, 1);
}

RString::size_type RString::findLastOf(const RString& rhs, size_type off /*= npos*/) const
{
    return findLastOf(rhs.dataptr(), off, rhs.size());
}

RString::size_type RString::findLastOf(const char *strptr, size_type off, size_type cnt) const
{    
    assert_cond_pointer(cnt != 0, strptr);
    if (cnt > 0 && size() > 0){
        // worth searching, do it
        const char *uptr = dataptr() + (off < size() ? off : size() - 1);
        for (; ; --uptr) {
            if (traits_type::find(strptr, cnt, *uptr) != 0)
                return uptr - dataptr();	// found a match
            else if (uptr == dataptr())
                break;	// at beginning, no more chance for match
        }
    }
    return npos;	// no match
}

RString::size_type RString::findLastOf(const char* strptr, size_type off /*= npos*/) const
{
    assert_pointer(strptr);
    return findLastOf(strptr, off, traits_type::length(strptr));
}

RString::size_type RString::findLastOf(char ch, size_type off /*= npos*/) const
{
    return rfind((const char *)&ch, off, 1);
}

RString::size_type RString::findFirstNotOf(const RString& rhs, size_type off /*= 0*/) const
{
    return findFirstNotOf(rhs.dataptr(), off, rhs.size());
}

RString::size_type RString::findFirstNotOf(const char *strptr, size_type off, size_type cnt) const
{
    assert_cond_pointer(cnt != 0, strptr);
    if(size() > off){
        // room for match, look for it
        const char *const vptr = dataptr() + size();
        for(const char* uptr = dataptr() + off; uptr < vptr; ++uptr){
            if (traits_type::find(strptr, cnt, *uptr) == 0)
                return uptr - dataptr();
        }
    }
    return npos;
}

RString::size_type RString::findFirstNotOf(const char *strptr, size_type off /*= 0*/) const
{
    assert_pointer(strptr);
    return findFirstNotOf(strptr, off, traits_type::length(strptr));
}

RString::size_type RString::findFirstNotOf(char ch, size_type off /*= 0*/) const
{
    return findFirstNotOf((const char *)&ch, off, 1);
}

RString::size_type RString::findLastNotOf(const RString& rhs, size_type off /*= npos*/) const
{
    return findLastNotOf(rhs.dataptr(), off, rhs.size());
}

RString::size_type RString::findLastNotOf(const char *strptr, size_type off, size_type cnt) const
{    
    assert_cond_pointer(cnt != 0, strptr);
    if(size() > 0){
        // worth searching, do it
        const char* uptr = dataptr() + (off < size() ? off : size() - 1);
        for(; ; --uptr){
            if (traits_type::find(strptr, cnt, *uptr) == 0)
                return uptr - dataptr();
            else if (uptr == dataptr())
                break;
        }
    }
    return npos;
}

RString::size_type RString::findLastNotOf(const char *strptr, size_type off /*= npos*/) const
{
    assert_pointer(strptr);
    return findLastNotOf(strptr, off, traits_type::length(strptr));
}

RString::size_type RString::findLastNotOf(char ch, size_type off /*= npos*/) const
{
    return findLastNotOf((const char *)&ch, off, 1);
}

RString::list_type RString::split(char splitchar) const
{
    list_type result_strings;
    size_type start = 0;
    size_type pos = 0;
    while(start < size()){
        pos = find(splitchar, start);
        if(pos == npos){
            result_strings.push_back(substr(start));
            break;
        }else if(start != pos)
            result_strings.push_back(substr(start, pos - start));
        start = pos + 1;
    }
    return result_strings;
}

RString& RString::assign(const char* strptr, size_type cnt)
{
    assert_cond_pointer(cnt != 0, strptr);
    if(inside(strptr))
        return assign(*this, strptr - dataptr(), cnt);	// substring
    if(grow(cnt)){
    	// make room and assign new stuff
        traits_type::copy(dataptr(), strptr, cnt);
        truncate(cnt);
    }
    return (*this);
}

RString& RString::assign(size_type cnt, char ch)
{	
    if(cnt == npos)
        std::logic_error("result too long!");
    if(grow(cnt)){
    	// make room and assign new stuff
        charAssign(0, cnt, ch);
        truncate(cnt);
    }
    return *this;
}

RString& RString::assign(const RString& rhs)
{	// assign rhs
    return assign(rhs, 0, npos);
}

RString& RString::assign(const RString& rhs, size_type roff, size_type cnt /*= npos*/)
{
    rhs.checkOffset(roff);
    cnt = rhs.getSuffixSize(roff, cnt);
    if(this == &rhs){
        erase((size_type)(roff + cnt));
        erase(0, roff);	// substring
    }else if(grow(cnt)){
        traits_type::copy(val_.dataptr(), rhs.val_.dataptr() + roff, cnt);
        truncate(cnt);
    }
    return *this;
}

RString& RString::assign(const_iterator first, const_iterator last)
{    
    return replace(begin(), end(), first, last);
}

RString& RString::assign(const char* strptr)
{
    assert_pointer(strptr);
    return assign(strptr, traits_type::length(strptr));
}

RString& RString::assign(const_pointer first, const_pointer last)
{
    return replace(begin(), end(), first, last);
}

RString RString::substr(size_type off /*= 0*/, size_type cnt /*= npos*/) const
{
    return RString(*this, off, cnt, allocator_);
}

int32_t RString::compare(const char* strptr) const
{
    assert_pointer(strptr);
    return traitsCompare(dataptr(), size(), strptr, traits_type::length(strptr));
}

int32_t RString::compare(size_type off, size_type cnt, const char* strptr) const
{
    assert_pointer(strptr);
    checkOffset(off);
    return traitsCompare(dataptr() + off, getSuffixSize(off, cnt), strptr, traits_type::length(strptr));
}

int32_t RString::compare(size_type off, size_type cnt, const char* lhs, size_type rcnt) const
{
    assert_cond_pointer(cnt != 0, lhs);
    checkOffset(off);
    return traitsCompare(dataptr() + off, getSuffixSize(off, cnt), lhs, rcnt);
}

int32_t RString::compare(const RString& rhs) const
{
    return traitsCompare(dataptr(), size(), rhs.dataptr(), rhs.size());
}

int32_t RString::compare(size_type off, size_type cnt, const RString& rhs) const
{
    checkOffset(off);
    return traitsCompare(dataptr() + off, getSuffixSize(off, cnt), rhs.dataptr(), rhs.size());
}

int32_t RString::compare(size_type off, size_type cnt, const RString& rhs, size_type roff, size_type rcnt /*= npos*/) const
{
    checkOffset(off);
    rhs.checkOffset(roff);
    return traitsCompare(dataptr() + off, getSuffixSize(off, cnt), rhs.dataptr() + roff, rhs.getSuffixSize(roff, rcnt));
}

RString& RString::insert(size_type off, const RString& rhs)
{
    return insert(off, rhs, 0, npos);
}

RString& RString::insert(size_type off, const RString& rhs, size_type roff, size_type rcnt /*= npos*/)
{
    checkOffset(off);
    rhs.checkOffset(roff);
    rcnt = rhs.getSuffixSize(roff, rcnt);
    if(npos - size() <= rcnt)
        throw std::logic_error("rcnt value is too big!");	// result too long
    const size_type kNewNum = size() + rcnt;
    if(rcnt > 0 && grow(kNewNum)){
        // make room and insert new stuff
        traits_type::move(dataptr() + off + rcnt, dataptr() + off, size() - off);	// empty out hole
        if (this == &rhs)
            traits_type::move(dataptr() + off, dataptr() + (off < roff ? roff + rcnt : roff), rcnt);	// substring
        else
            traits_type::copy(dataptr() + off, rhs.dataptr() + roff, rcnt);	// fill hole
        truncate(kNewNum);
    }
    return (*this);
}

RString& RString::insert(size_type off, const char* strptr, size_type cnt)
{
    assert_cond_pointer(cnt != 0, strptr);
    if (inside(strptr))
        return insert(off, *this, strptr - dataptr(), cnt);	// substring
    checkOffset(off);
    if (npos - size() <= cnt)
        throw std::logic_error("cnt value is too big!");	// result too long
    const size_type kNewNum = size() + cnt;
    if(cnt > 0 && grow(kNewNum)){
        // make room and insert new stuff
        traits_type::move(dataptr() + off + cnt, dataptr() + off, size() - off);	// empty out hole
        traits_type::copy(dataptr() + off, strptr, cnt);	// fill hole
        truncate(kNewNum);
    }
    return (*this);
}

RString& RString::insert(size_type off, const char* strptr)
{
    assert_pointer(strptr);
    return insert(off, strptr, traits_type::length(strptr));
}

RString& RString::insert(size_type off, size_type cnt, char ch)
{
    checkOffset(off);
    if(npos - size() <= cnt)
        throw std::invalid_argument("cnt is too big!");	// result too long
    const size_type kNewSize = size() + cnt;
    if(cnt > 0 && grow(kNewSize)){
        // make room and insert new stuff
        traits_type::move(dataptr() + off + cnt, dataptr() + off, size() - off);	// empty out hole
        charAssign(off, cnt, ch);	// fill hole
        truncate(kNewSize);
    }
    return *this;
}

RString::iterator RString::insert(const_iterator where)
{
    return insert(where, char());
}

RString::iterator RString::insert(const_iterator where, char ch)
{
    size_type _Off = where - begin();
    insert(_Off, 1, ch);
    return (begin() + _Off);
}

RString::iterator RString::insert(const_iterator where, size_type cnt, char ch)
{
    size_type off = where - begin();
    insert(off, cnt, ch);
    return (begin() + off);
}

RString::iterator RString::insert(const_iterator where, iterator first, iterator last)
{
    size_type off = where - begin();
    replace(where, where, first, last);
    return begin() + off;
}

RString::iterator RString::insert(const_iterator where, const_pointer first, const_pointer last)
{
    size_type off = where - begin();
    replace(where, where, first, last);
    return begin() + off;
}

RString::iterator RString::insert(const_iterator where, const_iterator first, const_iterator last)
{
    size_type off = where - begin();
    replace(where, where, first, last);
    return begin() + off;
}

RString& RString::append(const char* strptr, size_type cnt)
{
    assert_cond_pointer(cnt != 0, strptr);
    if (inside(strptr))
        return append(*this, strptr - dataptr(), cnt);	// substring
    if (npos - size() <= cnt)
        throw std::invalid_argument("cnt value is too big!");	// result too long
    const size_type kNewNum = size() + cnt;
    if(cnt > 0 && grow(kNewNum)){	
        // make room and append new stuff
        traits_type::copy(dataptr() + size(), strptr, cnt);
        truncate(kNewNum);
    }
    return (*this);
}

RString& RString::append(const RString& rhs, size_type roff, size_type cnt /*= npos*/)
{    
    rhs.checkOffset(roff);
    cnt = rhs.getSuffixSize(roff, cnt);
    if (npos - size() <= cnt)
        throw std::invalid_argument("cnt value is too big!");	// result too long
    const size_type kNewNum = size() + cnt;
    if (cnt > 0 && grow(kNewNum)){
        // make room and append new stuff
        traits_type::copy(dataptr() + size(), rhs.dataptr() + roff, cnt);
        truncate(kNewNum);
    }
    return (*this);
}

RString& RString::append(const RString& rhs)
{
    return append(rhs, 0, npos);
}

RString& RString::append(const char* strptr)
{
    if (!strptr)
        throw std::invalid_argument("invalid strptr!");
    return append(strptr, traits_type::length(strptr));
}

RString& RString::append(size_type cnt, char ch)
{
    if (npos - size() <= cnt)
        throw std::invalid_argument("cnt value is too big!");	// result too long
    const size_type kNewNum = size() + cnt;
    if (cnt > 0 && grow(kNewNum)){
        // make room and append new stuff using assign
        charAssign(size(), cnt, ch);
        truncate(kNewNum);
    }
    return (*this);
}

RString& RString::append(const_iterator first, const_iterator last)
{
    return replace(end(), end(), first, last);
}

RString& RString::append(std::initializer_list<char> chars)
{
    return append(chars.begin(), chars.end());
}

RString& RString::append(const_pointer first, const_pointer last)
{
    return (replace(end(), end(), first, last));
}

RString& RString::erase(size_type off /*= 0*/)
{
    checkOffset(off);
    truncate(off);
    return *this;
}

RString& RString::erase(size_type off, size_type cnt)
{
    checkOffset(off);
    if (val_.cur_size - off <= cnt)
        truncate(off);	// erase elements [off, ...)
    else if (cnt > 0){	
        // move elements down
        pointer ptr = val_.dataptr() + off;
        size_type newsize = val_.cur_size - cnt;
        traits_type::move(ptr, ptr + cnt, newsize - off);
        truncate(newsize);
    }
    return *this;
}

RString::iterator RString::erase(const_iterator w)
{
    size_type cnt = w - begin();
    erase(cnt, 1);
    return begin() + cnt;
}

RString::iterator RString::erase(const_iterator first, const_iterator last)
{
    size_type cnt = first - begin();
    erase(cnt, last - first);
    return begin() + cnt;
}

RString& RString::popBack()
{
    if(!empty())
        return erase(size() - 1);
    return *this;
}

RString& RString::popFront()
{
    if(!empty()){
        char* s = dataptr();
        size_t newsize = size() - 1;
        traits_type::move(s, s + 1, newsize);
        truncate(newsize);
    }
    return *this;
}

template<class ptr_type>
inline auto AutoPointer(ptr_type p) // converts from a fancy pointer to a plain pointer
{
    return std::addressof(*p);
}

template<class ptr_type>
inline ptr_type* AutoPointer(ptr_type* ptr) // do nothing for plain pointers
{
    return (ptr);
}

char* RString::RStringVal::dataptr() // determine current pointer to buffer for mutable string
{
    return BUFFER_SIZE <= reserve_size ? AutoPointer(data.ptr) : data.buffer;
}

const char* RString::RStringVal::dataptr() const // determine current pointer to buffer for nonmutable string
{
    return BUFFER_SIZE <= reserve_size ? AutoPointer(data.ptr) : data.buffer;
}

RString::RStringVal::RStringVal()
    :cur_size(0),
    reserve_size(0)
{
    data.ptr = nullptr;
    traits_type::assign(data.buffer, BUFFER_SIZE, 0);
}

RString& RString::replace(size_type lhs_off, size_type lhs_cnt, const RString& rhs, size_type rhs_off, size_type rhs_cnt /*= npos*/)
{
    checkOffset(lhs_off);
    rhs.checkOffset(rhs_off);
    lhs_cnt = getSuffixSize(lhs_off, lhs_cnt);
    rhs_cnt = rhs.getSuffixSize(rhs_off, rhs_cnt);
    if (val_.cur_size - lhs_cnt + rhs_cnt > npos)
        throw std::logic_error("result too long");	// result too long

    const size_type kNewSize = val_.cur_size + rhs_cnt - lhs_cnt; //may be bigger or smaller
    if (val_.cur_size < kNewSize)
        grow(kNewSize);

    const size_type kChNum = val_.cur_size - (lhs_off + lhs_cnt);	// length of kept tail
    const size_type kLhsEndpos = lhs_off + lhs_cnt;
    const size_type kRhsEndpos = rhs_off + rhs_cnt;
    if (rhs_cnt == lhs_cnt)
        // only one movement required, so _Traits::move handles any overlap
        traits_type::move(val_.dataptr() + lhs_off, rhs.val_.dataptr() + rhs_off, rhs_cnt);	// fill hole
    else if (this != &rhs) {
        // no overlap, just move down and copy in new stuff
        traits_type::move(val_.dataptr() + lhs_off + rhs_cnt, val_.dataptr() + kLhsEndpos, kChNum);	// empty hole
        traits_type::copy(val_.dataptr() + lhs_off, rhs.val_.dataptr() + rhs_off, rhs_cnt);	// fill hole
    }else if (rhs_cnt < lhs_cnt) {
        // hole doesn't get larger, just copy in substring
        traits_type::move(val_.dataptr() + lhs_off, val_.dataptr() + rhs_off, rhs_cnt);	// fill hole
        traits_type::move(val_.dataptr() + lhs_off + rhs_cnt, val_.dataptr() + kLhsEndpos, kChNum);	// move tail down
    }else if (rhs_off <= lhs_off) {
        // hole gets larger, substring begins before hole
        traits_type::move(val_.dataptr() + lhs_off + rhs_cnt, val_.dataptr() + kLhsEndpos, kChNum);	// move tail down
        traits_type::move(val_.dataptr() + lhs_off, val_.dataptr() + rhs_off, rhs_cnt);	// fill hole
    }else if (lhs_off + lhs_cnt <= rhs_off) {
        // hole gets larger, substring begins after hole
        traits_type::move(val_.dataptr() + lhs_off + rhs_cnt, val_.dataptr() + kLhsEndpos, kChNum);	// move tail down
        traits_type::move(val_.dataptr() + lhs_off, val_.dataptr() + (kRhsEndpos - lhs_cnt), rhs_cnt);	// fill hole
    }else {
        // hole gets larger, substring begins in hole
        traits_type::move(val_.dataptr() + lhs_off, val_.dataptr() + rhs_off, lhs_cnt);	// fill old hole
        traits_type::move(val_.dataptr() + lhs_off + rhs_cnt, val_.dataptr() + kLhsEndpos, kChNum);	// move tail down
        traits_type::move(val_.dataptr() + lhs_off + lhs_cnt, val_.dataptr() + kRhsEndpos, rhs_cnt - lhs_cnt);	// fill rest of new hole
    }
    truncate(kNewSize);
    return *this;
}

RString& RString::replace(const_iterator first, const_iterator last, iterator first2, iterator last2)
{
    if (first2 == last2)
        erase(first - begin(), last - first);
    else
        replace(first - begin(), last - first, &*first2, last2 - first2);
    return *this;
}

RString RString::FormatString(const char * format, ...)
{
    va_list vl;
    va_start(vl, format);
    char buffer[MAX_STR_BUFFER_SIZE] = { '\0' };
    vsprintf_s(buffer, MAX_STR_BUFFER_SIZE - 1, format, vl);
    va_end(vl);
    return buffer;
}

std::string RString::decodeToLocale() const
{
    //convert utf8 to wstring
    std::wstring tmpwstr = decodeToWString();
    //convert wstring to locale string    
    std::locale l(""); //get locale from environment, same as: setlocale(LC_ALL, "").
    std::wstring_convert<LocaleCodecvt> cvt(new LocaleCodecvt(l.name()));
    return cvt.to_bytes(tmpwstr);
}

std::wstring RString::decodeToWString() const
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    return cvt.from_bytes(cstr());
}

RString RString::FromLocale(const char* localestr)
{
	RString rs;
	return rs.encodeFromLocale(localestr);
}

RString& RString::encodeFromLocale(const char* localestr)
{
    std::locale l(""); //get locale from environment, same as: setlocale(LC_ALL, "").
    std::wstring_convert<LocaleCodecvt> cv(new LocaleCodecvt(l.name()));
    std::wstring tmpwstr = cv.from_bytes(localestr);
    return encodeFromWString(tmpwstr);
}

RString RString::FromWString(const std::wstring& srcstr)
{
	RString rs;
	return rs.encodeFromWString(srcstr);
}

RString& RString::encodeFromWString(const std::wstring& srcstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    std::string utf8str = cvt.to_bytes(srcstr);
    return assign(utf8str.c_str());
}

RString RString::NewUID()
{
#ifdef WIN32
	GUID guid;
	::CoCreateGuid(&guid);
	return FormatString("%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
						guid.Data1, guid.Data2, guid.Data3,
						guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
						guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
#else
	uuid_t uuid;
	uuid_generate_random(uuid);
	char s[37];
	uuid_unparse(uuid, s);
	return s;
#endif	
}

RString& RString::format(const char * format, ...)
{
    va_list vl;
    va_start(vl, format);
    char buffer[MAX_STR_BUFFER_SIZE] = {'\0'};
    vsprintf_s(buffer, MAX_STR_BUFFER_SIZE - 1, format, vl);
    va_end(vl);
    return assign(buffer);
}

RString& RString::upper()    
{
    for(std::size_t i = 0; i < size(); ++i){
        reference c = at(i);
        if(c >= 'a' && c <= 'z')
            c = 'A' + (c - 'a');
    }
    return *this;
}

RString RString::upper() const
{
    RString cpy(*this);
    cpy.upper();
    return cpy;
}

RString& RString::lower()    
{
    for(std::size_t i = 0; i < size(); ++i){
        reference c = at(i);
        if(c >= 'A' && c <= 'Z')
            c = 'a' + (c - 'A');
    }
    return *this;
}

RString RString::lower() const
{
    RString cpy(*this);
    cpy.lower();
    return cpy;
}

int16_t RString::toInt16() const
{
	std::stringstream ss;
	ss << cstr();
	int16_t val = 0;
	ss >> val;
	return val;
}

RString& RString::fromInt16(int16_t num)
{
	std::stringstream ss;
	ss << num;
	return assign(ss.str().c_str());
}

uint16_t RString::toUInt16() const
{
	std::stringstream ss;
	ss << cstr();
	uint16_t val = 0;
	ss >> val;
	return val;
}

RString& RString::fromUInt16(uint16_t num)
{
	std::stringstream ss;
	ss << num;
	return assign(ss.str().c_str());
}

int32_t RString::toInt32() const
{
	std::stringstream ss;
	ss << cstr();
	int32_t val = 0;
	ss >> val;
	return val;
}

RString& RString::fromInt32(int32_t num)
{
	std::stringstream ss;
	ss << num;
	return assign(ss.str().c_str());
}

uint32_t RString::toUInt32() const
{
	std::stringstream ss;
	ss << cstr();
	uint32_t val = 0;
	ss >> val;
	return val;
}

RString& RString::fromUInt32(uint32_t num)
{
	std::stringstream ss;
	ss << num;
	return assign(ss.str().c_str());
}

int64_t RString::toInt64() const
{
	std::stringstream ss;
	ss << cstr();
	int64_t val = 0;
	ss >> val;
	return val;
}

RString& RString::fromInt64(int64_t num)
{
	std::stringstream ss;
	ss << num;
	return assign(ss.str().c_str());
}

uint64_t RString::toUInt64() const
{
	std::stringstream ss;
	ss << cstr();
	uint64_t val = 0;
	ss >> val;
	return val;
}

RString& RString::fromUInt64(uint64_t num)
{
	std::stringstream ss;
	ss << num;
	return assign(ss.str().c_str());
}

double RString::toDouble() const
{
	std::istringstream ss(cstr());
	double val = 0;
	ss >> val;
	return val;
}

RString& RString::fromDouble(double num, int8_t p /*= 0*/, ios_base::fmtflags fs /*= 0*/)
{
	std::ostringstream ss;
	if(p > 0)
		ss.precision(p);
	if(fs > 0)
		ss.setf(fs);
	ss << num;	
	return assign(ss.str().c_str());
}

RString& RString::replace(const_iterator first, const_iterator last, const char* strptr, size_type cnt)
{
    return replace(first - begin(), last - first, strptr, cnt);
}

RString& RString::replace(const_iterator first, const_iterator last, const_iterator first2, const_iterator last2)
{
    if(first2 == last2)
        erase(first - begin(), last - first);
    else
        replace(first - begin(), last - first, &*first2, last2 - first2);
    return *this;
}

RString& RString::replace(const_iterator first, const_iterator last, const_pointer first2, const_pointer last2)
{
    if(first2 == last2)
        erase(first - begin(), last - first);
    else
        replace(first - begin(), last - first, &*first2, last2 - first2);
    return *this;
}

RString& RString::replace(size_type off, size_type cnt, const RString& rhs)
{
    return replace(off, cnt, rhs, 0, npos);
}

RString& RString::replace(size_type off, size_type cnt, const char* strptr, size_type rcnt)
{
    assert_cond_pointer(rcnt != 0, strptr);
    if(inside(strptr))
        return replace(off, cnt, *this, strptr - dataptr(), rcnt);	// substring, replace carefully
    checkOffset(off);
    cnt = getSuffixSize(off, cnt);
    if(npos - rcnt <= size() - cnt)
        throw std::logic_error("cnt value is too big!");	// result too long
    size_type leftover_num = size() - cnt - off;
    if(rcnt < cnt)
        traits_type::move(dataptr() + off + rcnt, dataptr() + off + cnt, leftover_num);	// smaller hole, move tail up
    const size_type kNewNum = size() + rcnt - cnt;
    if((0 < rcnt || 0 < cnt) && grow(kNewNum)) {
        // make room and rearrange
        if (cnt < rcnt)
            traits_type::move(dataptr() + off + rcnt, dataptr() + off + cnt, leftover_num);	// move tail down
        traits_type::copy(dataptr() + off, strptr, rcnt);	// fill hole
        truncate(kNewNum);
    }
    return *this;
}

RString& RString::replace(size_type off, size_type cnt, const char* strptr)
{
    assert_pointer(strptr);
    return replace(off, cnt, strptr, traits_type::length(strptr));
}

RString& RString::replace(size_type off, size_type cnt, size_type chcnt, char ch)
{
    checkOffset(off);
    cnt = getSuffixSize(off, cnt);
    if (npos - chcnt <= size() - cnt)
        throw std::logic_error("result too long!");	// result too long
    size_type leftover_num = size() - cnt - off;
    if (chcnt < cnt)
        traits_type::move(dataptr() + off + chcnt, dataptr() + off + cnt, leftover_num);	// smaller hole, move tail up
    const size_type kNewNum = size() + chcnt - cnt;
    if((0 < chcnt || 0 < cnt) && grow(kNewNum)) {
        // make room and rearrange
        if (cnt < chcnt)
            traits_type::move(dataptr() + off + chcnt, dataptr() + off + cnt, leftover_num);	// move tail down
        charAssign(off, chcnt, ch);	// fill hole
        truncate(kNewNum);
    }
    return *this;
}

RString& RString::replace(const_iterator first, const_iterator last, const RString& rhs)
{
    return replace(first - begin(), last - first, rhs);
}

RString& RString::replace(const_iterator first, const_iterator last, const char* strptr)
{
    return replace(first - begin(), last - first, strptr);
}

RString& RString::replace(const_iterator first, const_iterator last, size_type cnt, char ch)
{
    return replace(first - begin(), last - first, cnt, ch);
}

RString& RString::replace(const_iterator first, const_iterator last, pointer first2, pointer last2)
{
    if (first2 == last2)
        erase(first - begin(), last - first);
    else
        replace(first - begin(), last - first, &*first2, last2 - first2);
    return (*this);
}

RString& RString::operator=(std::initializer_list<char> ilist)
{
    return assign(ilist.begin(), ilist.end());
}

RString& RString::operator=(char ch)
{
    return assign(1, ch);
}

RString& RString::operator=(const char* strptr)
{
    return assign(strptr);
}

RString& RString::operator=(const RString& rhs)
{    
    if (this != &rhs)
        assign(rhs);
    return *this;
}

RString& RString::operator+=(const RString& rhs)
{
    return append(rhs);
}

RString& RString::operator+=(const char* strptr)
{    
    return append(strptr);
}

RString& RString::operator+=(char ch)
{    
    return append((size_type)1, ch);
}

RString& RString::operator+=(std::initializer_list<char> ilist)
{
    return append(ilist.begin(), ilist.end());
}

RString::reference RString::operator[](size_type off)
{
//     if (off >= size())
//         throw std::logic_error("string subscript out of range");
    return dataptr()[off];
}

RString::const_reference RString::operator[](size_type off)const
{
//     if (off >= size())	// sic
//         throw std::logic_error("string subscript out of range");
    return (dataptr()[off]);
}

RString::RString(const RString& rhs)
    :allocator_(rhs.allocator_)
{
    tidy();
    assign(rhs, 0, npos);
}

RString::RString()
{
    tidy();
}

RString::RString(const allocator_type& al)
    :allocator_(al)
{
    tidy();
}

RString::RString(const RString& rhs, size_type roff, size_type cnt /*= npos*/)
    :allocator_(rhs.allocator_)
{
    tidy();
    assign(rhs, roff, cnt);
}

RString::RString(const RString& rhs, size_type roff, size_type cnt, const allocator_type& al)
    :allocator_(al)
{    
    tidy();
    assign(rhs, roff, cnt);
}

RString::RString(const char* strptr, size_type cnt)
{
    tidy();
    assign(strptr, cnt);
}

RString::RString(const char* strptr, size_type cnt, const allocator_type& al)
    :allocator_(al)
{
    tidy();
    assign(strptr, cnt);
}

RString::RString(const char* strptr)
{
    tidy();
    assign(strptr);
}

RString::RString(const char* strptr, const allocator_type& al)
    :allocator_(al)
{
    tidy();
    assign(strptr);
}

RString::RString(size_type cnt, char ch)
{
    tidy();
    assign(cnt, ch);
}

RString::RString(size_type cnt, char ch, const allocator_type& al) 
    :allocator_(al)
{
    tidy();
    assign(cnt, ch);
}

RString::RString(RString&& rhs) 
    :allocator_(std::move(rhs.allocator_))
{
    tidy();
    assignMove(std::forward<RString>(rhs));
}

RString::RString(RString&& rhs, const allocator_type& al)
    :allocator_(al)
{
    if (allocator_ != rhs.allocator_)
        assign(rhs.begin(), rhs.end());
    else
        assignMove(std::forward<RString>(rhs));
}

RString::RString(const std::wstring& wstr)
{
    encodeFromWString(wstr);
}

RString::~RString()
{
    tidy(true);
}

bool RString::grow(size_type newsize, bool trim /*= false*/)
{
    if (newsize >= maxsize())
        throw std::logic_error("result too long");	// result too long
    if (newsize >= val_.reserve_size)
        copy(newsize, val_.cur_size);	// reallocate to grow
    else if (trim && newsize < RStringVal::BUFFER_SIZE)
        // copy and deallocate if trimming to small string
        tidy(true, newsize < val_.cur_size ? newsize : val_.cur_size);
    else if (newsize == 0)
        truncate(0);	// new size is zero, just null terminate
    return newsize > 0;	// return true only if more work to do
}

void RString::copy(size_type newsize, size_type oldsize)
{
    size_type cur_newres = newsize | RStringVal::ALLOC_MASK;
    if (cur_newres >= maxsize())
        cur_newres = newsize;	// undo roundup if too big
    else if (val_.reserve_size / 2 <= cur_newres / 3)
        ;
    else if (val_.reserve_size + val_.reserve_size / 2 <= maxsize())
        cur_newres = val_.reserve_size + val_.reserve_size / 2;	// grow exponentially if possible
    else
        cur_newres = maxsize();	// settle for max_size()

    pointer new_allocated_ptr;
    try {
        new_allocated_ptr = allocator_.allocate(cur_newres + 1);
    }catch (...) {
        cur_newres = newsize;	// allocation failed, undo roundup and retry
        try {
            new_allocated_ptr = allocator_.allocate(cur_newres + 1);
        }catch (...) {
            tidy(true);	// failed again, discard storage and reraise
            throw;
        };
    };
    if (oldsize > 0)
        traits_type::copy(AutoPointer(new_allocated_ptr), val_.dataptr(), oldsize);	// copy existing elements
    tidy(true);
    allocator_.construct(std::addressof(val_.data.ptr), new_allocated_ptr);
    val_.reserve_size = cur_newres;
    truncate(oldsize);
}

void RString::checkOffset(const size_type off) const
{
    if(val_.cur_size < off)
        throw std::invalid_argument("subscript out of range");
}

RString::size_type RString::getSuffixSize(const size_type off, const size_type source_size) const
{
    const size_type max_effective_size = val_.cur_size - off;
    return (source_size <= max_effective_size) ? source_size : max_effective_size;
}

void RString::charAssign(size_type off, size_type cnt, char ch)
{
    if(cnt == 1)
        traits_type::assign(*(val_.dataptr() + off), ch);
    else
        traits_type::assign(val_.dataptr() + off, cnt, ch);
}

void RString::tidy(bool built /*= false*/, size_type newsize /*= 0*/)
{
    if(!built)
        ;
    else if (val_.reserve_size > RStringVal::BUFFER_SIZE) {
        // copy any leftovers to small buffer and deallocate
        char* ptr = val_.data.ptr;
        allocator_.destroy(std::addressof(ptr));
        if (newsize > 0)
            traits_type::copy(val_.data.buffer, ptr, newsize);
        allocator_.deallocate(ptr, val_.reserve_size + 1);
    }
    val_.reserve_size = RStringVal::BUFFER_SIZE - 1;
    truncate(newsize);
}

void RString::truncate(size_type newsize)
{
    val_.cur_size = newsize;
    val_.dataptr()[val_.cur_size] = char();
}

void RString::assignMove(RString&& rhs)
{
    // assign by moving rhs
    if (rhs.val_.reserve_size < RStringVal::BUFFER_SIZE)
        traits_type::move(val_.data.buffer, rhs.val_.data.buffer, rhs.val_.cur_size + 1);
    else{	// copy pointer
        allocator_.construct(std::addressof(val_.data.ptr), rhs.val_.data.ptr);
        rhs.val_.data.ptr = pointer();
    }
    val_.cur_size = rhs.val_.cur_size;
    val_.reserve_size = rhs.val_.reserve_size;
    rhs.tidy();
}

int32_t RString::traitsCompare(const char* const lhs, const size_type lsize, const char* const rhs, const size_type rsize)const
{    
    const size_type kMinSize = lsize < rsize ? lsize : rsize;
    const int32_t res = traits_type::compare(lhs, rhs, kMinSize);
    if (res != 0)
        return res;
    if (lsize < rsize)
        return -1;
    if (lsize > rsize)
        return 1;
    return 0;
}

RATEL_NAMESPACE_END



