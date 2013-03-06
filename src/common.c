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
LTSPFS_AtomWait (Atom atom)
{
  XEvent ev;

  /*
   * Declare our interest in obtaining PropertyChange events.
   */

  XSelectInput (dpy, window, PropertyChangeMask);

  for (;;)
    {
      XNextEvent (dpy, &ev);
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
LTSPFS_GetLong (Atom atom, long *result)
{
  long *property = NULL;
  Atom type;
  int format, res;
  unsigned long nitems, nleft;
  Bool retval;

  res = XGetWindowProperty(dpy, window, atom, 0, 1, 0, XA_INTEGER,
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
LTSPFS_GetString (Atom atom, long len, char **result)
{
  char *property;
  Atom type;
  int format, res;
  unsigned long nitems, nleft;
  Bool retval;

  res = XGetWindowProperty(dpy, window, atom, 0, len, 0, XA_STRING,
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
LTSPFS_PutLong (Atom atom, long data)
{
  if (XChangeProperty (dpy, window, atom, XA_INTEGER, 32,
		       PropModeReplace, (unsigned char *)&data, 1) != Success)
    {
      return False;
    }
  else
    {
      return True;
    }
}

Bool
LTSPFS_PutString (Atom atom, long len, char *data)
{
  if (XChangeProperty (dpy, window, atom, XA_STRING, 8,
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
LTSPFS_InitAtoms (Bool only_if_exists)
{
  int i;

  i = 0;

  store[i].atom = None; store[i].name = LTSPFS_OPCODE; i++;
  store[i].atom = None; store[i].name = LTSPFS_RETVAL; i++;
  store[i].atom = None; store[i].name = LTSPFS_DATA; i++;
  store[i].atom = None; store[i].name = LTSPFS_DIRLIST; i++;
  store[i].atom = None; store[i].name = LTSPFS_MODE; i++;
  store[i].atom = None; store[i].name = LTSPFS_NLINK; i++;
  store[i].atom = None; store[i].name = LTSPFS_UID; i++;
  store[i].atom = None; store[i].name = LTSPFS_GID; i++;
  store[i].atom = None; store[i].name = LTSPFS_SIZE; i++;
  store[i].atom = None; store[i].name = LTSPFS_ATIME; i++;
  store[i].atom = None; store[i].name = LTSPFS_MTIME; i++;
  store[i].atom = None; store[i].name = LTSPFS_CTIME; i++;
  store[i].atom = None; store[i].name = NULL; i++;

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
LTSPFS_GetAtom (char *name)
{
  int i;

  for (i = 0; store[i].name != NULL; i++)
    {
      if (strcmp (store[i].name, name) == 0)
	{
	  return store[i].atom;
	}
    }

  return None;
}

Bool
LTSPFS_StoreStatBuf (struct stat *st)
{
  Atom tmp;

  if ((tmp = LTSPFS_GetAtom (LTSPFS_MODE)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (tmp, (long) st->st_mode) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (LTSPFS_NLINK)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (tmp, (long) st->st_nlink) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (LTSPFS_UID)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (tmp, (long) st->st_uid) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (LTSPFS_GID)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (tmp, (long) st->st_gid) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (LTSPFS_SIZE)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (tmp, (long) st->st_size) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (LTSPFS_ATIME)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (tmp, (long) st->st_atime) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (LTSPFS_MTIME)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (tmp, (long) st->st_mtime) != True)
    {
      return False;
    }

  if ((tmp = LTSPFS_GetAtom (LTSPFS_CTIME)) == None)
    {
      return False;
    }

  if (LTSPFS_PutLong (tmp, (long) st->st_ctime) != True)
    {
      return False;
    }
}

Bool
LTSPFS_GetStatBuf (struct stat *st)
{
  Atom tmp;
  long tmplong;

  if ((tmp = LTSPFS_GetAtom (LTSPFS_MODE)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (tmp, &tmplong) != True)
    {
      return False;
    }
  st->st_mode = (mode_t) tmplong;

  if ((tmp = LTSPFS_GetAtom (LTSPFS_NLINK)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (tmp, &tmplong) != True)
    {
      return False;
    }
  st->st_nlink = (nlink_t) tmplong;

  if ((tmp = LTSPFS_GetAtom (LTSPFS_UID)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (tmp, &tmplong) != True)
    {
      return False;
    }
  st->st_uid = (uid_t) tmplong;

  if ((tmp = LTSPFS_GetAtom (LTSPFS_GID)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (tmp, &tmplong) != True)
    {
      return False;
    }
  st->st_gid = (gid_t) tmplong;

  if ((tmp = LTSPFS_GetAtom (LTSPFS_SIZE)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (tmp, &tmplong) != True)
    {
      return False;
    }
  st->st_size = (off_t) tmplong;

  if ((tmp = LTSPFS_GetAtom (LTSPFS_ATIME)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (tmp, &tmplong) != True)
    {
      return False;
    }
  st->st_atime = (time_t) tmplong;

  if ((tmp = LTSPFS_GetAtom (LTSPFS_MTIME)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (tmp, &tmplong) != True)
    {
      return False;
    }
  st->st_mtime = (time_t) tmplong;

  if ((tmp = LTSPFS_GetAtom (LTSPFS_CTIME)) == None)
    {
      return False;
    }

  if (LTSPFS_GetLong (tmp, &tmplong) != True)
    {
      return False;
    }
  st->st_ctime = (time_t) tmplong;

  return True;
}
