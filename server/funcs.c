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

/*
 * getattr
 */

Bool
LTSPFS_getattr (InfoStruct *is, long *retval)
{
  char path[PATH_MAX];
  char *fusepath;
  struct stat statbuf;
  int ret;

  if (LTSPFS_GetString (is, LTSPFS_PATH1, &fusepath) != True)
    {
      return False;
    }

  snprintf (path, sizeof path, "%s/%s", is->root, fusepath);

  free (fusepath);

  if ((ret = stat (path, &statbuf)) < 0)
    {
      *retval = (long) ret;
      return True;
    }

  if (LTSPFS_StoreStatBuf (is, &st) != True)
    {
      return False;
    }

  *retval = (long) ret;

  return True;
}
