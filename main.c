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
  if(!fp) {
    fprintf (stderr, "file no good.\n");
    exit (EXIT_FAILURE);    
  }

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


void calc_matrix(int **A, int **B, int **C, lenAx, lenAy, lenBx, lenBy)
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
*/

/* Print matrix values */
void print_matrix(double **matrix, int row, int col) 
{

  int i, j;
  for(i = 0; i < row; i++) {
    printf("Row: %d\n",i);
    for(j = 0; j < col; j++) {
      printf("%.2f ",matrix[i][j]);
    }  
    putchar('\n');
  }
}
double ** read_matrix(int * rowCnt, int * colCnt, char * mapped)
{
  double value;  
  const char *delim_space = " ";
  char *token = NULL;  
  char *unconverted;
  int i, j;
  double **matrix;

  *colCnt = 0;
  *rowCnt = 0;

  
  /* Determine Col Count */
  i = 0;
  while(mapped[i] != '\n'){
    if(mapped[i] == '.') {
     (*colCnt)++;
    }
    i++;
  }  

  /* Determine Row Count */
  i = 0;
  while(i < strlen(mapped)){
    if(mapped[i] == '\n') {
     (*rowCnt)++;
    }
    i++;
  }
  (*rowCnt)++;


  /* Malloc the Matrix */
  if (( matrix = (double**)malloc((*rowCnt) * sizeof(double*))) == NULL ) {
    printf("malloc issue");
  }
  for(i = 0; i < (*rowCnt); i++) {
    if (( matrix[i] = (double*)malloc((*colCnt) * sizeof(double))) == NULL ) {
      printf("inside malloc issue");
    }
  }
    
  /* Read values into matrix */
  i = 0; j = 0;
  for (token = strtok(mapped, delim_space); token != NULL; token = strtok(NULL, delim_space)) {
    value = strtod(token, &unconverted);
    matrix[i][j] = value;
    j++;
    if(j == (*colCnt)) {
      j = 0;
      i++;
    }
  }
  return matrix;

}
int main ()
{

  const char * Afile = "test.in";
  const char * Bfile = "B.in";
  
  double ** Amatrix;
  double ** Bmatrix;

  int Arow, Acol, Brow, Bcol;
  int i;

  char * Amapped, * Bmapped;

  Amapped = regular_read(Afile);
  Bmapped = regular_read(Bfile);

  Amatrix = read_matrix(&Arow, &Acol, Amapped); 
  Bmatrix = read_matrix(&Brow, &Bcol, Bmapped);
  
  print_matrix(Amatrix, Arow, Acol);
  printf("A colCnt: %d\n", Acol);
  printf("A rowCnt: %d\n", Arow);

  print_matrix(Bmatrix, Brow, Bcol);
  printf("B colCnt: %d\n", Bcol);
  printf("B rowCnt: %d\n", Brow);


  /* Free Stuff */
  for(i = 0; i < Arow; i++) {
    free(Amatrix[i]);
  }
  free(Amatrix);
  free(Amapped);

  for(i = 0; i < Brow; i++) {
    free(Bmatrix[i]);
  }
  free(Bmatrix);
  free(Bmapped);
  return 0;

}
