/*
 Copyright 2020 The Surelog Team.

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

#include <Surelog/Utils/NumUtils.h>
#include <gtest/gtest.h>

namespace SURELOG {
TEST(NumUtilsTest, ParseDecimalInt) {
  int64_t value;
  EXPECT_FALSE(parse_int64("hello", &value));
  EXPECT_TRUE(parse_int64("123", &value));
  EXPECT_EQ(123, value);
  EXPECT_TRUE(parse_int64("+456", &value));
  EXPECT_EQ(456, value);
  EXPECT_TRUE(parse_int64("-789", &value));
  EXPECT_EQ(-789, value);
  EXPECT_TRUE(parse_int64(" 123 ", &value));
  EXPECT_EQ(123, value);

  // Make sure we can parse beyond 32 bit.
  EXPECT_TRUE(parse_int64("12345678901234", &value));
  EXPECT_EQ(12345678901234LL, value);

  // Make sure we're not assumming a nul-byte at a particular point
  std::string_view longer_string("4255");
  EXPECT_TRUE(parse_int64(longer_string.substr(0, 2), &value));
  EXPECT_EQ(42, value);

  // Make sure the returned value points to the characters after the number.
  const std::string_view input = " +314cm";
  const std::string_view expected_remain = input.substr(input.find("cm"));
  const char *remain_string = parse_int64(input, &value);
  ASSERT_TRUE(remain_string);
  EXPECT_EQ(314, value);
  EXPECT_EQ(remain_string, expected_remain.data());  // pointers must match.
}

TEST(NumUtilsTest, ValidateRangeSigned32) {
  int32_t value;
  EXPECT_TRUE(parse_int32("-1", &value));
  EXPECT_EQ(value, -1);

  EXPECT_TRUE(parse_int32("2147483647", &value));
  EXPECT_EQ(value, 2147483647);

  EXPECT_TRUE(parse_int32("-2147483648", &value));
  EXPECT_EQ(value, -2147483648);

  EXPECT_FALSE(parse_int32("2147483648", &value));   // out of range.
  EXPECT_FALSE(parse_int32("-2147483649", &value));  // out of range.
}

TEST(NumUtilsTest, ValidateRangeUnsigned32) {
  uint32_t value;
  EXPECT_FALSE(parse_uint32("-1", &value));

  EXPECT_TRUE(parse_uint32("4294967295", &value));
  EXPECT_EQ(value, 4294967295);

  EXPECT_FALSE(parse_uint32("4294967296", &value));  // out of range.
}

TEST(NumUtilsTest, ValidateRangeSigned64) {
  int64_t value;
  EXPECT_TRUE(parse_int64("-1", &value));
  EXPECT_EQ(value, -1);

  EXPECT_TRUE(parse_int64("9223372036854775807", &value));
  EXPECT_EQ(value, 9223372036854775807LL);

  EXPECT_TRUE(parse_int64("-9223372036854775808", &value));
  EXPECT_EQ(value, -9223372036854775807LL - 1);

  EXPECT_FALSE(parse_int64("9223372036854775808", &value));   // out of range.
  EXPECT_FALSE(parse_int64("-9223372036854775809", &value));  // out of range.
}

TEST(NumUtilsTest, ValidateRangeUnsigned64) {
  uint64_t value;
  EXPECT_FALSE(parse_uint64("-1", &value));

  EXPECT_TRUE(parse_uint64("18446744073709551615", &value));
  EXPECT_EQ(value, 18446744073709551615UL);

  EXPECT_FALSE(parse_uint64("18446744073709551616", &value));  // out of range.
}

TEST(NumUtilsTest, ParseLenientSigned32) {
  int32_t value;

  // Full signed range allowed
  EXPECT_TRUE(parse_int_lenient("2147483647", &value));
  EXPECT_EQ(value, 2147483647);

  EXPECT_TRUE(parse_int_lenient("-2147483648", &value));
  EXPECT_EQ(value, -2147483648);

  // Also full unsigned range allowed
  EXPECT_TRUE(parse_int_lenient("4294967295", &value));
  EXPECT_EQ(value, -1);  // ... that aliases into negative signed

  EXPECT_FALSE(parse_int_lenient("-2147483649", &value));  // range
  EXPECT_FALSE(parse_int_lenient("4294967296", &value));   // range
}

TEST(NumUtilsTest, ParseLenientUnsigned32) {
  uint32_t value;

  EXPECT_TRUE(parse_int_lenient("4294967295", &value));
  EXPECT_EQ(value, 4294967295);

  EXPECT_TRUE(parse_int_lenient("-1", &value));  // Signed value
  EXPECT_EQ(value, 4294967295);  // .. interpreted as all bits set

  EXPECT_TRUE(parse_int_lenient(" -1 ", &value));  // also recognize with space
  EXPECT_EQ(value, 4294967295);

  // Full signed range allowed
  EXPECT_TRUE(parse_int_lenient("2147483647", &value));
  EXPECT_EQ(value, 2147483647);

  EXPECT_TRUE(parse_int_lenient("-2147483648", &value));
  EXPECT_EQ(value, 2147483648);

  EXPECT_FALSE(parse_int_lenient("-2147483649", &value));  // range
  EXPECT_FALSE(parse_int_lenient("4294967296", &value));   // range
}

TEST(NumUtilsTest, ParseLenientSigned64) {
  int64_t value;

  // Full signed range allowed
  EXPECT_TRUE(parse_int_lenient("9223372036854775807", &value));
  EXPECT_EQ(value, 9223372036854775807L);

  EXPECT_TRUE(parse_int_lenient("-9223372036854775808", &value));
  EXPECT_EQ(value, -9223372036854775807L - 1);

  // Also full unsigned range allowed
  EXPECT_TRUE(parse_int_lenient("18446744073709551615", &value));
  EXPECT_EQ(value, -1);  // ... that aliases into negative signed

  EXPECT_FALSE(parse_int_lenient("-9223372036854775809", &value));  // range
  EXPECT_FALSE(parse_int_lenient("18446744073709551616", &value));  // range
}

TEST(NumUtilsTest, ParseLenientUnsigned64) {
  uint64_t value;

  EXPECT_TRUE(parse_int_lenient("18446744073709551615", &value));
  EXPECT_EQ(value, 18446744073709551615UL);

  EXPECT_TRUE(parse_int_lenient("-1", &value));  // Signed value
  EXPECT_EQ(value, 18446744073709551615UL);  // .. interpreted as all bits set

  EXPECT_TRUE(parse_int_lenient(" -1 ", &value));  // also recognize with space
  EXPECT_EQ(value, 18446744073709551615UL);

  // Full signed range allowed
  EXPECT_TRUE(parse_int_lenient("9223372036854775807", &value));
  EXPECT_EQ(value, 9223372036854775807UL);

  EXPECT_TRUE(parse_int_lenient("-9223372036854775808", &value));
  EXPECT_EQ(value, 9223372036854775808UL);

  EXPECT_FALSE(parse_int_lenient("-9223372036854775809", &value));  // range
  EXPECT_FALSE(parse_int_lenient("18446744073709551616", &value));  // range
}

TEST(NumUtilsTest, ParseFloat) {
  float value;
  EXPECT_FALSE(parse_float("hello", &value));
  EXPECT_TRUE(parse_float("123", &value));
  EXPECT_EQ(123, value);
  EXPECT_TRUE(parse_float("+456.5", &value));
  EXPECT_EQ(456.5, value);
  EXPECT_TRUE(parse_float("-789", &value));
  EXPECT_EQ(-789, value);
  EXPECT_TRUE(parse_float(" 123 ", &value));  // leading space
  EXPECT_EQ(123, value);

  // Make sure the returned value points to the characters after the number.
  const std::string_view input = " +314.159cm";
  const std::string_view expected_remain = input.substr(input.find("cm"));
  const char *remain_string = parse_float(input, &value);
  ASSERT_TRUE(remain_string);
  EXPECT_NEAR(314.159, value, 0.0001);
  EXPECT_EQ(remain_string, expected_remain.data());  // pointers must match.
}

TEST(NumUtilsTest, ParseDouble) {
  double value;
  EXPECT_FALSE(parse_double("hello", &value));
  EXPECT_TRUE(parse_double("123", &value));
  EXPECT_EQ(123, value);
  EXPECT_TRUE(parse_double("+456.5", &value));
  EXPECT_EQ(456.5, value);
  EXPECT_TRUE(parse_double("-789", &value));
  EXPECT_EQ(-789, value);
  EXPECT_TRUE(parse_double(" 123 ", &value));
  EXPECT_EQ(123, value);
  EXPECT_TRUE(parse_double("6.022e23", &value));
  EXPECT_EQ(6.022e23, value);

  // Make sure the returned value points to the characters after the number.
  const std::string_view input = " +314.159cm";
  const std::string_view expected_remain = input.substr(input.find("cm"));
  const char *remain_string = parse_double(input, &value);
  ASSERT_TRUE(remain_string);
  EXPECT_NEAR(314.159, value, 0.00001);
  EXPECT_EQ(remain_string, expected_remain.data());  // pointers must match.
}

TEST(NumUtilsTest, ParseLongDouble) {
  long double value;
  EXPECT_FALSE(parse_longdouble("hello", &value));
  EXPECT_TRUE(parse_longdouble("123", &value));
  EXPECT_EQ(123, value);
  EXPECT_TRUE(parse_longdouble("+456.5", &value));
  EXPECT_EQ(456.5, value);
  EXPECT_TRUE(parse_longdouble("-789", &value));
  EXPECT_EQ(-789, value);
  EXPECT_TRUE(parse_longdouble(" 123 ", &value));
  EXPECT_EQ(123, value);

  // Make sure the returned value points to the characters after the number.
  const std::string_view input = " +314.159cm";
  const std::string_view expected_remain = input.substr(input.find("cm"));
  const char *remain_string = parse_longdouble(input, &value);
  ASSERT_TRUE(remain_string);
  EXPECT_NEAR(314.159, value, 0.00001);
  EXPECT_EQ(remain_string, expected_remain.data());  // pointers must match.
}
}  // namespace SURELOG
