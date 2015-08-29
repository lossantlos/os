
#include <kernel/fs.h>
#include <stdlib.h>

uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    return (node->read) ? node->read(node, offset, size, buffer) : 0;
}

uint32_t write_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    return (node->write) ? node->write(node, offset, size, buffer) : 0;
}

void open_fs(fs_node_t *node)
{
    if(node->open) node->open(node);
}

void close_fs(fs_node_t *node)
{
    if(node->close) node->close(node);
}

struct dirent *readdir_fs(fs_node_t *node, uint32_t num)
{
    if((node->flags & FS_DIRECTORY) == FS_DIRECTORY && node->readdir) return node->readdir(node, num);
    return NULL;
}

struct fs_node *finddir(fs_node_t *node, char *name)
{
    if((node->flags & FS_DIRECTORY) == FS_DIRECTORY && node->finddir) return node->finddir(node, name);
    return NULL;
}
