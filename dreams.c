#include <stdlib.h>
#include <stdio.h>

// So now that we have our data being inserted and out struct filled lets try and walk the entries


typedef unsigned char BYTE;

int diskSize = 20000;
int makeImg() {

    FILE* file = fopen("tired.img", "wb");

    BYTE* bytes = calloc(diskSize, 1);
    //BYTE buff = 0x00;
    fwrite(bytes, diskSize, 1, file);

    printf("Created Img\n");
    fclose(file);
    return 0;

}

typedef struct {
    int offset; // starting offset
    char name[50];
    int nextEntry; // offset of next entry
    int entrynum;
    int deleted;
} entry;

// 10 entries


typedef struct {
    entry* en;
    char reserved[28];
    int num;
} Tired;

Tired* tired;
int CreateStruct() {

    FILE* file = fopen("tired.img", "rb+");
    // 10 entries
    tired = malloc(1 * sizeof(Tired));
    tired->en = malloc(10 * sizeof(entry));

    fwrite(tired, sizeof(Tired), 1, file);

    fclose(file);

}

int delete(char* name) {
    FILE* file = fopen("tired.img", "rb+");

    tired = malloc(sizeof(Tired));
    entry* en = malloc(sizeof(entry));

    int b = 10;
    for (int i=0; i < b; i++) {

        fseek(file, i * sizeof(entry), SEEK_SET);

        fread(en, 1, sizeof(entry), file);

       // printf("offset: %lu\n", en->offset);

        if (strcmp(en->name, name) == 0) {
            en->deleted = 1;

            fseek(file, i * sizeof(entry), SEEK_SET);
            fwrite(en, sizeof(entry), 1, file);

            printf("[%s] Deleted: %lu\n", name, en->deleted);
        }


        //b++;
    }
    
}
 
// leave 512 bytes for struct
int currentoffset = 0;
int entryCount = 0;
int writeImg(char* name, char* data, int offset, int size) {

    // FIX!
    if (tired->en == NULL) {
        tired = malloc(1 * sizeof(Tired));
        tired->en = malloc(10 * sizeof(entry));

    }
    FILE* file = fopen("tired.img", "rb+"); // LOL

    fseek(file, offset, SEEK_SET);

    fwrite(data, size, 1, file);

    currentoffset = offset + size;
    printf("Next offset: %lu\n", currentoffset);

    fseek(file, 0, SEEK_SET);

    entryCount++;

    int b = 1;
    for (int i=0; i < b; i++) {


        if (tired->en[i].entrynum >= 10) {
                printf("Full\n");
                return 0;
        }
        // finding last entry
        if (tired->en[i].nextEntry == 0) {


            strcpy(tired->en[i].name, name);
            tired->en[i].nextEntry = currentoffset;
            tired->en[i].offset = offset;
            tired->en[i].entrynum = entryCount;
            tired->en[i].deleted = 0;
            tired->num = entryCount;
            fseek(file, i * sizeof(entry), SEEK_SET);
            // I changed this off camera to write the whole struct lets see if it works
            fwrite(&tired->en[i], sizeof(entry), 1, file);


            // offset of is ((size_t)&(((Tired*)0)->num))
            // Writing num
            fseek(file, ((size_t)&(((Tired*)0)->num)), SEEK_SET);
            fwrite(&tired->num, sizeof(int), 1, file);
            
            printf("Added struct entry\n");
            break;
        }

        // Keeping b in front of i
        b++;
    }


    fclose(file);

}

int readnum() {

    tired = malloc(sizeof(Tired));
    FILE* file = fopen("tired.img", "rb");

    fseek(file, 0, SEEK_SET);

    fread(tired, 1, sizeof(Tired), file);

    fclose(file);
    return tired->num;
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

    if (out->offset == 0) break;
    
    if (out->deleted != 1) {
    printf("name: %s\t Next: %lu\t Entry: %lu\t Offset: %lu\n", out->name, out->nextEntry, out->entrynum, out->offset);
    }
    }


    }

    fclose(file);
}

int loadStruct() {

    FILE* file = fopen("tired.img", "rb");

    tired = malloc(1 * sizeof(Tired));
    tired->en = malloc(10 * sizeof(entry)); 
    
    //entryCount++;
    for (int i=0; i < 10; i++) {

        
    fseek(file, i * sizeof(entry), SEEK_SET);
    fread(&tired->en[i],sizeof(entry), 1, file);

    if (tired->en[i].offset == 0) break;

    entryCount++;

    if (tired->en->deleted != 1) {
    printf("name: %s\t Next: %lu\t Entry: %lu\t Offset: %lu\n", tired->en->name, tired->en->nextEntry, tired->en->entrynum, tired->en->offset);
    }

    }

}

