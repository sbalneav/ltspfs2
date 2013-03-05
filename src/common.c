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

Bool
LTSPFS_AtomWait (InfoStruct *is, Atom atom)
{
  XEvent ev;

  /*
   * Declare our interest in obtaining PropertyChange events.
   */

  XSelectInput (is->dpy, is->win, PropertyChangeMask);

  for (;;)
    {
      XNextEvent (is->dpy, &ev);
      if (ev.type == PropertyNotify)
	{
          if (ev.xproperty.atom == atom)
	    {
	      return True;
	    }
        }
    }
}

Bool
LTSPFS_GetLong (InfoStruct *is, Atom atom, long *result)
{
  long *property = NULL;
  Atom type;
  int format, res;
  unsigned long nitems, nleft;
  Bool retval;

  res = XGetWindowProperty(is->dpy, is->win, atom, 0, 1, 0, XA_INTEGER,
                           &type, &format, &nitems, &nleft,
			   (unsigned char **) &property);

  if ((res == Success) && (type == XA_INTEGER))
    {
      *result = *property;
      retval = True;
    }
  else
    {
      retval = False;
    }

  if (property != NULL)
    {
      XFree ((void *) property);
    }

  return retval;
}

Bool
LTSPFS_GetString (InfoStruct *is, Atom atom, long len, char **result)
{
  char *property;
  Atom type;
  int format, res;
  unsigned long nitems, nleft;
  Bool retval;

  res = XGetWindowProperty(is->dpy, is->win, atom, 0, len, 0, XA_STRING,
                           &type, &format, &nitems, &nleft,
   		           (unsigned char **) &property);

  if ((res == Success) && (type == XA_STRING))
    {
      *result = strdup (property);
      retval = True;
    }
  else
    {
      retval = False;
    }

  if (property != NULL)
    {
      XFree ((void *) property);
    }

   return retval;
}

Bool
LTSPFS_PutLong (InfoStruct *is, Atom atom, long *data)
{
  if (XChangeProperty (is->dpy, is->win, atom, XA_INTEGER, 32,
		       PropModeReplace, (unsigned char *)data, 1) != Success)
    {
      return False;
    }
  else
    {
      return True;
    }
}

Bool
LTSPFS_PutString (InfoStruct *is, Atom atom, long len, char *data)
{
  if (XChangeProperty (is->dpy, is->win, atom, XA_STRING, 8,
		       PropModeReplace, (unsigned char *)data,
		       len) != Success)
    {
      return False;
    }
  else
    {
      return True;
    }
}

Bool
LTSPFS_InitAtoms (InfoStruct *is, Bool only_if_exists)
{
  int i;
  AtomStore **store = is->store;

  for (i = 0; store[i].name != NULL; i++)
    {
      if ((store[i].atom =
	   XInternAtom (dpy, store[i].name, only_if_exists)) == None)
	{
	  return False;
	}
    }

  return True;
}

Atom
LTSPFS_GetAtom (InfoStruct *is, char *name)
{
  int i;
  AtomStore **store = is->store;

  for (i = 0; store[i]->name != NULL; i++)
    {
      if (strcmp (store[i]->name, name) == 0)
	{
	  return store[i]->atom;
	}
    }

  return None;
}

Bool
LTSPFS_StoreStatBuf (InfoStruct *is, struct stat *st)
{
  Atom tmp;

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_MODE)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, tmp, (long) st->st_mode) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_NLINK)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, tmp, (long) st->st_nlink) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_UID)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, tmp, (long) st->st_uid) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_GID)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, tmp, (long) st->st_gid) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_SIZE)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, tmp, (long) st->st_size) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_ATIME)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, tmp, (long) st->st_atime) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_MTIME)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, tmp, (long) st->st_mtime) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_CTIME)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, tmp, (long) st->st_ctime) != True)
    {
      return False;
    }
}

Bool
LTSPFS_GetStatBuf (InfoStruct *is, struct stat *st)
{
  Atom tmp;

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_MODE)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, tmp, (long) st->st_mode) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_NLINK)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, tmp, (long) st->st_nlink) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_UID)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, tmp, (long) st->st_uid) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_GID)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, tmp, (long) st->st_gid) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_SIZE)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, tmp, (long) st->st_size) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_ATIME)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, tmp, (long) st->st_atime) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_MTIME)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, tmp, (long) st->st_mtime) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (is, LTSPFS_CTIME)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, tmp, (long) st->st_ctime) != True)
    {
      return False;
    }
}
