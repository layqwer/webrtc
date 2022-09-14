/*
 *  Copyright (c) 2022 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef API_TEST_METRICS_METRIC_H_
#define API_TEST_METRICS_METRIC_H_

#include <map>
#include <string>
#include <vector>

#include "absl/types/optional.h"
#include "api/units/timestamp.h"

namespace webrtc {
namespace test {

enum class Unit {
  kTimeMs,
  kPercent,
  kSizeInBytes,
  kKilobitsPerSecond,
  kHertz,
  // General unitless value. Can be used either for dimensionless quantities
  // (ex ratio) or for units not presented in this enum and too specific to add
  // to this enum.
  kUnitless,
  kCount
};

absl::string_view ToString(Unit unit);

enum class ImprovementDirection {
  kBiggerIsBetter,
  kNeitherIsBetter,
  kSmallerIsBetter
};

absl::string_view ToString(ImprovementDirection direction);

struct Metric {
  struct TimeSeries {
    struct Sample {
      // Timestamp in microseconds associated with a sample. For example,
      // the timestamp when the sample was collected.
      webrtc::Timestamp timestamp;
      double value;
      // Metadata associated with this particular sample.
      std::map<std::string, std::string> sample_metadata;
    };

    // All samples collected for this metric. It can be empty if the Metric
    // object only contains `stats`.
    std::vector<Sample> samples;
  };

  // Contains metric's precomputed statistics based on the `time_series` or if
  // `time_series` is omitted (has 0 samples) contains precomputed statistics
  // provided by the metric's calculator.
  struct Stats {
    // Sample mean of the metric
    // (https://en.wikipedia.org/wiki/Sample_mean_and_covariance).
    absl::optional<double> mean;
    // Standard deviation (https://en.wikipedia.org/wiki/Standard_deviation).
    // Is undefined if `time_series` contains only a single value.
    absl::optional<double> stddev;
    absl::optional<double> min;
    absl::optional<double> max;
  };

  // Metric name, for example PSNR, SSIM, decode_time, etc.
  std::string name;
  Unit unit;
  ImprovementDirection improvement_direction;
  // If the metric is generated by a test, this field can be used to specify
  // this information.
  std::string test_case;
  // Metadata associated with the whole metric.
  std::map<std::string, std::string> metric_metadata;
  // Contains all samples of the metric collected during test execution.
  // It can be empty if the user only stores precomputed statistics into
  // `stats`.
  TimeSeries time_series;
  Stats stats;
};

}  // namespace test
}  // namespace webrtc

#endif  // API_TEST_METRICS_METRIC_H_
