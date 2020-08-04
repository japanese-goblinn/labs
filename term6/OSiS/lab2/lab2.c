// Вариант 1

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <locale.h>

typedef int bool;
#define true 1
#define false 0

struct boot_block {
    unsigned char bootstrap[3];
    unsigned char manufacturer[8];
    unsigned short n_bytes_per_block;
    unsigned char n_blocks_per_alloc;
    unsigned short n_reserved_block;
    unsigned char n_fat;
    unsigned short n_root_dirs;
    unsigned short n_blocks_on_disk;
    unsigned char meta_desc;
    unsigned short n_blocks_on_fat;
    unsigned short n_blocks_per_track;
    unsigned short n_heads;
    unsigned int n_hidden_block;
    unsigned int n_blocks_on_disk_true;
    unsigned short physical_drive_nb;
    unsigned char signature;
    unsigned int serial_num;
    unsigned char label[11];
    unsigned char fs_id[8];
};

struct root_dir_entry {
    unsigned char filename[10];
    unsigned char extension[3];
    unsigned char attr;
    unsigned char reserved[10];
    unsigned short time_last_updated;
    unsigned short date_last_updated;
    unsigned short starting_cluster;
    unsigned int file_size;
};

unsigned char* trim_f(unsigned char *f) {
    int i = 7;
    while(i >= 0 && f[i] == ' ') {
        i--;
    }
    
    f[i+1] = '\0';
    return f;
}

unsigned char* trim_e(unsigned char *e) {
    int i = 2;
    while(i >= 0 && e[i] == ' ') {
        i--;
    }
    
    e[i+1] = '\0';
    return e;
}

bool is_valid_entry(struct root_dir_entry r) 
{
    unsigned char sign = r.filename[0];

    if (r.starting_cluster == 0) 
        return false;
    if (sign == 0x00 || sign == 0x20) 
        return false;
    if (sign == 0xE5 || sign == 0x05)
         return false;
    if ((r.attr & 0x08) == 0x08) 
        return false;
    return true;
}

void read_boot_block(struct boot_block *bb, FILE **f)
{
    fseek(*f, 0, SEEK_SET);
    fread(bb->bootstrap, sizeof(unsigned char), 3, *f);
    fread(bb->manufacturer, sizeof(unsigned char), 8, *f);
    fread(&bb->n_bytes_per_block, sizeof(unsigned char), 2, *f);
    fread(&bb->n_blocks_per_alloc, sizeof(unsigned char), 1, *f);
    fread(&bb->n_reserved_block, sizeof(unsigned char), 2, *f);
    fread(&bb->n_fat, sizeof(unsigned char), 1, *f);
    fread(&bb->n_root_dirs, sizeof(unsigned char), 2, *f);
    fread(&bb->n_blocks_on_disk, sizeof(unsigned char), 2, *f);
    fread(&bb->meta_desc, sizeof(unsigned char), 1, *f);
    fread(&bb->n_blocks_on_fat, sizeof(unsigned char), 2, *f);
    fread(&bb->n_blocks_per_track, sizeof(unsigned char), 2, *f);
    fread(&bb->n_heads, sizeof(unsigned char), 2, *f);
    fread(&bb->n_hidden_block, sizeof(unsigned char), 4, *f);
    fread(&bb->n_blocks_on_disk_true, sizeof(unsigned char), 4, *f);
    fread(&bb->physical_drive_nb, sizeof(unsigned char), 2, *f);
    fread(&bb->signature, sizeof(unsigned char), 1, *f);
    fread(&bb->serial_num, sizeof(unsigned char), 4, *f);
    fread(&bb->label, sizeof(unsigned char), 11, *f);
    fread(&bb->fs_id, sizeof(unsigned char), 8, *f);
}

