#ifndef sharcnet_summer_school_2021_paralg_tbb_control_hpp_
#define sharcnet_summer_school_2021_paralg_tbb_control_hpp_

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

#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <string>
#include <sstream>
#include <thread>

//===========================================================================

//
// Atr this time ISO C++ does not have any scheduling/control mechanisms to
// control parallel algorithms so one has to use the underlying 
// implementation's implementation of such. The GCC and Intel's compilers 
// both use Intel Threaded Building Blocks (TBB) to do this, so this code
// creates and returns a tbb::global_control object which must
// live for the duration of the desired executions of parallel algorithms.
//
// If the underlying implementation is unknown, then a meaningless
// value is returned.
//
// NOTE: One needs to use the TBB library from 2020 on. Use of older TBB
//       library versions will require some edits to this code.
//

#ifdef __cpp_lib_parallel_algorithm
#  if defined(__GNUC__)
#    include <tbb/global_control.h>
#    define PARALG_USING_TBB_CONTROLS
#  endif
#endif

//===========================================================================

namespace sharcnet_summer_school_2021 {

//===========================================================================

//
// Provide an easy way for the programmer to ask to use at most num_threads
// to execute code. 
//
// NOTE: The returned object must be kept alive for the duration
//       of the parallel executions where num_threads must hold.
//
// This avoids having to pollute code with non-ISO code using the 
// underlying TBB library. If the underlying library is not TBB then this
// call has no effect.
//
[[nodiscard]] inline auto use_num_of_threads(unsigned int const& num_threads)
{
#ifdef PARALG_USING_TBB_CONTROLS
  using namespace tbb;
  auto const result = 
    std::min(num_threads, std::thread::hardware_concurrency())
  ;
  std::cerr << "INFO: TBB will use up to " << num_threads << " thread(s).\n";
  return global_control(global_control::max_allowed_parallelism, result);
#else
  std::cerr << "WARNING: TBB is not being used.\n";
  return std::thread::hardware_concurrency(); // meaningless return value
#endif
}

//===========================================================================

//
// Request using the maximum number of threads suitable for the underlying
// hardware, e.g., number of CPU cores * the number of hyperthreads per
// CPU core.
//
// NOTE: The returned object must be kept alive for the duration
//       of the parallel executions where num_threads must hold.
//
[[nodiscard]] inline auto use_max_num_of_threads()
{
  return use_num_of_threads(std::thread::hardware_concurrency());
}

//===========================================================================

namespace detail {

inline unsigned int env_num_of_threads_or_provided(
  std::initializer_list<char const*> env_var_names,
  unsigned int const& num_threads
)
{
  for (auto const& env_var : env_var_names)
  {
    if (char *p = std::getenv(env_var); p != nullptr)
    {
      std::istringstream buf(p);
      if (size_t num; buf >> num)
        return num;
      else
        return num_threads;
    }
  }
  return num_threads;
}

} // namespace detail

//
// Request using the number of threads as specified in an environment
// variable or use the maximum the hardware allows.
//
// The environment variables checked are those emitted by the Slurm
// scheduler within a job or OMP_NUM_TASKS.
//
// NOTE: The returned object must be kept alive for the duration
//       of the parallel executions where num_threads must hold.
//
[[nodiscard]] inline auto use_env_num_of_threads_or_provided(
  unsigned int const& num_threads
)
{
  return 
    use_num_of_threads(
      detail::env_num_of_threads_or_provided(
        {
          "SLURM_CPUS_PER_TASK",
          "SLURM_CPUS_PER_NODE",
          "OMP_THREAD_LIMIT",
          "OMP_NUM_THREADS"
        },
        num_threads
      )
    )
  ;
}

[[nodiscard]] inline auto use_env_num_of_threads_or_max()
{
  return use_env_num_of_threads_or_provided(
    std::thread::hardware_concurrency()
  );
}

//===========================================================================

#undef PARALG_USING_TBB_CONTROLS

//===========================================================================

} // namespace sharcnet_summer_school_2021

//===========================================================================

#endif // sharcnet_summer_school_2021_paralg_tbb_control_hpp_
