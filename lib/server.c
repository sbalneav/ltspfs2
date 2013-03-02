void
dispatch (Display dpy)
{
  Atom Opcode;
  Atom Retval;

  Opcode = XInternAtom (dpy, LTSPFS_OPCODE, False)
  Retval = XInternAtom (dpy, LTSPFS_RETVAL, False)

  if (Opcode == None)
    {
      fprintf (stderr, "Couldn't allocate atom for %s\n", LTSPFS_OPCODE);
      exit(1);
    }

  if (Retval == None)
    {
      fprintf (stderr, "Couldn't allocate atom for %s\n", LTSPFS_RETVAL);
      exit(1);
    }

  /*
   * Main loop.
   */

  LTSPFS_AtomWait (dpy, Opcode);

  OpcodeVal = (LtspfsOp) LTSPFS_GetLong (dpy, Opcode);

  switch (OpcodeVal)
    {
    GETATTR,
    READLINK,
    READDIR,
    MKNOD,
    MKDIR,
    SYMLINK,
    UNLINK,
    RMDIR,
    RENAME,
    LINK,
    CHMOD,
    CHOWN,
    TRUNCATE,
    UTIME,
    OPEN,
    READ,
    WRITE,
    STATFS,
    RELEASE,
    RSYNC,
    SETXATTR,
    GETXATTR,
    LISTXATTR,
    REMOVEXATTR,
    XAUTH,
    MOUNT,
    PING,
    QUIT 




