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

/*
 * Global definitions
 */

Display *dpy;
Atom windowid;
Window window;
char *root;
AtomStore store[20];

/*
 * dispatch
 */

void
dispatch (void)
{
  long opcodeval;
  int retval;
  Bool result;
  Atom opcode;

  /*
   * Main loop.
   */

  if ((opcode = LTSPFS_GetAtom (LTSPFS_OPCODE)) == None)
    {
      return;
    }

  for (;;)
    {
      LTSPFS_AtomWait (opcode);

      if (LTSPFS_GetLong (opcode, &opcodeval) == False)
	{
	  return;
	}

      switch (opcodeval)
	{
	  case GETATTR:
	       result = LTSPFS_getattr (&retval);
	       break;
	  case READDIR:
	       result = LTSPFS_readdir (&retval);
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

      LTSPFS_PutLong ((long) retval);
    }
}

int
main (int argc, char **argv)
{
  int screen;

  dpy = XOpenDisplay (NULL);
  screen = DefaultScreen (dpy);
  root = argv[1];

  /*
   * Create our window we operate on
   */

  window = XCreateSimpleWindow (dpy, XRootWindow (dpy, screen),
			           1, 1, 1, 1, 1,
				   BlackPixel (dpy, 0), BlackPixel(dpy, 0));

  /*
   * Set the window name so client can find it.
   */

  XChangeProperty(dpy, window,
		  XInternAtom (dpy, "_NET_WM_NAME", False),
		  XA_STRING,
	          8, PropModeReplace, (unsigned char *) "LTSPFS2_WINDOW", 14);

  printf ("After _net_wm_name");
  LTSPFS_InitAtoms ();

  /*
   * Ready.  Dispatch based on opcode.
   */

  dispatch ();

  /*
   * We've returned from dispatch.  Exit.
   */

  exit (0);
}
