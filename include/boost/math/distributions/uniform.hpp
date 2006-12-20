//  Copyright John Maddock 2006.
//  Copyright Paul A. Bristow 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// TODO deal with infinity as special better - or remove.
// 

#ifndef BOOST_STATS_UNIFORM_HPP
#define BOOST_STATS_UNIFORM_HPP

// http://www.itl.nist.gov/div898/handbook/eda/section3/eda3668.htm
// http://mathworld.wolfram.com/UniformDistribution.html
// http://documents.wolfram.com/calculationcenter/v2/Functions/ListsMatrices/Statistics/UniformDistribution.html
// http://en.wikipedia.org/wiki/Uniform_distribution_%28continuous%29

#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/special_functions/expm1.hpp>
#include <boost/math/distributions/detail/common_error_handling.hpp>
#include <boost/math/distributions/complement.hpp>

namespace boost{ namespace math
{
  namespace detail
  {
    template <class RealType>
    bool check_uniform_lower(
      const char* function,
      RealType lower,
      RealType* result)
    {
      if((boost::math::isfinite)(lower))
      { // any finite value is OK.
        return true; 
      }
      else
      { // Not finite.
        *result = tools::domain_error<RealType>(
          function,
          "Lower parameter is %1%, but must be finite!", lower);
        return false;
      }
    } // bool check_uniform_lower(

    template <class RealType>
    bool check_uniform_upper(
      const char* function,
      RealType upper,
      RealType* result)
    {
      if((boost::math::isfinite)(upper))
      { // Any finite value is OK.
        return true;
      } 
      else
      { // Not finite.
        *result = tools::domain_error<RealType>(
          function,
          "Upper parameter is %1%, but must be finite!", upper);
        return false;
      }
    } // bool check_uniform_upper(

    template <class RealType>
    bool check_uniform_x(
      const char* function,
      RealType const& x,
      RealType* result)
    {
      if((boost::math::isfinite)(x))
      { // Any finite value is OK
        return true;
      }
      else
      { // Not finite..
        *result = tools::domain_error<RealType>(
          function,
          "x parameter is %1%, but must be finite!", x);
        return false;
      }
    } // bool check_uniform_x

    template <class RealType>
    inline bool check_uniform(
      const char* function,
      RealType lower,
      RealType upper,
      RealType* result)
    {
      if(check_uniform_lower(function, lower, result)
        && check_uniform_upper(function, upper, result)
        && (lower < upper)) // If lower == upper then 1 / (upper-lower) = 1/0 = +infinity!
      { 
        return true;
      }
      else
      { // upper and lower have been checked before, so must be lower >= upper.
        *result = tools::domain_error<RealType>(
          function,
          "lower parameter is %1%, but must be less than upper!", lower);
        return false;
      }
    } // bool check_uniform(

  } // namespace detail

  template <class RealType = double>
  class uniform_distribution
  {
  public:
    typedef RealType value_type;

    uniform_distribution(RealType lower = 0, RealType upper = 1) // Constructor.
      : m_lower(lower), m_upper(upper) // Default is standard uniform distribution.
    {
      RealType result;
      detail::check_uniform(BOOST_CURRENT_FUNCTION, lower, upper, &result);
    }
    // Accessor functions.
    RealType lower()const
    {
      return m_lower;
    }

    RealType upper()const
    {
      return m_upper;
    }
  private:
    // Data members:
    RealType m_lower;  // distribution lower aka a.
    RealType m_upper;  // distribution upper aka b.
  }; // class uniform_distribution

  typedef uniform_distribution<double> uniform;

  template <class RealType>
  RealType pdf(const uniform_distribution<RealType>& dist, const RealType& x)
  {
    RealType lower = dist.lower();
    RealType upper = dist.upper();
    RealType result; // of checks.
    if(false == detail::check_uniform(BOOST_CURRENT_FUNCTION,lower, upper, &result))
    {
      return result;
    }
    if(false == detail::check_uniform_x(BOOST_CURRENT_FUNCTION, x, &result))
    {
      return result;
    }

    if((x < lower) || (x > upper) )
    {
      return 0;
    }
    else 
    {
      return 1 / (upper - lower);
    }
  } // RealType pdf(const uniform_distribution<RealType>& dist, const RealType& x)

  template <class RealType>
  inline RealType cdf(const uniform_distribution<RealType>& dist, const RealType& x)
  {
    RealType lower = dist.lower();
    RealType upper = dist.upper();
    RealType result; // of checks.
    if(false == detail::check_uniform(BOOST_CURRENT_FUNCTION,lower, upper, &result))
    {
      return result;
    }
    if(false == detail::check_uniform_x(BOOST_CURRENT_FUNCTION, x, &result))
    {
      return result;
    }
    if (x < lower) 
    {
      return 0;
    }
    if (x > upper)
    {
      return 1;
    }
    return (x - lower) / (upper - lower); // lower <= x <= upper
  } // RealType cdf(const uniform_distribution<RealType>& dist, const RealType& x)

