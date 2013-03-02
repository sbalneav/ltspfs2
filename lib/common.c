void
LTSPFS_AtomWait (Display *dpy, Atom atom)
{
  Window root = RootWindow (dpy, DefaultScreen (dpy));
  XEvent ev;

  /*
   * Declare our interest in obtaining PropertyChange events.
   */

  XSelectInput (dpy, root, PropertyChangeMask);

  for (;;)
    {
      if (XPending(dpy))
	{
          XNextEvent(dpy, &ev);
          if (ev.type == PropertyNotify)
	    {
              if (ev.xproperty.atom == atom)
		{
		  return;
		}
            }
	}
      else
        {
	  usleep(10000);
        }
    }
}

long
LTSPFS_GetLong (Display *dpy, Atom atom)
{
   long result;
   long *property;
   Atom type;
   int format, res;
   unsigned long nitems, nleft;
   Window root = RootWindow (dpy, DefaultScreen (dpy));


   res = XGetWindowProperty(dpy, root, atom, 0, 1, 0, XA_INTEGER,
                            &type, &format, &nitems, &nleft,
			    (unsigned char **) &property);

   if (type == XA_INTEGER)
     {
       result = *property;
     }

   XFree ((void *) property);

   return result;
}


