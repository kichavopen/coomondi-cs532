#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *fp1, *fp2;
    char buffer[1024];
    size_t n;
    
    /* The correct number of arguments has to be checked */
    if (argc != 3) {
        printf("Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }
    
    /* The names of files are checked here */
    if (strcmp(argv[1], argv[2]) == 0) {
        printf("Error: filenames are the same\n");
        return 1;
    }
    
    /* File 1 is opened for appending */
    fp1 = fopen(argv[1], "a");
    if (fp1 == NULL) {
        perror("Error opening file1");
        return 1;
    }
    
    /* We open file 2 for reading */
    fp2 = fopen(argv[2], "r");
    if (fp2 == NULL) {
        perror("Error opening file2");
        fclose(fp1);
        return 1;
    }
    
    /* We copy contents of file 2 to file 1 */
    while ((n = fread(buffer, 1, sizeof(buffer), fp2)) > 0) {
        fwrite(buffer, 1, n, fp1);
    }
    
    fclose(fp1);
    fclose(fp2);
    
    return 0;
}
