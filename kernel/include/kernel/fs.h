#ifndef _FS_NODE_H
#define _FS_NODE_H

#include <stdint.h>

typedef int32_t ssize_t;

/*
#define FS_FILE         0x01
#define FS_DIRECTORY    0x02
#define FS_CHARDEVICE   0x03
#define FS_BLOCKDEVICE  0x04
#define FS_PIPE         0x05
#define FS_SYMLINK      0x06
#define FS_MOUNTPOINT   0x08*/

typedef struct node_t {
	char *name;
	ssize_t (*write)(struct node_t*, const void*, ssize_t);
	ssize_t (*read)(struct node_t*, const void*, ssize_t);
} node_t;

#warning TODO remove

struct dirent {

};

extern node_t *fds[];
extern uint32_t nodes;

#endif
