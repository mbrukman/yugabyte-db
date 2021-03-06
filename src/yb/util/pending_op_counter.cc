// Copyright (c) YugaByte, Inc.
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

#include "yb/util/pending_op_counter.h"

#include <glog/logging.h>

#include "yb/gutil/strings/substitute.h"
#include "yb/util/monotime.h"
#include "yb/util/status.h"

using strings::Substitute;

namespace yb {
namespace util {

// The implementation is based on OperationTracker::WaitForAllToFinish.
Status PendingOperationCounter::WaitForOpsToFinish(const MonoDelta& timeout,
                                                   uint64_t num_remaining_ops) const {
  const int complain_ms = 1000;
  MonoTime start_time = MonoTime::Now();
  int64_t num_pending_ops = 0;
  int num_complaints = 0;
  int wait_time_usec = 250;
  while ((num_pending_ops = GetOpCounter()) > num_remaining_ops) {
    const MonoDelta diff = MonoTime::Now().GetDeltaSince(start_time);
    if (diff.MoreThan(timeout)) {
      return STATUS(TimedOut, Substitute(
          "Timed out waiting for all pending operations to complete. "
          "$0 transactions pending. Waited for $1",
          num_pending_ops, diff.ToString()));
    }
    const int64_t waited_ms = diff.ToMilliseconds();
    if (waited_ms / complain_ms > num_complaints) {
      LOG(WARNING) << Substitute("Waiting for $0 pending operations to complete now for $1 ms",
                                 num_pending_ops, waited_ms);
      num_complaints++;
    }
    wait_time_usec = std::min(wait_time_usec * 5 / 4, 1000000);
    SleepFor(MonoDelta::FromMicroseconds(wait_time_usec));
  }
  CHECK_EQ(num_pending_ops, num_remaining_ops) << "Number of pending operations must be " <<
      num_remaining_ops;
  return Status::OK();
}

}  // namespace util
}  // namespace yb
