/*
 Copyright 2020 Alain Dargelas

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * File:   NumUtils.h
 * Author: alain
 *
 * Created on Dec 29, 2020, 10:43 PM
 */

#ifndef SURELOG_NUMUTILS_H
#define SURELOG_NUMUTILS_H
#pragma once

#include <cstdint>
#include <string>
#include <charconv>
#include <string_view>
#include <ctype.h>

namespace SURELOG {

// These funcionts parse a number from a std::string_view into "result".
// Returns the end of the number on success, nullptr otherwise.
// So this can be used in a simple boolean context for success testing, but
// as well in parsing context where advancing to the next position
// is needed.

// Parse any number type with std::from_chars
namespace internal {
template <typename number_type>
const char *convert_strto_num(std::string_view s, number_type *result) {
  while (!s.empty() && isspace(s.front())) s.remove_prefix(1);
  if (!s.empty() && s.front() == '+') s.remove_prefix(1);
  auto success = std::from_chars(s.data(), s.data() + s.size(), *result);
  return (success.ec == std::errc()) ? success.ptr : nullptr;
}
}

// Parse an integer type (int32_, int64_t, uint32_t, uint64_t)
// Must check result: returns 'nullptr' if parse not successful.
template <typename int_type>
[[nodiscard]] const char *convert_strtoi(std::string_view s, int_type *result) {
  return internal::convert_strto_num(s, result);
}

// Parse a float value
// Must check result: returns 'nullptr' if parse not successful.
[[nodiscard]] const char *convert_strtof(std::string_view s, float *result);

// Parse a double value
// Must check result: returns 'nullptr' if parse not successful.
[[nodiscard]] const char *convert_strtod(std::string_view s, double *result);

// Parse a long double value
// Must check result: returns 'nullptr' if parse not successful.
[[nodiscard]] const char *convert_strtold(std::string_view s,
                                          long double *result);

class NumUtils final {
 public:
  static std::string hexToBin(const std::string &s);

  static std::string binToHex(const std::string &s);

  static std::string toBinary(int size, uint64_t val);

  static std::string trimLeadingZeros(const std::string &s);

  static uint64_t getMask(uint64_t wide);

 private:
  NumUtils() = delete;
  NumUtils(const NumUtils &orig) = delete;
  ~NumUtils() = delete;

 private:
};

};  // namespace SURELOG

#endif /* SURELOG_NUMUTILS_H */
