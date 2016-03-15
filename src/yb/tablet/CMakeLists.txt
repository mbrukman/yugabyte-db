# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

set(TABLET_SRCS
  tablet.cc
  tablet_bootstrap.cc
  tablet_metrics.cc
  tablet_peer_mm_ops.cc
  tablet_peer.cc
  transactions/transaction.cc
  transactions/alter_schema_transaction.cc
  transactions/transaction_driver.cc
  transactions/transaction_tracker.cc
  transactions/write_transaction.cc
  transaction_order_verifier.cc
  cfile_set.cc
  compaction.cc
  compaction_policy.cc
  delta_key.cc
  diskrowset.cc
  lock_manager.cc
  maintenance_manager.cc
  memrowset.cc
  multi_column_writer.cc
  mutation.cc
  mvcc.cc
  row_op.cc
  rowset.cc
  rowset_info.cc
  rowset_tree.cc
  svg_dump.cc
  tablet_metadata.cc
  rowset_metadata.cc
  deltafile.cc
  deltamemstore.cc
  delta_applier.cc
  delta_compaction.cc
  delta_iterator_merger.cc
  delta_stats.cc
  delta_store.cc
  delta_tracker.cc
)

PROTOBUF_GENERATE_CPP(
  TABLET_PROTO_SRCS TABLET_PROTO_HDRS TABLET_PROTO_TGTS
  SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..
  BINARY_ROOT ${CMAKE_CURRENT_BINARY_DIR}/../..
  PROTO_FILES
    tablet.proto
    metadata.proto)
set(TABLET_PROTO_LIBS
  protobuf
  fs_proto
  consensus_metadata_proto
  yb_common)
ADD_EXPORTABLE_LIBRARY(tablet_proto
  SRCS ${TABLET_PROTO_SRCS}
  DEPS ${TABLET_PROTO_LIBS}
  NONLINK_DEPS ${TABLET_PROTO_TGTS})

add_library(tablet ${TABLET_SRCS})
target_link_libraries(tablet
  tablet_proto
  yb_common
  cfile
  gutil
  server_common
  yb_fs
  yb_util
  consensus)

set(YB_TEST_LINK_LIBS tablet ${YB_MIN_TEST_LIBS})
ADD_YB_TEST(tablet-test)
ADD_YB_TEST(tablet_metadata-test)
ADD_YB_TEST(mt-tablet-test RUN_SERIAL true)
ADD_YB_TEST(compaction_policy-test)
ADD_YB_TEST(diskrowset-test)
ADD_YB_TEST(mt-diskrowset-test RUN_SERIAL true)
ADD_YB_TEST(memrowset-test)
ADD_YB_TEST(deltamemstore-test)
ADD_YB_TEST(deltafile-test)
ADD_YB_TEST(cfile_set-test)
ADD_YB_TEST(tablet-pushdown-test)
ADD_YB_TEST(tablet-schema-test)
ADD_YB_TEST(tablet_bootstrap-test)
ADD_YB_TEST(maintenance_manager-test)
ADD_YB_TEST(metadata-test)
ADD_YB_TEST(mvcc-test)
ADD_YB_TEST(compaction-test)
ADD_YB_TEST(lock_manager-test)
ADD_YB_TEST(rowset_tree-test)
ADD_YB_TEST(composite-pushdown-test)
ADD_YB_TEST(delta_compaction-test)
ADD_YB_TEST(mt-rowset_delta_compaction-test)
ADD_YB_TEST(major_delta_compaction-test)
ADD_YB_TEST(transactions/transaction_tracker-test)
ADD_YB_TEST(tablet_peer-test)
ADD_YB_TEST(tablet_random_access-test)
ADD_YB_TEST(tablet_mm_ops-test)

# Some tests don't have dependencies on other tablet stuff
set(YB_TEST_LINK_LIBS yb_util gutil ${YB_MIN_TEST_LIBS})
ADD_YB_TEST(cbtree-test RUN_SERIAL true)