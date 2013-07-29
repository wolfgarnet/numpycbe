typedef struct SliceObject
{
  unsigned int slice_type;//0:row, 1:col
  unsigned int slice_start;//start row/col
  unsigned int slice_end;//end row/col
  unsigned int padding;
} SliceObject __attribute__((aligned (16)));
