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

char *LTSPFS_OPCODE = "LTSPFS_OPCODE";
char *LTSPFS_RETVAL = "LTSPFS_RETVAL";
