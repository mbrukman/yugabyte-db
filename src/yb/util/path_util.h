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
// Utility methods for dealing with file paths.
#ifndef YB_UTIL_PATH_UTIL_H
#define YB_UTIL_PATH_UTIL_H

#include <string>

namespace yb {

// Join two path segments with the appropriate path separator,
// if necessary.
std::string JoinPathSegments(const std::string &a,
                             const std::string &b);

// Return the enclosing directory of path.
// This is like dirname(3) but for C++ strings.
std::string DirName(const std::string& path);

// Return the terminal component of a path.
// This is like basename(3) but for C++ strings.
std::string BaseName(const std::string& path);

} // namespace yb
#endif /* YB_UTIL_PATH_UTIL_H */