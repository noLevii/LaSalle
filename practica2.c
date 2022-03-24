#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <errno.h>
#include <unistd.h>
#include <dirent.h>

#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024

const char *symbolic = { "symbolic" };
const char *hard = { "hard" };

const char *our_names = { "Sebastian Azamar Aviles.\nKarime Aylen de la Cruz Vargas.\nCarolina Gonzalez Gonzalez.\nMartin Granados Reyes." };

void list_file_attributes(const char *, bool);
int check_by_created_diectory(const char *);
void list_directory(const char *);
unsigned int create_name_file(const char *);
void create_symbolic_link(const char *, const char *, const char *);
void create_hard_link(const char *, const char *, const char *);
void create_directory(const char *, const char *);

int main(int argc, const char **argv)
{
    char new_dirname[BUFFER_SIZE] = { 0 };

    if(argc > 2) {
        const char *dirname = argv[1];
        const char *filename = argv[2];

        if(!check_by_created_diectory(dirname)) {
            printf("El directorio %s ya existe.\n", dirname);

            printf("\nDame un nuevo nombre para el directorio: ");
            scanf("%s", new_dirname);
            dirname = new_dirname;

            create_directory(dirname, filename);
        } else {
            create_directory(dirname, filename);
        }
    } else {
        printf("Se requiere el nombre del directorio y el nombre del archivo\n\n");
    }

    return 0;
}

void list_file_attributes(const char *filename, bool all) {
    // atributos del archivo de nombres
    struct stat filestat = { 0 };
    stat (filename, &filestat);
    if(all) {
        printf("File attributes: %s\n", filename);
        printf("Inode number: %ld\n", filestat.st_ino);
        printf("Total size, in bytes: %ld\n", filestat.st_size);
        printf("Number of links: %ld\n", filestat.st_nlink);
    } else {
        printf("File attributes: %s\n", filename);
        printf("Inode number: %ld\n", filestat.st_ino);
    }
}

int check_by_created_diectory(const char *dirname) {
    struct stat dirstats = { 0 };
    return stat(dirname, &dirstats);
}

void list_directory(const char *dirname) {
    // listar directorio actual
    DIR *dp = opendir(".");

    if(dp) {
        struct dirent *ep = NULL;

        while((ep = readdir(dp)) != NULL) {
            // if(strcmp(ep->d_name, ".") && strcmp(ep->d_name, "..")) {
                printf("---Archivo: %s\n", ep->d_name);
                list_file_attributes(ep->d_name, false);
            // }
        }

        closedir(dp);
    }
}

unsigned int create_name_file(const char *filename) {
    unsigned int retval = 1;
    FILE *names = fopen(filename, "w+");

    if(names) {
        printf("Se creo el archivo %s.\n", filename);
        size_t wrote = fwrite(our_names, strlen(our_names), 1, names);

        if(wrote) {
            printf("Se escribieron %lu elementos en %s.\n\n", wrote, filename);
            retval = 0;
        }

        fclose(names);
    } else {
        printf("Error %d al crear el archivo %s.\n\n", errno, filename);
    }

    return retval;
}

void create_symbolic_link(const char *dirname, const char *single_filename, const char *filename) {
    // crear symbolic link
    char buffer[BUFFER_SIZE] = { 0 };
    printf("Creando link simbolico a %s...\n", filename);
    snprintf(buffer, BUFFER_SIZE - 1, "%s-%s-%s", symbolic, dirname, single_filename);

    if(!symlink (filename, buffer)) {
        printf("Link simbolico %s a %s creado.\n", buffer, filename);
    } else {
        printf("Error %d al crear link simbolico %s.\n", errno, buffer);
    }
}

void create_hard_link(const char *dirname, const char *single_filename, const char *filename) {
    // crear hard link
    char buffer[BUFFER_SIZE] = { 0 };
    printf("Creando hard link a %s...\n", filename);
    snprintf(buffer, BUFFER_SIZE - 1, "%s-%s-%s", hard, dirname, single_filename);

    if(!link (filename, buffer)) {
        printf("Hard link %s a %s creado.\n", buffer, filename);
    } else {
        printf("Error %d al crear hard link %s.\n");
    }
}

void create_directory(const char *dirname, const char *filename) {
    char buffer[BUFFER_SIZE] = { 0 };

    if(!mkdir(dirname, 0777)) {
        snprintf(buffer, BUFFER_SIZE - 1, "%s/%s", dirname, filename);

        if(!create_name_file(buffer)) {
            create_symbolic_link(dirname, filename, buffer);
            create_hard_link(dirname, filename, buffer);
            printf("\n");
            list_file_attributes(buffer, true);
            printf("\n");
            list_directory(dirname);
        }
    } else {
        printf("Error %d al crear el directorio %s.\n\n", errno, dirname);
    }
}
