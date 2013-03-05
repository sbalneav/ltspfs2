#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#include <X11/Xlib.h>

#include "ltspfs2.h"

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
mkpath (InfoStruct *is, char *patharg, char *pathbuf)
{
  char *fusepath;

  if (LTSPFS_GetString (is, patharg, &fusepath) != True)
    {
      return False;
    }

  snprintf (path, PATH_MAX, "%s/%s", is->root, fusepath);

  free (fusepath);

  return True;
}

/*
 * getattr
 */

Bool
LTSPFS_getattr (InfoStruct *is, long *retval)
{
  char path[PATH_MAX];
  struct stat statbuf;
  int ret;

  if (mkpath (is, LTSPFS_PATH1, path) == False)
    {
      return False;
    }

  if ((ret = stat (path, &statbuf)) < 0)
    {
      *retval = (long) ret;
      return True;
    }

  if (LTSPFS_StoreStatBuf (is, &st) == False)
    {
      return False;
    }

  *retval = (long) ret;

  return True;
}

/*
 * readdir
 */

Bool
LTSPFS_readdir (InfoStruct *is, long *retval)
{
  char path[PATH_MAX];
  char *dirnames = NULL;
  char *last = dirnames;
  long total = 0;
  int ret;
  DIR *de;
  struct dirent *dire;
  Atom diratom;

  if (mkpath (is, LTSPFS_PATH1, path) != True)
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

  if ((diratom = LTSPFS_GetAtom (is, LTSPFS_DIRLIST)) == None)
    {
      return False;
    }

  if ((LTSPFS_PutString (is, diratom, total, dirnames)) == False)
    {
      return False;
    }

  *retval = 0;

  return True;
}
