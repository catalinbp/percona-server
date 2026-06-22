/* Copyright (c) 2021, 2026, Oracle and/or its affiliates.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

   This program is designed to work with certain software (including
   but not limited to OpenSSL) that is licensed under separate terms,
   as designated in a particular file or component or in included license
   documentation.  The authors of MySQL hereby grant you an additional
   permission to link the program and your derivative works with the
   separately licensed software that they have either included with
   the program or referenced in the documentation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License, version 2.0, for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

#include <openssl/err.h>   // Err_clear_error
#include <openssl/rand.h>  // RAND_bytes

#include <random>

#include "utils.h"

namespace keyring_common::utils {

/**
  Generate random data

  @param [in, out] data   Managed pointer to data. Should have been initialized
  @param [in]      length Length of the data to be generated

  @returns status of random data generation
    @retval true  Success. data contains generated data.
    @retval false Error. Do not rely on data.
*/
bool get_random_data(const std::unique_ptr<unsigned char[]> &data,
                     size_t length) {
  if (!data || length == 0) return false;
  if (!RAND_bytes(data.get(), length)) {
    ERR_clear_error();
    return false;
  }
  return true;
}

/**
  Generate a random number of iterations(+/- 10%) from a random distribution
  around the provided mean_iterations value.

  @param [in] mean_iterations the value to use as pivot for the random
  distribution.
  @return random uint between mean_iterations - 10% and mean_iterations + 10%
 */
uint32_t get_random_iterations(const uint32_t mean_iterations) {
  static std::random_device r;
  static std::default_random_engine el(r());
  std::uniform_int_distribution<std::size_t> dist(
      static_cast<int>(mean_iterations * 0.9),
      static_cast<int>(mean_iterations * 1.1));
  return dist(el);
}

}  // namespace keyring_common::utils
