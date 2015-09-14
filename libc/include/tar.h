/**
@file tar.h
*/

#ifndef _TAR_H
#define _TAR_H

#include <stdint.h>

/** \callgraph lol
@{
*/
#define TARTYPE_FILE     '0'   ///<Regular file
#define TARTYPE_SFILE    '\0'  ///<Regular file
#define TARTYPE_LINK     '1'   ///<Link
#define TARTYPE_SYMTYPE  '2'   ///<Reserved
#define TARTYPE_CHARDEV  '3'   ///<Character device (special)
#define TARTYPE_BLOCKDEV '4'   ///<Block device (special)
#define TARTYPE_DIR      '5'   ///<Directory
#define TARTYPE_FIFO     '6'   ///<FIFO special
#define TARTYPE_CONTTYPE '7'   ///<Reserved
/**@}*/

///<Owner
#define TARUREAD   00400       ///<read
#define TARUWRITE  00200       ///<write
#define TARUEXEC   00100       ///<execute/search
///<Group
#define TARGREAD   00040       ///<read
#define TARGWRITE  00020       ///<write
#define TARGEXEC   00010       ///<execute/search
///<Other
#define TAROREAD   00004       ///<read
#define TAROWRITE  00002       ///<write
#define TAROEXEC   00001       ///<execute/search


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
