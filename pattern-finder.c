#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

/*
Basic pattern finder for buffer overflow

*/


size_t pattern_search(char *str) // Takes the pattern that is on EIP and find how far the pattern is from original string
{
  int byte = 0;
  const char file_name[] = ".pattern.bin";
  struct stat st;
  stat(file_name, &st);
  char *pattern_string[st.st_size];
  FILE *fptr;
  fptr = fopen(file_name, "rb");
  if(fptr == NULL)
  {
  fprintf(stderr,"find_byte : No se ha podido abrir el archivo");
  }
  fread(pattern_string,st.st_size,1,fptr);
  fclose(fptr);
  char *str_remainder = strstr(pattern_string, str);
  size_t str_bytesize = strlen(pattern_string) - strlen(str_remainder);
  return str_bytesize;
}


char* pattern_create(size_t length) // Generates the pattern
{

    char* charset[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };
    char* charset2[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" };
    char* charset3[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
    char* pattern_string = (char*)malloc(((length + 1) * sizeof(char)) * 3);
    pattern_string[0] = '\0';
    int k = 0, j = 0, l = 0;


    for (int i = 0; i < length; i++)
    {
         strcat(pattern_string, charset[k]);
        strcat(pattern_string, charset2[j]);
        strcat(pattern_string, charset3[l]);
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

  // Writes pattern_string to file for further use

  FILE *fptr;
  fptr = fopen(".pattern.bin", "w");
  fprintf(fptr, pattern_string);
  fclose(fptr);
    return pattern_string;

}
int main(int argc, char* argv[])
{
  if(argc != 3){
  printf("Introduce un valor y un parametro:");
  exit(0);
  }

 if(strcmp(argv[1],"-f") == 0){
     printf("%s", pattern_create(strtol(argv[2],NULL, 10)));

  }

  if(strcmp(argv[1],"-l") == 0){
        printf("%lu",pattern_search(argv[2]));
  }
    return 0;
}
