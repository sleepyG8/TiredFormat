#include <Windows.h>
#include <stdio.h>

typedef struct {
    int offset; // starting offset
    char name[256];
    int nextEntry; // offset of next entry
    int entrynum;
} entry;

// 10 entries
entry* en;

int entryCount = 0;
// We will store entry count in the struct as well later
int readEntry(int getOffset, char* name) {

    FILE* file = fopen("tired.img", "rb");
    if (!file) return 0;

    entry* out = malloc(10 * sizeof(entry)); 

    entryCount++;
    for (int i=0; i < 256; i++) {

    if (out->offset == 0) break;

    fseek(file, i * sizeof(entry), SEEK_SET);

    fread(out, 1, sizeof(entry), file);

    if (getOffset == 0) {
    printf("name: %s\t Next: %lu\t Entry: %lu\t Offset: %lu\n", out->name, out->nextEntry, out->entrynum, out->offset);
    }

    if (name != NULL && strcmp(out->name, name) == 0) {
        return out->offset;
    }

    }

    fclose(file);
    return 0;

}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        puts("[Img Name] [File]");
        return 0;
    }

    int zero = readEntry(0, argv[2]);

    if (argv[2] != NULL) {

    FILE* file = fopen(argv[1], "rb");

    fseek(file, zero, SEEK_SET);

    BYTE* buffer[2000];
    fread(buffer, 1, sizeof(buffer), file);

    printf("%s\n", (char*)buffer);

    }

    return 0;
    

}
