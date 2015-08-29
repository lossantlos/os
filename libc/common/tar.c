
#include <tar.h>



#define TARTYPE_FILE     '0'   //regular file
#define TARTYPE_SFILE    '\0'  //regular file
#define TARTYPE_LINK     '1'   //link
#define TARTYPE_SYMTYPE  '2'   //reserved
#define TARTYPE_CHARDEV  '3'   //character special
#define TARTYPE_BLOCKDEV '4'   //block special
#define TARTYPE_DIR      '5'   //directory
#define TARTYPE_FIFO     '6'   //FIFO special
#define TARTYPE_CONTTYPE '7'   //reserved

//owner
#define TARUREAD   00400       //read
#define TARUWRITE  00200       //write
#define TARUEXEC   00100       //execute/search
//group
#define TARGREAD   00040       //read
#define TARGWRITE  00020       //write
#define TARGEXEC   00010       //execute/search
//other
#define TAROREAD   00004       //read
#define TAROWRITE  00002       //write
#define TAROEXEC   00001       //execute/search


uint32_t tar_data_read(const char *archive, uint32_t address, char *buf, uint32_t count);
/*
char *basename(const char *path)
{

}*/

unsigned int getsize(const char *in)
{
    unsigned int size = 0, count = 1;
    for (unsigned int j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);

    return size;
}

uint32_t tar_header_address(const char *path)
{
    #warning TODO write
    return 5;
}

uint32_t tar_file_size(const char *archive, const char *path)
{
    uint32_t address = tar_header_address(path);
    tar_header_t *a = (tar_header_t *)(archive + address * 512);
    if(!a->filename[0]) return 0;
    return getsize(a->size);
}

uint32_t tar_file_read(const char *archive, const char *path, char *buf, uint32_t count)
{
    uint32_t address = tar_header_address(path);

    tar_header_t *h = (tar_header_t *)(archive + address * 512);

    uint32_t i, size = getsize(h->size);

    printf("%i\n", size);

    if(count > size) count = size;
    return tar_read(archive, address+1, buf, count);
}

uint32_t tar_read(const char *archive, uint32_t address, char *buf, uint32_t count)
{
    char *a = (char *)(archive + (address * 512));

    uint32_t i;
    for(i = 0; i < count; i++) buf[i] = a[i];

    return i;
}


unsigned int tar_list_all(const char *archive)
{
    tar_header_t *a;
    a->filename[0]='0';
    unsigned int off = 0;
    while(1)
    {
        a = (tar_header_t *)(archive + off);
        if(!a->filename[0]) break;
        unsigned int size = getsize(a->size);
        printf("%s (%i)\n", a->filename, size);
        off += ((size / 512) + 1) * 512;
        if (size % 512) off += 512;
    }
}
