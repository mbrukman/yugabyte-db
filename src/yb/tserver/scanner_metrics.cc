// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
// The following only applies to changes made to this file as part of YugaByte development.
//
// Portions Copyright (c) YugaByte, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied.  See the License for the specific language governing permissions and limitations
// under the License.
//
#include "yb/tserver/scanner_metrics.h"

#include "yb/util/metrics.h"

METRIC_DEFINE_counter(server, scanners_expired,
                      "Scanners Expired",
                      yb::MetricUnit::kScanners,
                      "Number of scanners that have expired since service start");

METRIC_DEFINE_histogram(server, scanner_duration,
                        "Scanner Duration",
                        yb::MetricUnit::kMicroseconds,
                        "Histogram of the duration of active scanners on this tablet.",
                        60000000LU, 2);

namespace yb {

namespace tserver {

ScannerMetrics::ScannerMetrics(const scoped_refptr<MetricEntity>& metric_entity)
    : scanners_expired(
          METRIC_scanners_expired.Instantiate(metric_entity)),
      scanner_duration(METRIC_scanner_duration.Instantiate(metric_entity)) {
}

void ScannerMetrics::SubmitScannerDuration(const CoarseMonoClock::TimePoint& time_started) {
  scanner_duration->Increment(ToMicroseconds(CoarseMonoClock::Now() - time_started));
}

} // namespace tserver
} // namespace yb
