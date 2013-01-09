#include <sys/stat.h>
#include <sys/mman.h> 
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


static void
check (int test, const char * message, ...)
{
    if (test) {
        va_list args;
        va_start (args, message);
        vfprintf (stderr, message, args);
        va_end (args);
        fprintf (stderr, "\n");
        exit (EXIT_FAILURE);
    }
}

char* regular_read(const char * file_name) 
{
  size_t size;
  char *buffer;
  FILE *fp;
  
  fp = fopen(file_name,"r");

  fseek (fp, 0, SEEK_END);
  size = ftell(fp);
  rewind (fp);
  
  buffer = (char*) malloc (sizeof(char)*size);
  fread (buffer, 1, size, fp);

  return buffer;
}


/* Does not work because returned string is not null terminated */
char* mmap_read(const char * file_name) 
{
  int fd;
  struct stat s;
  int status;
  size_t size;
  char * mapped;

  /* Open the file for reading. */
  fd = open (file_name, O_RDONLY);
  check (fd < 0, "open %s failed: %s", file_name, strerror (errno));
  
  /* Get the size of the file. */
  status = fstat (fd, & s);
  check (status < 0, "stat %s failed: %s", file_name, strerror (errno));
  size = s.st_size;    
  
  /* Memory-map the file. */
  mapped = mmap (0, size, PROT_READ, MAP_SHARED, fd, 0);
  check (mapped == MAP_FAILED, "mmap %s failed: %s", file_name, strerror (errno));
  
  /* Now do something with the information. */
  /*
  for (i = 0; i < size; i++) {
      putchar (mapped[i]);
  }
  putchar('\n');
  */  
  
  return mapped;
}

int main ()
{

  const char * file_name = "test.in";
  char * mapped;
  double value;  
  const char *delim = " ";
  char *token = NULL;  
  char *unconverted;
  int colCnt = 0;
  int rowCnt = 0;
  int i;
  double **matrix;

  mapped = regular_read(file_name);
  
  i = 0;
  while(mapped[i] != '\n'){
    if(mapped[i] == '.') {
     colCnt++;
    }
    i++;
  }
  printf("colCnt: %d\n", colCnt);    
  
  i = 0;
  while(i < strlen(mapped)){
    if(mapped[i] == '\n') {
     rowCnt++;
    }
    i++;
  } 
  printf("rowCnt: %d\n", rowCnt);    


  /* Malloc Matrix */
  if (( matrix = (double**)malloc(rowCnt * sizeof(double*))) == NULL ) {
    printf("malloc issue");
  }
  for(i = 0; i < rowCnt; i++) {
    if (( matrix[i] = (double*)malloc(colCnt * sizeof(double))) == NULL ) {
      printf("inside malloc issue");
    }
  }
    
    
  for (token = strtok(mapped, delim); token != NULL; token = strtok(NULL, delim))
  {
    value = strtod(token, &unconverted);
    //printf("%.2f\n", value);
  }

  return 0;
}