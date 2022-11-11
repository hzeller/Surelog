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
TEST(StringUtilTest, ParseDecimalInt) {
  int64_t value;
  EXPECT_FALSE(convert_strtoi("hello", &value));
  EXPECT_TRUE(convert_strtoi("123", &value));
  EXPECT_EQ(123, value);
  EXPECT_TRUE(convert_strtoi("+456", &value));
  EXPECT_EQ(456, value);
  EXPECT_TRUE(convert_strtoi("-789", &value));
  EXPECT_EQ(-789, value);
  EXPECT_TRUE(convert_strtoi(" 123 ", &value));
  EXPECT_EQ(123, value);

  // Make sure we can parse beyond 32 bit.
  EXPECT_TRUE(convert_strtoi("12345678901234", &value));
  EXPECT_EQ(12345678901234LL, value);

  // Make sure we're not assumming a nul-byte at a particular point
  std::string_view longer_string("4255");
  EXPECT_TRUE(convert_strtoi(longer_string.substr(0, 2), &value));
  EXPECT_EQ(42, value);

  // Make sure the returned value points to the characters after the number.
  const std::string_view input = " +314cm";
  const std::string_view expected_remain = input.substr(input.find("cm"));
  const char *remain_string = convert_strtoi(input, &value);
  ASSERT_TRUE(remain_string);
  EXPECT_EQ(314, value);
  EXPECT_EQ(remain_string, expected_remain.data());  // pointers must match.
}

TEST(StringUtilTest, ValidateRangeSigned32) {
  int32_t value;
  EXPECT_TRUE(convert_strtoi("-1", &value));
  EXPECT_EQ(value, -1);

  EXPECT_TRUE(convert_strtoi("2147483647", &value));
  EXPECT_EQ(value, 2147483647);

  EXPECT_TRUE(convert_strtoi("-2147483648", &value));
  EXPECT_EQ(value, -2147483648);

  EXPECT_FALSE(convert_strtoi("2147483648", &value));   // out of range.
  EXPECT_FALSE(convert_strtoi("-2147483649", &value));  // out of range.
}

TEST(StringUtilTest, ValidateRangeUnsigned32) {
  uint32_t value;
  EXPECT_FALSE(convert_strtoi("-1", &value));

  EXPECT_TRUE(convert_strtoi("4294967295", &value));
  EXPECT_EQ(value, 4294967295);

  EXPECT_FALSE(convert_strtoi("4294967296", &value));  // out of range.
}

TEST(StringUtilTest, ValidateRangeSigned64) {
  int64_t value;
  EXPECT_TRUE(convert_strtoi("-1", &value));
  EXPECT_EQ(value, -1);

  EXPECT_TRUE(convert_strtoi("9223372036854775807", &value));
  EXPECT_EQ(value, 9223372036854775807LL);

  EXPECT_TRUE(convert_strtoi("-9223372036854775808", &value));
  EXPECT_EQ(value, -9223372036854775807LL - 1);

  EXPECT_FALSE(convert_strtoi("9223372036854775808", &value));  // out of range.
  EXPECT_FALSE(
      convert_strtoi("-9223372036854775809", &value));  // out of range.
}

TEST(StringUtilTest, ValidateRangeUnsigned64) {
  uint64_t value;
  EXPECT_FALSE(convert_strtoi("-1", &value));

  EXPECT_TRUE(convert_strtoi("18446744073709551615", &value));
  EXPECT_EQ(value, 18446744073709551615UL);

  EXPECT_FALSE(
      convert_strtoi("18446744073709551616", &value));  // out of range.
}

TEST(StringUtilTest, ParseFloat) {
  float value;
  EXPECT_FALSE(convert_strtof("hello", &value));
  EXPECT_TRUE(convert_strtof("123", &value));
  EXPECT_EQ(123, value);
  EXPECT_TRUE(convert_strtof("+456.5", &value));
  EXPECT_EQ(456.5, value);
  EXPECT_TRUE(convert_strtof("-789", &value));
  EXPECT_EQ(-789, value);
  EXPECT_TRUE(convert_strtof(" 123 ", &value));  // leading space
  EXPECT_EQ(123, value);

  // Make sure the returned value points to the characters after the number.
  const std::string_view input = " +314.159cm";
  const std::string_view expected_remain = input.substr(input.find("cm"));
  const char *remain_string = convert_strtof(input, &value);
  ASSERT_TRUE(remain_string);
  EXPECT_NEAR(314.159, value, 0.0001);
  EXPECT_EQ(remain_string, expected_remain.data());  // pointers must match.
}

TEST(StringUtilTest, ParseDouble) {
  double value;
  EXPECT_FALSE(convert_strtod("hello", &value));
  EXPECT_TRUE(convert_strtod("123", &value));
  EXPECT_EQ(123, value);
  EXPECT_TRUE(convert_strtod("+456.5", &value));
  EXPECT_EQ(456.5, value);
  EXPECT_TRUE(convert_strtod("-789", &value));
  EXPECT_EQ(-789, value);
  EXPECT_TRUE(convert_strtod(" 123 ", &value));
  EXPECT_EQ(123, value);
  EXPECT_TRUE(convert_strtod("6.022e23", &value));
  EXPECT_EQ(6.022e23, value);

  // Make sure the returned value points to the characters after the number.
  const std::string_view input = " +314.159cm";
  const std::string_view expected_remain = input.substr(input.find("cm"));
  const char *remain_string = convert_strtod(input, &value);
  ASSERT_TRUE(remain_string);
  EXPECT_NEAR(314.159, value, 0.00001);
  EXPECT_EQ(remain_string, expected_remain.data());  // pointers must match.
}

TEST(StringUtilTest, ParseLongDouble) {
  long double value;
  EXPECT_FALSE(convert_strtold("hello", &value));
  EXPECT_TRUE(convert_strtold("123", &value));
  EXPECT_EQ(123, value);
  EXPECT_TRUE(convert_strtold("+456.5", &value));
  EXPECT_EQ(456.5, value);
  EXPECT_TRUE(convert_strtold("-789", &value));
  EXPECT_EQ(-789, value);
  EXPECT_TRUE(convert_strtold(" 123 ", &value));
  EXPECT_EQ(123, value);

  // Make sure the returned value points to the characters after the number.
  const std::string_view input = " +314.159cm";
  const std::string_view expected_remain = input.substr(input.find("cm"));
  const char *remain_string = convert_strtold(input, &value);
  ASSERT_TRUE(remain_string);
  EXPECT_NEAR(314.159, value, 0.00001);
  EXPECT_EQ(remain_string, expected_remain.data());  // pointers must match.
}
}  // namespace SURELOG
