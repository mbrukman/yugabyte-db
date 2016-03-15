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

set(INTEGRATION_TESTS_SRCS
  cluster_itest_util.cc
  cluster_verifier.cc
  external_mini_cluster.cc
  external_mini_cluster_fs_inspector.cc
  mini_cluster.cc
  test_workload.cc
)

add_library(integration-tests ${INTEGRATION_TESTS_SRCS})
target_link_libraries(integration-tests
  tserver
  tserver_test_util
  master
  ysck
  yb_client
  yb_client_test_util
  yb_fs
  yb_test_util)
add_dependencies(integration-tests
  yb-tserver
  yb-master)

# Tests
set(YB_TEST_LINK_LIBS integration-tests ${YB_MIN_TEST_LIBS})
ADD_YB_TEST(alter_table-test)
ADD_YB_TEST(alter_table-randomized-test)
ADD_YB_TEST(registration-test RESOURCE_LOCK "master-web-port")
ADD_YB_TEST(client_failover-itest)
ADD_YB_TEST(client-stress-test
  RESOURCE_LOCK "master-rpc-ports"
  RUN_SERIAL true)
ADD_YB_TEST(master_replication-itest RESOURCE_LOCK "master-rpc-ports")
ADD_YB_TEST(master_failover-itest RESOURCE_LOCK "master-rpc-ports")
ADD_YB_TEST(raft_consensus-itest RUN_SERIAL true)
ADD_YB_TEST(ts_tablet_manager-itest)
ADD_YB_TEST(ts_recovery-itest)
ADD_YB_TEST(create-table-stress-test)
ADD_YB_TEST(delete_table-test)
ADD_YB_TEST(external_mini_cluster-test RESOURCE_LOCK "master-rpc-ports")
ADD_YB_TEST(linked_list-test RESOURCE_LOCK "master-rpc-ports")
ADD_YB_TEST(all_types-itest RESOURCE_LOCK "master-rpc-ports")
ADD_YB_TEST(remote_bootstrap-itest)
ADD_YB_TEST(tablet_replacement-itest)
ADD_YB_TEST(create-table-itest)

# Some tests have additional dependencies
set(YB_TEST_LINK_LIBS yb_client yb_tools_util ${YB_TEST_LINK_LIBS})
ADD_YB_TEST(full_stack-insert-scan-test RUN_SERIAL true)
ADD_YB_TEST(update_scan_delta_compact-test RUN_SERIAL true)
ADD_YB_TEST(flex_partitioning-itest)