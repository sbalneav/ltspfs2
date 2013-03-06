#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#include <X11/Xlib.h>

#include <ltspfs2.h>

/*
 * The general flow for the functions is:
 * If the LTSPFS function ITSELF fails, i.e. LTSPFS_getattr can't store it's
 * attributes, or what have you, you return false.
 *
 * If the filesystem function call you're executing, i.e. stat () fails, you
 * return it's failure in the passed pointer to the retval, but you return
 * true.  This is because while the filesystem call has failed, the function
 * itself has worked.
 */

Bool
mkpath (char *patharg, char **pathptr)
{
  char *fusepath;
  size_t lenroot;
  size_t lenfuse;
  Atom pathatom;

  if ((pathatom = LTSPFS_GetAtom (patharg)) == None)
    {
      return False;
    }

  if (LTSPFS_GetString (pathatom, &fusepath) != True)
    {
      return False;
    }

  lenroot = strlen (root);
  lenfuse = strlen (fusepath);

  if ((*pathptr = malloc (lenroot + lenfuse + 1)) == NULL)
    {
      return False;
    }

  sprintf (*pathptr, "%s/%s", root, fusepath);

  free (fusepath);

  return True;
}

/*
 * getattr
 */

Bool
LTSPFS_getattr (long *retval)
{
  char *path;
  struct stat statbuf;
  int ret;

  if (mkpath (LTSPFS_PATH1, &path) == False)
    {
      return False;
    }

  if ((ret = stat (path, &statbuf)) < 0)
    {
      *retval = (long) ret;
      return True;
    }

  if (LTSPFS_StoreStatBuf (&statbuf) == False)
    {
      return False;
    }

  free (path);
  *retval = (long) ret;

  return True;
}

/*
 * readdir
 */

Bool
LTSPFS_readdir (long *retval)
{
  char *path;
  char *dirnames = NULL;
  char *last = dirnames;
  long total = 0;
  int ret;
  DIR *de;
  struct dirent *dire;
  Atom diratom;

  if (mkpath (LTSPFS_PATH1, &path) == False)
    {
      return False;
    }

  /*
   * Construct a buffer of the dir names.  The buffer will look like
   * this:
   *
   * dirname\0dirname\0dirname\0\0
   *
   * Note the double NULL at the end.
   */

  if ((de = opendir (path)) == NULL)
    {
      *retval = (long) errno;
      return True;
    }

  while ((dire = readdir (de)) != NULL)
    {
      int size;

      size = strlen (dire->d_name);

      /* current total + length of name + 1 for NULL */
      if ((dirnames = realloc (dirnames, total + size + 1)) == NULL)
	{
	  return False;
	}

      strcpy (last, dire->d_name);
      last += size + 1;          /* last will point past last string */
      total += size + 1;
    }

  /* NULL terminate the array */
  total++;
  if ((dirnames = realloc (dirnames, total)) == NULL)
    {
      return False;
    }
  *last = '\0';

  /*
   * dirnames is now our buffer of null-separated filenames, and total contains
   * the total length of the buffer.
   */

  if ((diratom = LTSPFS_GetAtom (LTSPFS_DIRLIST)) == None)
    {
      return False;
    }

  if ((LTSPFS_PutString (diratom, total, dirnames)) == False)
    {
      return False;
    }

  free (path);
  free (dirnames);
  *retval = 0;

  return True;
}
