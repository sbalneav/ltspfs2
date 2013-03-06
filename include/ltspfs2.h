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

#define LTSPFS_OPCODE  "LTSPFS_OPCODE"
#define LTSPFS_RETVAL  "LTSPFS_RETVAL"
#define LTSPFS_DATA    "LTSPFS_DATA"
#define LTSPFS_PATH1   "LTSPFS_PATH1"
#define LTSPFS_PATH2   "LTSPFS_PATH2"
#define LTSPFS_DIRLIST "LTSPFS_DIRLIST"
#define LTSPFS_MODE    "LTSPFS_MODE"
#define LTSPFS_NLINK   "LTSPFS_NLINK"
#define LTSPFS_UID     "LTSPFS_UID"
#define LTSPFS_GID     "LTSPFS_GID"
#define LTSPFS_SIZE    "LTSPFS_SIZE"
#define LTSPFS_ATIME   "LTSPFS_ATIME"
#define LTSPFS_MTIME   "LTSPFS_MTIME"
#define LTSPFS_CTIME   "LTSPFS_CTIME"

extern Display *dpy;
extern Atom windowid;
extern Window window;
extern char *root;
extern AtomStore store[];
