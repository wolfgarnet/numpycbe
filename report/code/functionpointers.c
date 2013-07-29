typedef struct
{
  void (*printstr)( char * );
  void (*printint)( unsigned int );
  void *(*malloc_align)(unsigned int, unsigned int);
} fpointers;
