static PyTypeObject MyType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "My name",                 /*tp_name*/
    sizeof(mytype),            /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_str*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "My object",               /* tp_doc */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    _methods,                  /* tp_methods */
    _members,                  /* tp_members */
    _getset,                   /* tp_getset */
    0,                         /* tp_dict */
    (initproc)_init     ,      /* tp_init */
    0,                         /* tp_alloc */
    _new,                      /* tp_new */
};
