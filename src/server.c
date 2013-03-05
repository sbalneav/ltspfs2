#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#include <X11/Xlib.h>

#include <ltspfs2.h>

void
dispatch (InfoStruct *is)
{
  long opcodeval;
  int retval;
  Bool result;
  Atom opcode;

  /*
   * Main loop.
   */

  if ((opcode = LTSPFS_GetAtom (is, LTSPFS_OPCODE)) == False)
    {
      return;
    }

  for (;;)
    {
      LTSPFS_AtomWait (is, opcode);

      if (LTSPFS_GetLong (is, opcode, &opcodeval) == False)
	{
	  return;
	}

      switch (opcodeval)
	{
	  case GETATTR:
	       result = LTSPFS_getattr (is, &retval);
	       break;
	  case READDIR:
	       result = LTSPFS_readdir (is, &retval);
	       break;
	  case READLINK:
	  case MKNOD:
	  case MKDIR:
	  case SYMLINK:
	  case UNLINK:
	  case RMDIR:
	  case RENAME:
	  case LINK:
	  case CHMOD:
	  case CHOWN:
	  case TRUNCATE:
	  case UTIME:
	  case OPEN:
	  case READ:
	  case WRITE:
	  case STATFS:
	  case RELEASE:
	  case RSYNC:
	  case SETXATTR:
	  case GETXATTR:
	  case LISTXATTR:
	  case REMOVEXATTR:
	  case XAUTH:
	  case MOUNT:
	  case PING:
	       break;
	  case QUIT:
	       return;
	}

      if (retval == False)
	{
	  /* Problem with LTSPFS itself.  Die. */
	  exit (1);
	}

      LTSPFS_PutLong (is, (long) retval);
    }
}

int
main (int argc, char **argv)
{
  InfoStruct is;
  int screen;
  AtomStore *store[] =
    {
      {None, LTSPFS_OPCODE},
      {None, LTSPFS_RETVAL},
      {None, LTSPFS_DATA},
      {None, LTSPFS_DIRLIST},
      {None, LTSPFS_MODE},
      {None, LTSPFS_NLINK},
      {None, LTSPFS_UID},
      {None, LTSPFS_GID},
      {None, LTSPFS_SIZE},
      {None, LTSPFS_ATIME},
      {None, LTSPFS_MTIME},
      {None, LTSPFS_CTIME},
      {None, NULL}
    };

  is.dpy = XOpenDisplay (NULL);
  screen = DefaultScreen (is.dpy);
  is.store = store;
  is.root = argv[1];

  /*
   * Create our window we operate on
   */

  is.window = XCreateSimpleWindow (is.dpy, XRootWindow (is.dpy, screen),
			           0, 0, 0, 0, 0, 0, 0);

  /*
   * Set the window name so client can find it.
   */

  XChangeProperty(is.dpy, is.window,
		  XInternAtom (is.dpy, "_NET_WM_NAME", False),
		  XA_STRING,
	          8, PropModeReplace, (unsigned char *) "LTSPFS2_WINDOW", 14);

  LTSPFS_InitAtoms (&is);

  /*
   * Ready.  Dispatch based on opcode.
   */

  dispatch (&is);

  /*
   * We've returned from dispatch.  Exit.
   */

  exit (0);
}