int readDisk(char* name) {

    FILE* file = fopen("tired.img", "rb");

    tired = malloc(1 * sizeof(Tired));
    tired->en = malloc(10 * sizeof(entry)); 

    //entryCount++;
    for (int i=0; i < 10; i++) {

    fseek(file, i * sizeof(entry), SEEK_SET);
    fread(tired->en, 1, sizeof(entry), file);

    if (tired->en->offset == 0) break;

    if (strcmp(tired->en->name, name) == 0) {
    //printf("name: %s\t Next: %lu\t Entry: %lu\t Offset: %lu\n", en->name, en->nextEntry, en->entrynum, en->offset);

    fseek(file, tired->en->offset, SEEK_SET);

    int size = tired->en->nextEntry - tired->en->offset;
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

int writeRecycleBin(char* fileName) {
    FILE* file = fopen("tired.img", "rb+");

    fseek(file, 5000, SEEK_SET);

    tired = malloc(sizeof(Tired));
    tired->en = malloc(10 * sizeof(entry));

    int dothis = 0;
    char* data2Save = NULL;

    for (int i=0; i < 10; i++) {

        fseek(file, i * sizeof(entry), SEEK_SET);
        fread(tired->en, 1, sizeof(entry), file);

            printf("name: %s\n", tired->en->name);
            if (strcmp(tired->en->name, "Recycle") == 0 && dothis == 1) {
                
                    fseek(file, tired->en->offset, 0);

                    int size = tired->en->nextEntry - tired->en->offset;
                    char* data = malloc(size);
                    fread(data, 1, size, file);

                    data[size] = '\0';

                    printf("%s\n", data);

                    fseek(file, tired->en->offset - 3000, 0);

                    fwrite(data2Save, strlen(data2Save), 1, file);
                    // printf("hello\n");

                    // loadStruct();
                    // writeImg("bub", "hello", tired->en->offset + size + 1, 5);

                    return 0;
            }

            if (strcmp(tired->en->name, fileName) == 0) {

                    fseek(file, tired->en->offset, 0);

                    int size = tired->en->nextEntry - tired->en->offset;
                    char* data = malloc(size);
                    fread(data, 1, size, file);

                    data[size] = '\0';

                    //printf("%s\n", data);

                    data2Save = malloc(size);

                    for (int j=0; j < size; j++) {
                        data2Save[j] = data[j];
                    }

                    fseek(file, tired->en->offset, 0);

                    memset(data, 0x00, size);

                    fwrite(data, size, 1, file);

                    // restart to find recycle bin again
                    i = 0;
                    dothis = 1;

            }

        Sleep(300);
    }
}

int encryptFile(char* fileName, BYTE encryptWith) {
    FILE* file = fopen("tired.img", "rb+");

    fseek(file, 5000, SEEK_SET);

    tired = malloc(sizeof(Tired));
    tired->en = malloc(10 * sizeof(entry));

    int dothis = 0;
    char* data2Save = NULL;

    for (int i=0; i < 10; i++) {

        fseek(file, i * sizeof(entry), SEEK_SET);
        fread(tired->en, 1, sizeof(entry), file);

        if (strcmp(tired->en->name, fileName) == 0) {
            fseek(file, tired->en->offset, SEEK_SET);
            int size = tired->en->nextEntry - tired->en->offset;

            BYTE* data = malloc(size);
            fread(data, 1, size, file);

            for (int i=0; i < size; i++) {
                data[i] ^= encryptWith;
            }

            if (data) {
                fseek(file, tired->en->offset, SEEK_SET);
                fwrite(data, size, 1, file);
                puts("Encrypted data\n");
            }
        }

    }
}

int rename(char* fileName, char* newName) {
        
    FILE* file = fopen("tired.img", "rb+");

    fseek(file, 5000, SEEK_SET);

    tired = malloc(sizeof(Tired));
    tired->en = malloc(10 * sizeof(entry));

    int dothis = 0;
    char* data2Save = NULL;

    for (int i=0; i < 10; i++) {

        fseek(file, i * sizeof(entry), SEEK_SET);
        fread(tired->en, 1, sizeof(entry), file);

        if (strcmp(tired->en->name, fileName) == 0) {
            strcpy(tired->en->name, newName);
            fseek(file, i * sizeof(entry) + offsetof(entry, name), SEEK_SET);
            fwrite(tired->en->name, 1, strlen(tired->en->name), file);

        }
}

fclose(file);
return 0;
}


int main(int argc, char* argv[]) {

   if (argc < 2) {
    puts("Tired Format Engine:\n-create (Run to create disk)\t-list (list all entries)\n-write (Write a file)\t\t-read (read all entries)");
    return 0;
    }

   if (strcmp(argv[1], "-create") == 0) {
    makeImg();
    CreateStruct();
    writeImg("Tired", "Sleepy Format", 6000, 13);

    BYTE* alloc = calloc(1000, sizeof(BYTE));
    writeImg("Recycle", alloc, 5000, 3);
    return 0;
   }

    if (strcmp(argv[1], "-list") == 0) {
        readEntry(0);
        int num = readnum();
        printf("Number of Entries: %lu\n", num);
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

      if (strcmp(argv[1], "-delete") == 0) {
        delete(argv[2]);
        writeRecycleBin(argv[2]);
      }

      if (strcmp(argv[1], "-encrypt") == 0) {
        loadStruct();
        encryptFile(argv[2], (BYTE)argv[2][1]);
      }

      if (strcmp(argv[1], "-rename") == 0) {
        loadStruct();
        rename(argv[2], argv[3]);
        puts("renamed");
      }

    return 0;
}