void read_root_directories(struct root_dir_entry *r, struct boot_block bb, FILE **f)
{
    long pos;
    unsigned int i;
    int root_dir_addr;
    unsigned char bb_sign[2];
    for (i = 0; i < bb.n_root_dirs; i++) {
        root_dir_addr = (bb.n_reserved_block * bb.n_bytes_per_block) 
                        + (bb.n_fat * bb.n_blocks_on_fat * bb.n_bytes_per_block);
        pos = root_dir_addr + (i * 32);
        fseek(*f, pos, SEEK_SET);
        fread(r[i].filename, sizeof(unsigned char), 8, *f);
        fread(r[i].extension, sizeof(unsigned char), 3, *f);
        fread(&r[i].attr, sizeof(unsigned char), 1, *f);
        fread(r[i].reserved, sizeof(unsigned char), 10, *f);
        fread(&r[i].time_last_updated, sizeof(unsigned char), 2, *f);
        fread(&r[i].date_last_updated, sizeof(unsigned char), 2, *f);
        fread(&r[i].starting_cluster, sizeof(unsigned char), 2, *f);
        fread(&r[i].file_size, sizeof(unsigned char), 4, *f);
    }
}

unsigned short get_next_block(unsigned short block, struct boot_block bb, FILE **f)
{
    long pos;
    unsigned short next_block;
    pos = bb.n_bytes_per_block + (long)(block)*1.5;
    fseek(*f, pos, SEEK_SET);
    fread(&next_block, sizeof(unsigned char), 2, *f);
    if (block % 2 == 0) {
        next_block &= 0x000FFF;
    } else {
        next_block >>= 4; 
        next_block &= 0x000FFF;
    }
    return next_block;
}

char* get_file_content(unsigned short block, struct boot_block bb, FILE **f)
{
    long pos;
    int root_dir_addr;
    char *buffer = calloc(1, bb.n_bytes_per_block * bb.n_blocks_per_alloc);

    root_dir_addr = (bb.n_reserved_block * bb.n_bytes_per_block) 
                    + (bb.n_fat * bb.n_blocks_on_fat * bb.n_bytes_per_block);
    pos = root_dir_addr
        + (bb.n_root_dirs * 32)
        + (block - 2) * (bb.n_bytes_per_block * bb.n_blocks_per_alloc);

    fseek(*f, pos, SEEK_SET);
    fread(buffer, sizeof(char), bb.n_bytes_per_block * bb.n_blocks_per_alloc, *f);

    return buffer;
}

void write_file_content(struct root_dir_entry r, struct boot_block bb, FILE **f)
{
    unsigned short next_block;
    unsigned char* f_content;
    char* buffer;

    int content_length = bb.n_bytes_per_block * bb.n_blocks_per_alloc;
    char* fullname = (char*)malloc(13 * sizeof(char));

    strcpy(fullname, trim_f(r.filename));
    if (*trim_e(r.extension) != '\0') {
        strcat(fullname, ".");
        strcat(fullname, trim_e(r.extension));
    }
    printf("%s\n", fullname);
    
    FILE* extr_file = fopen(fullname, "w");
    
    next_block = r.starting_cluster;
    while (next_block < 0xFF8) {
        buffer = get_file_content(next_block, bb, f);
        next_block = get_next_block(next_block, bb, f);
        if (next_block >= 0xFF8) 
            content_length = r.file_size % content_length;
        fwrite(buffer, sizeof(char), content_length, extr_file);
        free(buffer);
    }
    if (extr_file)
        fclose(extr_file);
}

int main(int argc, char** argv)
{
    struct boot_block bb;
    FILE *f;
    long pos;
    unsigned char bb_sign[2];
    struct root_dir_entry *r_entries;
    if (argc < 2) {
        return 1;
    }
    f = fopen(argv[1], "r");
    if (f == NULL) {
        perror(NULL);
        return 1;
    }
    memset(&bb, 0, sizeof(struct boot_block));
    read_boot_block(&bb, &f);
    r_entries = calloc(sizeof(struct root_dir_entry), bb.n_root_dirs);
    read_root_directories(r_entries, bb, &f);
    for (pos = 0; pos < bb.n_root_dirs; pos++) {
        if (is_valid_entry(r_entries[pos])) 
            write_file_content(r_entries[pos], bb, &f);
    }
    return 0;
}