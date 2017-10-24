
#include <tar.h>
#include <string.h>




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

uint32_t tar_header_address(const char *archive, const char *path)
{
    tar_header_t *a;
    a->filename[0]='0';
    unsigned int off = 0;
    while(1)
    {
        a = (tar_header_t *)(archive + off);
        if(!a->filename[0]) break;
        if(!strcmp(path, a->filename)) return off / 512;
        unsigned int size = getsize(a->size);
        off += ((size / 512) + 1) * 512;
        if (size % 512) off += 512;
    }
    return 0;
}

uint32_t tar_file_size(const char *archive, const char *path)
{
    uint32_t address = tar_header_address(archive, path);
    tar_header_t *a = (tar_header_t *)(archive + address * 512);
    if(!a->filename[0]) return 0;
    return getsize(a->size);
}

uint32_t tar_file_read(const char *archive, const char *path, char *buf, uint32_t count)
{
    uint32_t address = tar_header_address(archive, path);

    tar_header_t *h = (tar_header_t *)(archive + address * 512);

    uint32_t i, size = getsize(h->size);

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
