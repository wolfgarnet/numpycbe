typedef struct _PyArray_Descr {
        PyObject_HEAD
        PyTypeObject *typeobj;  /* the type object representing an
                                   instance of this type -- should not
                                   be two type_numbers with the same type
                                   object. */
        char kind;              /* kind for this type */
        char type;              /* unique-character representing this type */
        char byteorder;         /* '>' (big), '<' (little), '|'
                                   (not-applicable), or '=' (native). */
        char hasobject;         /* non-zero if it has object arrays
                                   in fields */
        int type_num;          /* number representing this type */
        int elsize;             /* element size for this type */
        int alignment;          /* alignment needed for this type */
        struct _arr_descr                                       \
        *subarray;              /* Non-NULL if this type is
                                   is an array (C-contiguous)
                                   of some other type
                                */
        PyObject *fields;       /* The fields dictionary for this type */
                                /* For statically defined descr this
                                   is always Py_None */

        PyObject *names;        /* An ordered tuple of field names or NULL
                                   if no fields are defined */

        PyArray_ArrFuncs *f;     /* a table of functions specific for each
                                    basic data descriptor */
} PyArray_Descr;
