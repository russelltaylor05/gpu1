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

const char * mmap_read_example(const char * file_name) 
{

  int fd;
  struct stat s;
  int status;
  size_t size;
  const char * mapped;
  int i;


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
      char c;
      c = mapped[i];
      putchar (c);
  }
  putchar('\n');
  */
  
  return mapped;
}
void buildtable (int **A, int **B, int **C, lenAx, len Ay, len Bx, len By)
{//LenAx = LenBy
        int i, j, k, sum = 0;
        for(i = 0; i < lenBx; i++)
        {
                for(j = 0; j < lenAy; j++)
                {
                    for(k = 0; k < lenAx; k++)
                    {
                        sum+= A[k][j] * A[i][k];
                    }
                    C[i][j] = sum;
                    sum = 0;
                }
        }
}
int main ()
{

  const char * file_name = "test.in";
  const char * mapped;

  double value;  
  char *delim = " "; // input separated by spaces
  char *token = NULL;  
  char *unconverted;
  
  mapped = mmap_read_example(file_name);
  
  for (token = strtok(mapped, delim); token != NULL; token = strtok(NULL, delim))
  {
    value = strtod(token, &unconverted);
    printf("%f\n",value);
  }
  return 0;
}
