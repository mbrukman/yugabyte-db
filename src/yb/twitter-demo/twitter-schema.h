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
// Inline functions to create the Twitter schema
#ifndef YB_TWITTER_DEMO_TWITTER_SCHEMA_H
#define YB_TWITTER_DEMO_TWITTER_SCHEMA_H

#include "yb/client/schema.h"

namespace yb {
namespace twitter_demo {

using client::YBColumnSchema;
using client::YBSchema;
using client::YBSchemaBuilder;

inline YBSchema CreateTwitterSchema() {
  YBSchema s;
  YBSchemaBuilder b;
  b.AddColumn("tweet_id")->Type(YBColumnSchema::INT64)->NotNull()->PrimaryKey();
  b.AddColumn("text")->Type(YBColumnSchema::STRING)->NotNull();
  b.AddColumn("source")->Type(YBColumnSchema::STRING)->NotNull();
  b.AddColumn("created_at")->Type(YBColumnSchema::STRING)->NotNull();
  b.AddColumn("user_id")->Type(YBColumnSchema::INT64)->NotNull();
  b.AddColumn("user_name")->Type(YBColumnSchema::STRING)->NotNull();
  b.AddColumn("user_description")->Type(YBColumnSchema::STRING)->NotNull();
  b.AddColumn("user_location")->Type(YBColumnSchema::STRING)->NotNull();
  b.AddColumn("user_followers_count")->Type(YBColumnSchema::INT32)->NotNull();
  b.AddColumn("user_friends_count")->Type(YBColumnSchema::INT32)->NotNull();
  b.AddColumn("user_image_url")->Type(YBColumnSchema::STRING)->NotNull();
  CHECK_OK(b.Build(&s));
  return s;
}

} // namespace twitter_demo
} // namespace yb
#endif

/*

Schema for Impala:

CREATE TABLE twitter (
  tweet_id bigint,
  text string,
  source string,
  created_at string,
  user_id bigint,
  user_name string,
  user_description string,
  user_location string,
  user_followers_count int,
  user_friends_count int,
  user_image_url string);


Schema for MySQL:

CREATE TABLE twitter (
  tweet_id bigint not null primary key,
  tweet_text varchar(1000) not null,
  source varchar(1000) not null,
  created_at varchar(1000) not null,
  user_id bigint not null,
  user_name varchar(1000) not null,
  user_description varchar(1000) not null,
  user_location varchar(1000) not null,
  user_followers_count int not null,
  user_friends_count int not null,
  user_image_url varchar(1000) not null);

*/