  template <class RealType>
  RealType quantile(const uniform_distribution<RealType>& dist, const RealType& p)
  {
    RealType lower = dist.lower();
    RealType upper = dist.upper();
    RealType result; // of checks
    if(false == detail::check_uniform(BOOST_CURRENT_FUNCTION,lower, upper, &result))
    {
      return result;
    }
    if(false == detail::check_probability(BOOST_CURRENT_FUNCTION, p, &result))
    {
      return result;
    }
    if(p == 0)
    {
      return lower;
    }
    if(p == 1)
    {
      return upper;
    }
    return p * (upper - lower) + lower;
  } // RealType quantile(const uniform_distribution<RealType>& dist, const RealType& p)

  template <class RealType>
  RealType cdf(const complemented2_type<uniform_distribution<RealType>, RealType>& c)
  {
    RealType lower = c.dist.lower();
    RealType upper = c.dist.upper();
    RealType x = c.param;
    RealType result; // of checks.
    if(false == detail::check_uniform(BOOST_CURRENT_FUNCTION, lower, upper, &result))
    {
      return result;
    }
    if(false == detail::check_uniform_x(BOOST_CURRENT_FUNCTION, x, &result))
    {
      return result;
    }
    if (x < lower) 
    {
      return 0;
    }
    if (x > upper)
    {
      return 1;
    }
    return (upper - x) / (upper - lower);
  } // RealType cdf(const complemented2_type<uniform_distribution<RealType>, RealType>& c)

  template <class RealType>
  RealType quantile(const complemented2_type<uniform_distribution<RealType>, RealType>& c)
  {
    RealType lower = c.dist.lower();
    RealType upper = c.dist.upper();
    RealType q = c.param;
    RealType result; // of checks.
    if(false == detail::check_uniform(BOOST_CURRENT_FUNCTION, lower, upper, &result))
    {
      return result;
    }
    if(false == detail::check_probability(BOOST_CURRENT_FUNCTION, q, &result))
      if(q == 0)
      {
        return lower;
      }
      if(q == 1)
      {
        return upper;
      }
      return -q * (upper - lower) + upper;
  } // RealType quantile(const complemented2_type<uniform_distribution<RealType>, RealType>& c)

  template <class RealType>
  inline RealType mean(const uniform_distribution<RealType>& dist)
  {
    RealType lower = dist.lower();
    RealType upper = dist.upper();
    RealType result;  // of checks.
    if(false == detail::check_uniform(BOOST_CURRENT_FUNCTION, lower, upper, &result))
    {
      return result;
    }
    return (lower + upper ) / 2;
  } // RealType mean(const uniform_distribution<RealType>& dist)

  template <class RealType>
  RealType variance(const uniform_distribution<RealType>& dist)
  {
    RealType lower = dist.lower();
    RealType upper = dist.upper();
    RealType result; // of checks.
    if(false == detail::check_uniform(BOOST_CURRENT_FUNCTION, lower, upper, &result))
    {
      return result;
    }
    return (upper - lower) * ( upper - lower) / 12;
    // for standard uniform = 0.833333333333333333333333333333333333333333;
  } // RealType variance(const uniform_distribution<RealType>& dist)

  template <class RealType>
  inline RealType mode(const uniform_distribution<RealType>& dist)
  {
    RealType lower = dist.lower();
    RealType upper = dist.upper();
    RealType result; // of checks.
    if(false == detail::check_uniform(BOOST_CURRENT_FUNCTION, lower, upper, &result))
    {
      return result;
    }
    result = lower; // Any value [lower, upper] but arbitrarily choose lower.
    return result;
  }

  template <class RealType>
  inline RealType median(const uniform_distribution<RealType>& dist)
  {
    RealType lower = dist.lower();
    RealType upper = dist.upper();
    RealType result; // of checks.
    if(false == detail::check_uniform(BOOST_CURRENT_FUNCTION, lower, upper, &result))
    {
      return result;
    }
    return (lower + upper) / 2; // 
  }
  template <class RealType>
  inline RealType skewness(const uniform_distribution<RealType>& dist)
  {
    RealType lower = dist.lower();
    RealType upper = dist.upper();
    RealType result; // of checks.
    if(false == detail::check_uniform(BOOST_CURRENT_FUNCTION,lower, upper, &result))
    {
      return result;
    }
    return 0;
  } // RealType skewness(const uniform_distribution<RealType>& dist)

  template <class RealType>
  inline RealType kurtosis_excess(const uniform_distribution<RealType>& dist)
  {
    RealType lower = dist.lower();
    RealType upper = dist.upper();
    RealType result;  // of checks.
    if(false == detail::check_uniform(BOOST_CURRENT_FUNCTION, lower, upper, &result))
    {
      return result;
    }
    return static_cast<RealType>(-6)/5; //  -6/5 = -1.2;  
  } // RealType kurtosis_excess(const uniform_distribution<RealType>& dist)

  template <class RealType>
  inline RealType kurtosis(const uniform_distribution<RealType>& dist)
  {
    return kurtosis_excess(dist) + 3;
  }

} // namespace math
} // namespace boost

// This include must be at the end, *after* the accessors
// for this distribution have been defined, in order to
// keep compilers that support two-phase lookup happy.
#include <boost/math/distributions/detail/derived_accessors.hpp>

#endif // BOOST_STATS_UNIFORM_HPP


