
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
