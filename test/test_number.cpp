/*
 * Copyright (c) 2015 Spotify AB
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#include <string>
#include <vector>

#include <boost/test/unit_test.hpp>

#include <spotify/json/codec/number.hpp>
#include <spotify/json/default_codec.hpp>
#include <spotify/json/encode_decode.hpp>

BOOST_AUTO_TEST_SUITE(spotify)
BOOST_AUTO_TEST_SUITE(json)
BOOST_AUTO_TEST_SUITE(codec)

namespace {

template<typename Codec>
typename Codec::object_type test_decode(const Codec &codec, const std::string &json) {
  decoding_context c(json.c_str(), json.c_str() + json.size());
  auto obj = codec.decode(c);
  BOOST_CHECK_EQUAL(c.position, c.end);
  return obj;
}

template<typename Codec>
void test_decode_partial(const Codec &codec, const std::string &json) {
  decoding_context c(json.c_str(), json.c_str() + json.size());
  codec.decode(c);
  BOOST_CHECK_NE(c.position, c.end);
}

template<typename Codec>
void test_decode_fail(const Codec &codec, const std::string &json) {
  decoding_context c(json.c_str(), json.c_str() + json.size());
  BOOST_CHECK_THROW(codec.decode(c), decode_exception);
}

struct example_t {
  std::string value;
};

}  // namespace

BOOST_AUTO_TEST_CASE(json_codec_number_should_construct) {
  number_t<double>();
  number_t<float>();
}

BOOST_AUTO_TEST_CASE(json_codec_number_should_construct_with_helper) {
  number<double>();
  number<float>();
}

BOOST_AUTO_TEST_CASE(json_codec_number_should_construct_with_default_codec) {
  default_codec<double>();
  default_codec<float>();
}

BOOST_AUTO_TEST_CASE(json_codec_number_should_encode_exactly) {
  BOOST_CHECK_EQUAL(encode(0.5), "0.5");
  BOOST_CHECK_EQUAL(encode(0.5f), "0.5");
}

BOOST_AUTO_TEST_CASE(json_codec_number_should_decode_exactly) {
  BOOST_CHECK_EQUAL(test_decode(number<double>(), "0.5"), 0.5);
  BOOST_CHECK_EQUAL(test_decode(number<float>(), "0.5"), 0.5);
}

BOOST_AUTO_TEST_CASE(json_codec_number_should_decode_with_rounding_errors) {
  // 1.1 is mean because it can't be encoded precisely using binary floats
  BOOST_CHECK_EQUAL(test_decode(number<double>(), "1.1"), 1.1);
  BOOST_CHECK_EQUAL(test_decode(number<float>(), "1.1"), 1.1f);
}

BOOST_AUTO_TEST_CASE(json_codec_number_should_not_decode_invalid_numbers) {
  test_decode_fail(number<double>(), "a");
  test_decode_fail(number<float>(), "NaN");
  test_decode_fail(number<float>(), "- 1.1");
  test_decode_partial(number<float>(), "1..1");
}

BOOST_AUTO_TEST_SUITE_END()  // codec
BOOST_AUTO_TEST_SUITE_END()  // json
BOOST_AUTO_TEST_SUITE_END()  // spotify