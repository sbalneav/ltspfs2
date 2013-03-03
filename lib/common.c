Bool
LTSPFS_AtomWait (InfoStruct *is, char *atomname)
{
  XEvent ev;

  if ((tmp = LTSPFS_GetAtom (is, atomname)) == None)
    {
      return False;
    }

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
LTSPFS_GetLong (InfoStruct *is, char *atomname, long *result)
{
  long *property = NULL;
  Atom type, tmp;
  int format, res;
  unsigned long nitems, nleft;
  Bool retval;

  if ((tmp = LTSPFS_GetAtom (is, atomname)) == None)
    {
      return False;
    }

  res = XGetWindowProperty(is->dpy, is->win, tmp, 0, 1, 0, XA_INTEGER,
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
LTSPFS_GetString (InfoStruct *is, char *atomname, long len, char **result)
{
  char *property;
  Atom type;
  int format, res;
  unsigned long nitems, nleft;
  Bool retval;

  if ((tmp = LTSPFS_GetAtom (is, atomname)) == None)
    {
      return False;
    }

  res = XGetWindowProperty(is->dpy, is->win, tmp, 0, len, 0, XA_STRING,
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
LTSPFS_PutString (InfoStruct *is, Atom atom, (char *) data)
{
  if (XChangeProperty (is->dpy, is->win, atom, XA_STRING, 8,
		       PropModeReplace, (unsigned char *)data,
		       strlen (data) + 1) != Success)
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
LTSPFS_StoreStatBuf (InfoStruct *is, struct stat *st)
{
  Atom tmp;

  if (LTSPFS_PutLong (is, LTSPFS_MODE, st->st_mode) != True)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, LTSPFS_NLINK, st->st_nlink) != True)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, LTSPFS_UID, st->st_uid) != True)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, LTSPFS_GID, st->st_gid) != True)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, LTSPFS_SIZE, st->st_size) != True)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, LTSPFS_ATIME, st->st_atime) != True)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, LTSPFS_MTIME, st->st_mtime) != True)
    {
      return False;
    }

  if (LTSPFS_PutLong (is, LTSPFS_CTIME, st->st_ctime) != True)
    {
      return False;
    }
}

Bool
LTSPFS_GetStatBuf (InfoStruct *is, struct stat *st)
{
  Atom tmp;

  if (LTSPFS_GetLong (is, LTSPFS_MODE, st->st_mode) != True)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, LTSPFS_NLINK, st->st_nlink) != True)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, LTSPFS_UID, st->st_uid) != True)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, LTSPFS_GID, st->st_gid) != True)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, LTSPFS_SIZE, st->st_size) != True)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, LTSPFS_ATIME, st->st_atime) != True)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, LTSPFS_MTIME, st->st_mtime) != True)
    {
      return False;
    }

  if (LTSPFS_GetLong (is, LTSPFS_CTIME, st->st_ctime) != True)
    {
      return False;
    }
}
