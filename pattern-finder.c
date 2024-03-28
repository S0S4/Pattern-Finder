#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

/*
Basic pattern finder for buffer overflow
*/

/*
* Takes the pattern that is on EIP and find how far the pattern is from original string
*/
size_t pattern_search(const char *str) {
  const char file_name[] = ".pattern.bin";
  struct stat st;
  if (stat(file_name, &st) != 0) {
    fprintf(stderr, "Error al obtener información del archivo\n");
    return 0;
  }
  char *pattern_string = (char *)malloc(st.st_size + 1); // Allocate memory for file content plus null terminator
  if (pattern_string == NULL) {
    fprintf(stderr, "Memoria insuficiente\n");
    return 0;
  }
  FILE *fptr = fopen(file_name, "rb");
  if (fptr == NULL) {
    fprintf(stderr, "No se ha podido abrir el archivo\n");
    free(pattern_string);
    return 0;
  }
  if (fread(pattern_string, 1, st.st_size, fptr) != st.st_size) {
    fprintf(stderr, "Error leyendo el archivo\n");
    fclose(fptr);
    free(pattern_string);
    return 0;
  }
  pattern_string[st.st_size] = '\0'; // Null-terminate the string
  fclose(fptr);

  char *str_remainder = strstr(pattern_string, str);
  if (str_remainder == NULL) {
    free(pattern_string);
    return 0; // Pattern not found
  }
  size_t str_bytesize = str_remainder - pattern_string;
  free(pattern_string);
  return str_bytesize;
}

/*
* Generate pattern
*/
char* pattern_create(size_t length) {
  char* charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char* charset2 = "abcdefghijklmnopqrstuvwxyz";
  char* charset3 = "0123456789";
  char* pattern_string = (char*)malloc(length * 3 + 1);
  if (pattern_string == NULL) {
    fprintf(stderr, "Not enough memory\n");
    return NULL;
  }
  pattern_string[0] = '\0';
  size_t k = 0, j = 0, l = 0;

  for (size_t i = 0; i < length; ++i) {
    size_t index = i * 3;
    pattern_string[index] = charset[k];
    pattern_string[index + 1] = charset2[j];
    pattern_string[index + 2] = charset3[l];
    pattern_string[index + 3] = '\0'; // Ensure string is always null-terminated

    l++;
    if (l == 10) {
      j++;
      l = 0;
    }
    if (j == 26) {
      k++;
      j = 0;
    }
  }

  FILE *fptr = fopen(".pattern.bin", "wb");
  if (fptr == NULL) {
    fprintf(stderr, "Unable to open file\n");
    free(pattern_string);
    return NULL;
  }
  fwrite(pattern_string, 1, length * 3, fptr);
  fclose(fptr);

  return pattern_string;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Use: [opcion] [valor]\n");
    exit(0);
  }

  if (strcmp(argv[1], "-f") == 0) {
    char* pattern = pattern_create(strtol(argv[2], NULL, 10));
    if (pattern != NULL) {
      printf("%s", pattern);
      free(pattern);
    }
  } else if (strcmp(argv[1], "-l") == 0) {
    printf("%lu", pattern_search(argv[2]));
  }

  return 0;
}

