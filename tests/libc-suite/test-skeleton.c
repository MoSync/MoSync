#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#ifndef TEST_FUNCTION
# define TEST_FUNCTION do_test (argc, argv)
#endif
 
int main() {
	int argc = 0;
	const char* argv[] = { NULL };
#ifdef PREPARE
	PREPARE(0,0);
#endif
	return TEST_FUNCTION;
}

/* PID of the test itself.  */
static pid_t pid;

/* Directory to place temporary files in.  */
static const char *test_dir = "";

/* List of temporary files.  */
struct temp_name_list
{
  struct qelem q;
  const char *name;
} *temp_name_list;

/* Add temporary files in list.  */
static void
__attribute__ ((unused))
add_temp_file (const char *name)
{
  struct temp_name_list *newp
    = (struct temp_name_list *) calloc (sizeof (*newp), 1);
  if (newp != NULL)
    {
      newp->name = name;
      if (temp_name_list == NULL)
	temp_name_list = (struct temp_name_list *) &newp->q;
      else
	insque (newp, temp_name_list);
    }
}

/* Delete all temporary files.  */
static void
delete_temp_files (void)
{
  while (temp_name_list != NULL)
    {
      remove (temp_name_list->name);
      temp_name_list = (struct temp_name_list *) temp_name_list->q.q_forw;
    }
}

/* Create a temporary file.  */
static int
__attribute__ ((unused))
create_temp_file (const char *base, char **filename)
{
  char *fname;
  int fd;

  fname = (char *) malloc (strlen (test_dir) + 1 + strlen (base)
			   + sizeof ("XXXXXX"));
  if (fname == NULL)
    {
      puts ("out of memory");
      return -1;
    }
  strcpy (stpcpy (stpcpy (stpcpy (fname, test_dir), "/"), base), "XXXXXX");

  fd = mkstemp (fname);
  if (fd == -1)
    {
      printf ("cannot open temporary file '%s': %m\n", fname);
      free (fname);
      return -1;
    }

  add_temp_file (fname);
  if (filename != NULL)
    *filename = fname;

  return fd;
}
