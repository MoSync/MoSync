# Copyright 2013 David Axmark
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

require 'fileutils'

include FileUtils::Verbose

cd "AndroidProject"

mkdir_p "assets"
mkdir_p "bin/com/mosync/java/android"
mkdir_p "libs/armeabi"
mkdir_p "src/com/mosync/internal/android"
mkdir_p "src/com/mosync/java/android"

cd ".."
