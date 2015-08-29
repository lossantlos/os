#ifndef _FS_NODE_H
#define _FS_NODE_H

#include <stdint.h>

#define FS_FILE         0x01
#define FS_DIRECTORY    0x02
#define FS_CHARDEVICE   0x03
#define FS_BLOCKDEVICE  0x04
#define FS_PIPE         0x05
#define FS_SYMLINK      0x06
#define FS_MOUNTPOINT   0x08


struct dirent
{
    char name[128];
    uint32_t ino; //inode number
};

typedef struct
{
    
    unsigned int count;
} DIR;

typedef struct fs_node
{
    char        name[128];
    uint32_t    mask;
    uint32_t    uid;
    uint32_t    gid;
    uint32_t    flags;
    uint32_t    inode;
    uint32_t    lenght;
    uint32_t    (*read) (struct fs_node*, uint32_t, uint32_t, uint8_t*);
    uint32_t    (*write) (struct fs_node*, uint32_t, uint32_t, uint8_t*);
    void        (*open) (struct fs_node*);
    void        (*close) (struct fs_node*);
    struct dirent *(*readdir) (struct fs_node*, uint32_t);
    struct fs_node *(*finddir) (struct fs_node*, char *name);
    struct fs_node *ptr;
} fs_node_t;

#endif
