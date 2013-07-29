typedef struct
{
  unsigned int padding[2];//This is just to represent a common header-expansion.
  char *data;             /* pointer to raw data buffer */
  int nd;                 /* number of dimensions, also called ndim */
  unsigned int *dimensions;       /* size in each dimension */
  unsigned int *strides;          /* bytes to jump to get to the
                                   next element in each dimension */
  unsigned int *base;         /* This object should be decref'd
                                   upon deletion of array */
                                /* For views it points to the original array */
                                /* For creation from buffer object it points
                                   to an object that shold be decref'd on
                                   deletion */
                                /* For UPDATEIFCOPY flag this is an array
                                   to-be-updated upon deletion of this one */
  unsigned int *descr;   /* Pointer to type structure */
  int flags;              /* Flags describing array -- see below*/
  unsigned int *weakreflist;  /* For weakreferences */

  char **blockData;
  unsigned int numberOfBlocks;
  unsigned int numberOfBlocksXDim;
  unsigned int numberOfBlocksYDim;
  unsigned int blockSize; // Number of elements per row
  unsigned int size_in_bytes;
  unsigned int paddingx;
  unsigned int paddingy;
  SliceObject *slice_info;
  unsigned int padding2;
} PyArrayObject __attribute__((aligned (16)));