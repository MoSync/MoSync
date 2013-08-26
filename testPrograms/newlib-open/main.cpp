/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <ma.h>
#include <maassert.h>
#include <conprint.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#include <errno.h>

int rc;
const char *dbname = "/my_db.sql3";

int MAMain() {
	rc = open( dbname, O_WRONLY | O_CREAT);
	fprintf(stderr, "rc= %-3d %i %s \n", rc, errno, dbname);
	close(rc);
	FREEZE;
	return 0;
}
