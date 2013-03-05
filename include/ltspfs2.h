typedef enum LtspfsOp {
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
} LtspfsOp;

typedef struct atomstore {
    Atom atom;
    char *name;
} AtomStore;

char *LTSPFS_OPCODE  = "LTSPFS_OPCODE";
char *LTSPFS_RETVAL  = "LTSPFS_RETVAL";
char *LTSPFS_DATA    = "LTSPFS_DATA";
char *LTSPFS_PATH1   = "LTSPFS_PATH1";
char *LTSPFS_PATH2   = "LTSPFS_PATH2";
char *LTSPFS_DIRLIST = "LTSPFS_DIRLIST";
char *LTSPFS_MODE    = "LTSPFS_MODE";
char *LTSPFS_NLINK   = "LTSPFS_NLINK";
char *LTSPFS_UID     = "LTSPFS_UID";
char *LTSPFS_GID     = "LTSPFS_GID";
char *LTSPFS_SIZE    = "LTSPFS_SIZE";
char *LTSPFS_ATIME   = "LTSPFS_ATIME";
char *LTSPFS_MTIME   = "LTSPFS_MTIME";
char *LTSPFS_CTIME   = "LTSPFS_CTIME";

extern Display *dpy;
extern Atom windowid;
extern Window window;
extern char *root;
extern AtomStore store[];
