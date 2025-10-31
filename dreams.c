#include <Windows.h>
#include <stdio.h>

// So now that we have our data being inserted and out struct filled lets try and walk the entries


int diskSize = 20000;
int makeImg() {

    FILE* file = fopen("tired.img", "wb");

    BYTE* bytes = calloc(diskSize, sizeof(BYTE));
    //BYTE buff = 0x00;
    fwrite(bytes, diskSize, 1, file);

    printf("Created Img\n");
    fclose(file);
    return 0;

}

typedef struct {
    int offset; // starting offset
    char name[256];
    int nextEntry; // offset of next entry
    int entrynum;
} entry;

// 10 entries
entry* en;
int CreateStruct() {

    FILE* file = fopen("tired.img", "rb+");
    // 10 entries
    en = malloc(10 * sizeof(entry));

    fwrite(en, sizeof(entry), 10, file);

    fclose(file);

}
 
// leave 512 bytes for struct
int currentoffset = 0;
int entryCount = 0;
int writeImg(char* name, char* data, int offset, int size) {

    // FIX!
    if (en == NULL) {
        en = malloc(10 * sizeof(entry));

    }
    FILE* file = fopen("tired.img", "rb+"); // LOL

    fseek(file, offset, SEEK_SET);

    fwrite(data, size, 1, file);

    currentoffset = offset + size;
    printf("Next offset: %lu\n", currentoffset);

    fseek(file, 0, SEEK_SET);

    BYTE buff[512];
    fread(buff, 1, sizeof(entry), file);

    entryCount++;

    for (int i=0; i < 10; i++) {


        if (en[i].entrynum >= 10) {
                printf("Full\n");
                return 0;
        }
        // finding last entry
        if (en[i].nextEntry == 0) {


            strcpy(en[i].name, name);
            en[i].nextEntry = currentoffset;
            en[i].offset = offset;
            en[i].entrynum = entryCount;

            fseek(file, i * sizeof(entry), SEEK_SET);
            // I changed this off camera to write the whole struct lets see if it works
            fwrite(&en[i], sizeof(entry), 1, file);

            printf("Added struct entry\n");
            break;
        }
    }


    fclose(file);

}

// We will store entry count in the struct as well later
int readEntry(int getOffset) {

    FILE* file = fopen("tired.img", "rb");

    entry* out = malloc(10 * sizeof(entry)); 

    entryCount++;
    for (int i=0; i < 10; i++) {

    fseek(file, i * sizeof(entry), SEEK_SET);
    fread(out, 1, sizeof(entry), file);
    if (getOffset == 1) {
    // Last entry -1 actual last offset
    if (out->nextEntry == 0) {

        int final = i - 1;
        fseek(file, final * sizeof(entry), SEEK_SET);
        fread(out, 1, sizeof(entry), file);

        printf("Last Entry: %lu\n", out->nextEntry);

        return out->nextEntry;

    }
}

    if (getOffset == 0) {
    printf("name: %s\t Next: %lu\t Entry: %lu\t Offset: %lu\n", out->name, out->nextEntry, out->entrynum, out->offset);
    }


    }

    fclose(file);
}

int loadStruct() {

    FILE* file = fopen("tired.img", "rb");

    if (en == NULL) {
    en = malloc(10 * sizeof(entry)); 
    }

    //entryCount++;
    for (int i=0; i < 10; i++) {

    fseek(file, i * sizeof(entry), SEEK_SET);
    fread(&en[i],sizeof(entry), 1, file);

    if (en[i].offset == 0) break;

    entryCount++;
    printf("name: %s\t Next: %lu\t Entry: %lu\t Offset: %lu\n", en->name, en->nextEntry, en->entrynum, en->offset);

    }

}

int readDisk(char* name) {

    FILE* file = fopen("tired.img", "rb");

    en = malloc(10 * sizeof(entry)); 

    //entryCount++;
    for (int i=0; i < 10; i++) {

    fseek(file, i * sizeof(entry), SEEK_SET);
    fread(en, 1, sizeof(entry), file);

    if (en->offset == 0) break;

    if (strcmp(en->name, name) == 0) {
    //printf("name: %s\t Next: %lu\t Entry: %lu\t Offset: %lu\n", en->name, en->nextEntry, en->entrynum, en->offset);

    fseek(file, en->offset, SEEK_SET);

    int size = en->nextEntry - en->offset;
    char* data = malloc(size);
    fread(data, 1, size, file);


    data[size] = '\0';

    printf("%s\n", data);

    return 0;

    }


        //printf("name: %s\t Next: %lu\t Entry: %lu\t Offset: %lu\n", en->name, en->nextEntry, en->entrynum, en->offset);



    }


    return 1;
}

int main(int argc, char* argv[]) {

   if (argc < 2) {
    puts("Tired Format Engine:\n-create (Run to create disk)\t-list (list all entries)\n-write (Write a file)\t\t-read (read all entries)");
    return 0;
    }

   if (strcmp(argv[1], "-create") == 0) {
    makeImg();
    CreateStruct();
    writeImg("Tired", "Sleepy Format", 5000, 13);
    return 0;
   }

    if (strcmp(argv[1], "-list") == 0) {
        readEntry(0);
        return 0;
    }
    // Will add name as argv2 and data as a file to read in or a string like this
     if (strcmp(argv[1], "-write") == 0) {

        if (argc < 4) {
            puts("-write <Name> <Path2File>");
            return 0;
        }
        loadStruct();
        int num = readEntry(1) + 1;

        FILE* file = fopen(argv[3], "rb");

        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* buff = malloc(size);
        fread(buff, 1, size, file);

        writeImg(argv[2], buff, num, size);
        return 0;
     }

      if (strcmp(argv[1], "-read") == 0) {
        if (argc < 3) {
            puts("-read <Name>");
            return 0;
        }
        
        if (readDisk(argv[2]) == 1) {
            printf("Incorrect entry name run -list\n");
            return 0;
        }
      }

    return 0;
}
