typedef struct {
    int two;              /* contains the integer 2 as a sanity check */
    int nd;               /* number of dimensions */
    char typekind;        /* kind in array --- character code of typestr */
    int itemsize;         /* size of each element */
    int flags;            /* how should be data interpreted. Valid
                             flags are CONTIGUOUS (1), FORTRAN (2),
                             ALIGNED (0x100), NOTSWAPPED (0x200), and
                             WRITEABLE (0x400).
                             ARR_HAS_DESCR (0x800) states that arrdescr
                                field is present in structure */
    npy_intp *shape;          /* A length-nd array of shape information */
    npy_intp *strides;        /* A length-nd array of stride information */
    void *data;           /* A pointer to the first element of the array */
    PyObject *descr;      /* A list of fields or NULL (ignored if flags
                                does not have ARR_HAS_DESCR flag set) */
} PyArrayInterface;
