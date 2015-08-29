
#ifndef _TAR_H
#define _TAR_H

#include <stdint.h>

typedef struct tar_header
{
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
    //less important
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char atime[12];
    char ctime[12];
} tar_header_t;

unsigned int getsize(const char *in);

uint32_t tar_read(const char *archive, uint32_t address, char *buf, uint32_t count);
uint32_t tar_file_read(const char *archive, const char *path, char *buf, uint32_t count);

#endif
