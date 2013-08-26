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

SETTINGS = {
	#:source_path => '/glibc-2.11.1/',
	:source_path => '/MoSyncDev/MoSyncF/glibc/',
	:stop_on_fail => true,
	:rebuild_failed => true,
	:retry_failed => true,
	:rebuild_missing_log => true,
	:test_dead_code_elimination => false,
	:test_release => false,
	:copy_targets => [],
	:loader_base_url => 'http://10.0.2.2:4042/libctests/',
	#:loader_base_url => 'http://192.168.0.136:4042/',
	#:htdocs_dir => '/htdocs/',	# set to nil or false to disable copying.
	:htdocs_dir => '/wamp/www/libctests/',
	:write_master_dump => false,
	:copy_dce => false,
	:dce_only => false,
	:force_copy_htdocs => true,
}
