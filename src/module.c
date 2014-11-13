/* vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 * Copyright 2014 Hewlett-Packard Development Company, L.P.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain 
 * a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 */

#include "module.h"

static PyMethodDef _attachsql_methods[] = {
  {
    "get_library_version",
    (PyCFunction)_attachsql_get_library_version,
    METH_NOARGS,
    "get_library_version() -- Returns a string representing the library version."
  },
  {NULL}
};

PyObject *_attachsql_Exception(attachsql_error_t *error)
{
  PyObject *tuple, *err;
  int err_code;
  if (!(tuple= PyTuple_New(2)))
  {
    return NULL;
  }
  if (!error)
  {
    err= _attachsql_InternalError;
    PyTuple_SET_ITEM(tuple, 0, PyInt_FromLong(-1L));
    PyTuple_SET_ITEM(tuple, 1, PyString_FromString("Unknown Error"));
    PyErr_SetObject(err, tuple);
    Py_DECREF(tuple);
    return NULL;
  }
  err_code= attachsql_error_code(error);
  if (err_code >= 2000)
  {
    err= _attachsql_ClientError;
  }
  else
  {
    err= _attachsql_ServerError;
  }
  PyTuple_SET_ITEM(tuple, 0, PyInt_FromLong(err_code));
  PyTuple_SET_ITEM(tuple, 1, PyString_FromString(attachsql_error_message(error)));
  PyErr_SetObject(err, tuple);
  Py_DECREF(tuple);
  attachsql_error_free(error);
  return NULL;
}

PyMODINIT_FUNC
initattachsql(void)
{
  PyObject *module;

  module = Py_InitModule3("attachsql", _attachsql_methods, "a wrapper for the libAttachSQL C API");
  if (!module)
  {
    return;
  }
  /* TODO: add types */
  _attachsql_Error= PyErr_NewException("attachsql.Error", NULL, NULL);
  Py_INCREF(_attachsql_Error);
  PyModule_AddObject(module, "Error", _attachsql_Error);

  _attachsql_InternalError= PyErr_NewException("attachsql.InternalError", NULL, NULL);
  Py_INCREF(_attachsql_InternalError);
  PyModule_AddObject(module, "InternalError", _attachsql_InternalError);

  _attachsql_ClientError= PyErr_NewException("attachsql.ClientError", NULL, NULL);
  Py_INCREF(_attachsql_ClientError);
  PyModule_AddObject(module, "ClientError", _attachsql_ClientError);

  _attachsql_ServerError= PyErr_NewException("attachsql.ServerError", NULL, NULL);
  Py_INCREF(_attachsql_ServerError);
  PyModule_AddObject(module, "ServerError", _attachsql_ServerError);
}
