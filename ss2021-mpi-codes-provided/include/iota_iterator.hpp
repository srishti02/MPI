#ifndef sharcnet_summer_school_2021_iota_iterator_hpp_
#define sharcnet_summer_school_2021_iota_iterator_hpp_

//===========================================================================
/*
  
  Modified BSD License (3-clause)

  Copyright 2021 (c) Paul Preney
  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions are met:
    1) Redistributions of source code must retain the above copyright 
       notice, this list of conditions and the following disclaimer.
    2) Redistributions in binary form must reproduce the above copyright 
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3) Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from 
       this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
//===========================================================================

#include <cstdlib>
#include <limits>
#include <iterator>
#include <type_traits>

//===========================================================================

namespace sharcnet_summer_school_2021 {

//===========================================================================

//
// iota_iterator<T> is an iterator class that stores a value of type T
// (which would typically be std::size_t or some integer value) used to
// iterate over a set of integer values without having to have a container
// holding values of type T, e.g.,
//
// for (
//   auto i=iota_iterator<unsigned>{}, iEnd=iota_iterator<unsigned>{100};
//   i != iEnd;
//   ++i)
// )
//  ;
//
// would iterate over the unsigned integer range [0,100).
//
// An equivalent of iota_iterator is in the TBB library called
// counting_iterator, see:
//    https://software.intel.com/content/www/us/en/develop/documentation/tbb-documentation/top/intel-threading-building-blocks-developer-reference/iterators/counting-iterator.html
//

//===========================================================================

template <typename T>
class iota_iterator
{
public:
  using value_type = T const;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::random_access_iterator_tag;

private:
  mutable T dist_;

public:
  constexpr iota_iterator() : 
    dist_{} 
  { 
  }

  constexpr iota_iterator(value_type const& v) : 
    dist_(v) 
  { 
  }

#if __cplusplus >= 201402L
  constexpr 
#endif
  iota_iterator(iota_iterator const&) = default;

#if __cplusplus >= 201402L
  constexpr 
#endif
  iota_iterator& operator =(iota_iterator const&) = default;

#if __cplusplus >= 201402L
  constexpr 
#endif
  iota_iterator(iota_iterator&) = default;

#if __cplusplus >= 201402L
  constexpr 
#endif
  iota_iterator& operator =(iota_iterator&) = default;

  constexpr bool operator ==(iota_iterator const& it) const 
    noexcept(noexcept( dist_ == it.dist_ ))
    { return dist_ == it.dist_; }
  constexpr bool operator !=(iota_iterator const& it) const 
    noexcept(noexcept( dist_ != it.dist_ ))
    { return dist_ != it.dist_; }
  
  constexpr bool operator <(iota_iterator const& it) const 
    noexcept(noexcept( dist_ < it.dist_ ))
    { return dist_ < it.dist_; }
  constexpr bool operator <=(iota_iterator const& it) const 
    noexcept(noexcept( dist_ <= it.dist_ ))
    { return dist_ <= it.dist_; }
  constexpr bool operator >=(iota_iterator const& it) const 
    noexcept(noexcept( dist_ >= it.dist_ ))
    { return dist_ >= it.dist_; }
  constexpr bool operator >(iota_iterator const& it) const 
    noexcept(noexcept( dist_ > it.dist_ ))
    { return dist_ > it.dist_; }

  constexpr reference operator *() const noexcept
    { return dist_; }
  constexpr pointer operator ->() const noexcept
    { return &dist_; }

#if __cplusplus >= 201402L
  constexpr 
#endif
  iota_iterator& operator ++() noexcept(noexcept( ++dist_ )) 
  { 
    ++dist_; 
    return *this; 
  }

#if __cplusplus >= 201402L
  constexpr 
#endif
  iota_iterator operator ++(int) 
    noexcept(
      std::is_nothrow_copy_constructible<iota_iterator>::value &&
      noexcept( ++dist_ )
    )
  { 
    iota_iterator tmp(*this); 
    this->operator ++(); 
    return tmp; 
  }

#if __cplusplus >= 201402L
  constexpr 
#endif
  iota_iterator& operator --() noexcept(noexcept( --dist_ ))
  { 
    --dist_; 
    return *this; 
  }

#if __cplusplus >= 201402L
  constexpr 
#endif
  iota_iterator operator --(int) 
    noexcept(
      std::is_nothrow_copy_constructible<iota_iterator>::value &&
      noexcept( --dist_ )
    )
  { 
    iota_iterator tmp(*this); 
    this->operator --(); 
    return tmp; 
  }

  constexpr difference_type operator -(iota_iterator const& it) const 
    noexcept(noexcept( dist_ - it.dist_ ))
  { 
    return dist_ - it.dist_; 
  }

#if __cplusplus >= 201402L
  constexpr 
#endif
  iota_iterator& operator +=(difference_type const& n) 
    noexcept(noexcept( dist_ += n ))
  {
    dist_ += n;
    return *this; 
  }

#if __cplusplus >= 201402L
  constexpr 
#endif
  iota_iterator& operator -=(difference_type const& n) 
    noexcept(noexcept( dist_ -= n ))
  { 
    dist_ -= n;
    return *this;
  }

#if __cplusplus >= 201402L
  constexpr 
#endif
  iota_iterator operator +(difference_type const& n) const
    noexcept(
      std::is_nothrow_constructible<
        iota_iterator, 
        difference_type const&
      >::value &&
      noexcept(std::declval<iota_iterator>() += n)
    )
  {
    iota_iterator retval(*this);
    retval += n;
    return retval;
  }
  
#if __cplusplus >= 201402L
  constexpr 
#endif
  iota_iterator operator -(difference_type const& n) const
    noexcept(
      std::is_nothrow_constructible<
        iota_iterator, 
        difference_type const&
      >::value &&
      noexcept(std::declval<iota_iterator>() -= n)
    )
  {
    iota_iterator retval(*this);
    retval -= n;
    return retval;
  }

  struct value_proxy final
  {
    value_type value_;

    constexpr value_type value() const noexcept
    {
      return value_;
    }

    constexpr reference ref() const noexcept
    {
      return value_;
    }

    constexpr operator reference() const noexcept
    {
      return value_;
    }
  };

  template <typename Index>
  constexpr auto operator [](Index i) const ->
    typename std::enable_if<
      std::numeric_limits<Index>::is_integer,
      value_proxy
    >::type
  {
    using std::numeric_limits;

    iota_iterator result(*this);

    for (; 
      i > numeric_limits<difference_type>::max(); 
      i -= numeric_limits<difference_type>::max()
    )
      result += numeric_limits<difference_type>::max();
    
    result += i;

    return { *result };
  }
};

template <typename T>
constexpr iota_iterator<T> operator +(
  typename iota_iterator<T>::difference_type const& n, 
  iota_iterator<T> const& i
) noexcept(
  std::is_nothrow_copy_constructible<
    iota_iterator<T>
  >::value &&
  noexcept(std::declval<iota_iterator<T>>() += n)
)
{
  iota_iterator<T> retval(i);
  retval += n;
  return retval;
}
  
template <typename T>
constexpr iota_iterator<T> operator -(
  typename iota_iterator<T>::difference_type const& n, 
  iota_iterator<T> const& i
) noexcept(
  std::is_nothrow_copy_constructible<
    iota_iterator<T>
  >::value &&
  noexcept(std::declval<iota_iterator<T>>() -= n)
)
{
  iota_iterator<T> retval(i);
  retval -= n;
  return retval;
}

//===========================================================================

} // namespace sharcnet_summer_school_2021

//===========================================================================

#endif // #ifndef sharcnet_summer_school_2021_iota_iterator_hpp_
