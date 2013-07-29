void initnumpycbe()
{
  (void)Py_InitModule("numpycbe", methods);
  setUpDispatcher();
}
