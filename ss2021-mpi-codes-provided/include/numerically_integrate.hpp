#ifndef numerically_integrate_hpp_
#define numerically_integrate_hpp_

//===========================================================================

#include <iterator>     // for std::distance
#include <numeric>      // for std::transform_reduce
#include <functional>   // for std::plus
#include <type_traits>
#include <utility>      // for std::forward

#include "iota_iterator.hpp"

//===========================================================================

namespace sharcnet_summer_school_2021 {

//===========================================================================

template <typename Iterator>
constexpr auto positive_distance(Iterator const& a, Iterator const& b)
{
  using namespace std;
  using diff_type = 
    typename std::iterator_traits<Iterator>::difference_type
  ;
  using size_type = 
    std::conditional_t<
      std::is_fundamental_v<diff_type>,
      std::make_unsigned_t<diff_type>,
      diff_type
    >
  ;
  if constexpr(
    std::is_same_v<
      typename std::iterator_traits<Iterator>::iterator_category,
      std::random_access_iterator_tag
    >
  )
  {
    // It is possible for random-access iterators to return a
    // negative distance. If such is true, then the result
    // is size_type{}, i.e., 0.
    if (a <= b)
      return static_cast<size_type>(distance(a,b));
    else
      return size_type{};
  }
  else
  {
    // It is not possible to use ++ on a to reach b so the result
    // will always be positive --so coerce the result to be positive.
    return static_cast<size_type>(distance(a,b));
  }
}

//===========================================================================

template <
  typename NumDivisionsType,
  typename Domain,
  typename Func,
  typename Iterator
>
constexpr auto numerically_integrate(
  NumDivisionsType N,
  Domain const& a, 
  Domain const& b,
  Func&& f,
  Iterator const& first,
  Iterator const& last
)
{
  using sharcnet_summer_school_2021::iota_iterator;

  if (!N)
    N = 1;

  // PRECONDITIONS: a <= b && first <= last
  auto const delta_x = (b-a)/Domain(N);
  auto const half_delta_x = delta_x/Domain(2);
  auto const a_minus_half_delta_x = a - half_delta_x;

  //
  // Numerically integrate the function f from a to b using N subdivisions
  // but only evaluate from first to last with one optimization:
  //   * delta_x is a common term of the resulting sum so compute
  //     delta_x * postive_distance(first,last) outside of 
  //     transform_reduce().
  //
  return
    std::transform_reduce(
      first, last,
      decltype(f(a)){}, // i.e., summation identity is zero
      std::plus<>{}, // sum operation
      [&](auto const& interval_index) constexpr
      {
        auto const midpoint_value = 
          a_minus_half_delta_x + interval_index*delta_x
        ;
        return f(midpoint_value); // * delta_x factored out
      }
    ) * delta_x // delta_x multiplied back in here
  ;
}

//===========================================================================

template <
  typename ExecutionPolicy,
  typename NumDivisionsType,
  typename Domain,
  typename Func,
  typename Iterator
>
constexpr auto numerically_integrate(
  ExecutionPolicy&& ep,
  NumDivisionsType N,
  Domain const& a, 
  Domain const& b,
  Func&& f,
  Iterator const& first,
  Iterator const& last
)
{
  using sharcnet_summer_school_2021::iota_iterator;

  if (!N)
    N = 1;

  // PRECONDITIONS: a <= b && first <= last
  auto const delta_x = (b-a)/Domain(N);
  auto const half_delta_x = delta_x/Domain(2);
  auto const a_minus_half_delta_x = a - half_delta_x;

  //
  // Numerically integrate the function f from a to b using N subdivisions
  // but only evaluate from first to last with one optimization:
  //   * delta_x is a common term of the resulting sum so compute
  //     delta_x * postive_distance(first,last) outside of 
  //     transform_reduce().
  //
  return
    std::transform_reduce(
      std::forward<ExecutionPolicy>(ep),
      first, last,
      decltype(f(a)){}, // i.e., summation identity is zero
      std::plus<>{}, // sum operation
      [&](auto const& interval_index) constexpr
      {
        auto const midpoint_value = 
          a_minus_half_delta_x + interval_index*delta_x
        ;
        return f(midpoint_value); // * delta_x factored out
      }
    ) * delta_x // delta_x multiplied back in here
  ;
}

//===========================================================================

template <
  typename NumDivisionsType,
  typename Domain,
  typename Func
>
constexpr auto numerically_integrate(
  NumDivisionsType const& N,
  Domain const& a, 
  Domain const& b,
  Func&& f
)
{
  using sharcnet_summer_school_2021::iota_iterator;
  return numerically_integrate(
    N, a, b, std::forward<Func>(f),
    iota_iterator<NumDivisionsType>{},
    iota_iterator<NumDivisionsType>(N)
  );
}

//===========================================================================

template <
  typename ExecutionPolicy,
  typename NumDivisionsType,
  typename Domain,
  typename Func
>
constexpr auto numerically_integrate(
  ExecutionPolicy&& ep,
  NumDivisionsType const& N,
  Domain const& a, 
  Domain const& b,
  Func&& f
)
{
  using sharcnet_summer_school_2021::iota_iterator;
  return numerically_integrate(
    std::forward<ExecutionPolicy>(ep),
    N, a, b, std::forward<Func>(f),
    iota_iterator<NumDivisionsType>{},
    iota_iterator<NumDivisionsType>(N)
  );
}

//===========================================================================

} // namespace sharcnet_summer_school_2021

//===========================================================================

#endif // #ifndef numerically_integrate_hpp_ 
