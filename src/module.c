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
#include "connection.h"
#include "group.h"
#include <datetime.h>

extern PyTypeObject _attachsql_ConnectionObject_Type;
extern PyTypeObject _attachsql_StatementObject_Type;
extern PyTypeObject _attachsql_QueryObject_Type;
extern PyTypeObject _attachsql_GroupObject_Type;

PyObject *_attachsql_Error;
  PyObject *_attachsql_InternalError;
  PyObject *_attachsql_ClientError;
  PyObject *_attachsql_ServerError;

static PyMethodDef _attachsql_methods[] = {
  {
    "connect",
    (PyCFunction)_attachsql_connect,
    METH_VARARGS | METH_KEYWORDS,
    "connect to a MySQL server"
  },
  {
    "group",
    (PyCFunction)_attachsql_group,
    METH_VARARGS | METH_KEYWORDS,
    "create a connection group"
  },
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

PyObject *_attachsql_connect(PyObject *self, PyObject *args, PyObject *kwargs)
{
  _attachsql_ConnectionObject *con= NULL;

  con= (_attachsql_ConnectionObject*) _attachsql_ConnectionObject_Type.tp_alloc(&_attachsql_ConnectionObject_Type, 0);
  if (con == NULL)
  {
    return NULL;
  }
  if (_attachsql_ConnectionObject_Initialize(con, args, kwargs))
  {
    Py_DECREF(con);
    con= NULL;
  }
  return (PyObject *) con;
}

PyObject *_attachsql_group(PyObject *self, PyObject *args, PyObject *kwargs)
{
  _attachsql_GroupObject *group= NULL;

  group= (_attachsql_GroupObject*) _attachsql_GroupObject_Type.tp_alloc(&_attachsql_GroupObject_Type, 0);
  if (group == NULL)
  {
    return NULL;
  }
  if (_attachsql_GroupObject_Initialize(group, args, kwargs))
  {
    Py_DECREF(group);
    group= NULL;
  }
  return (PyObject *) group;
}

PyMODINIT_FUNC
initattachsql(void)
{
  PyObject *module, *dict;

  module = Py_InitModule3("attachsql", _attachsql_methods, "a wrapper for the libAttachSQL C API");
  if (!module)
  {
    return;
  }
  if (! PyEval_ThreadsInitialized())
  {
    PyEval_InitThreads();
  }
  _attachsql_ConnectionObject_Type.ob_type= &PyType_Type;
  _attachsql_ConnectionObject_Type.tp_alloc= PyType_GenericAlloc;
  _attachsql_ConnectionObject_Type.tp_new= PyType_GenericNew;
  _attachsql_ConnectionObject_Type.tp_free= PyObject_GC_Del;

  _attachsql_StatementObject_Type.ob_type= &PyType_Type;
  _attachsql_StatementObject_Type.tp_alloc= PyType_GenericAlloc;
  _attachsql_StatementObject_Type.tp_new= PyType_GenericNew;
  _attachsql_StatementObject_Type.tp_free= PyObject_GC_Del;

  _attachsql_QueryObject_Type.ob_type= &PyType_Type;
  _attachsql_QueryObject_Type.tp_alloc= PyType_GenericAlloc;
  _attachsql_QueryObject_Type.tp_new= PyType_GenericNew;
  _attachsql_QueryObject_Type.tp_free= PyObject_GC_Del;

  _attachsql_GroupObject_Type.ob_type= &PyType_Type;
  _attachsql_GroupObject_Type.tp_alloc= PyType_GenericAlloc;
  _attachsql_GroupObject_Type.tp_new= PyType_GenericNew;
  _attachsql_GroupObject_Type.tp_free= PyObject_GC_Del;

  if (!(dict = PyModule_GetDict(module)))
  {
    if (PyErr_Occurred())
    {
      PyErr_SetString(PyExc_ImportError, "attachsql: init failed");
      return;
    }
  }

  if (PyDict_SetItemString(dict, "connection", (PyObject*)&_attachsql_ConnectionObject_Type))
  {
    if (PyErr_Occurred())
    {
      PyErr_SetString(PyExc_ImportError, "attachsql: init failed");
      return;
    }
    Py_INCREF(&_attachsql_ConnectionObject_Type);
  }

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

  PyDateTime_IMPORT;

  PyModule_AddIntConstant(module, "RETURN_NONE", ATTACHSQL_RETURN_NONE);
  PyModule_AddIntConstant(module, "RETURN_NOT_CONNECTED", ATTACHSQL_RETURN_NOT_CONNECTED);
  PyModule_AddIntConstant(module, "RETURN_IDLE", ATTACHSQL_RETURN_IDLE);
  PyModule_AddIntConstant(module, "RETURN_PROCESSING", ATTACHSQL_RETURN_PROCESSING);
  PyModule_AddIntConstant(module, "RETURN_ROW_READY", ATTACHSQL_RETURN_ROW_READY);
  PyModule_AddIntConstant(module, "RETURN_ERROR", ATTACHSQL_RETURN_ERROR);
  PyModule_AddIntConstant(module, "RETURN_EOF", ATTACHSQL_RETURN_EOF);
  PyModule_AddIntConstant(module, "ESCAPE_TYPE_NONE", ATTACHSQL_ESCAPE_TYPE_NONE);
  PyModule_AddIntConstant(module, "ESCAPE_TYPE_CHAR", ATTACHSQL_ESCAPE_TYPE_CHAR);
  PyModule_AddIntConstant(module, "ESCAPE_TYPE_CHAR_LIKE", ATTACHSQL_ESCAPE_TYPE_CHAR_LIKE);
  PyModule_AddIntConstant(module, "ESCAPE_TYPE_INT", ATTACHSQL_ESCAPE_TYPE_INT);
  PyModule_AddIntConstant(module, "ESCAPE_TYPE_BIGINT", ATTACHSQL_ESCAPE_TYPE_BIGINT);
  PyModule_AddIntConstant(module, "ESCAPE_TYPE_FLOAT", ATTACHSQL_ESCAPE_TYPE_FLOAT);
  PyModule_AddIntConstant(module, "ESCAPE_TYPE_DOUBLE", ATTACHSQL_ESCAPE_TYPE_DOUBLE);
  PyModule_AddIntConstant(module, "OPTION_COMPRESS", ATTACHSQL_OPTION_COMPRESS);
  PyModule_AddIntConstant(module, "OPTION_FOUND_ROWS", ATTACHSQL_OPTION_FOUND_ROWS);
  PyModule_AddIntConstant(module, "OPTION_IGNORE_SIGPIPE", ATTACHSQL_OPTION_IGNORE_SIGPIPE);
  PyModule_AddIntConstant(module, "OPTION_INTERACTIVE", ATTACHSQL_OPTION_INTERACTIVE);
  PyModule_AddIntConstant(module, "OPTION_LOCAL_FILES", ATTACHSQL_OPTION_LOCAL_FILES);
  PyModule_AddIntConstant(module, "OPTION_MULTI_STATEMENTS", ATTACHSQL_OPTION_MULTI_STATEMENTS);
  PyModule_AddIntConstant(module, "OPTION_NO_SCHEMA", ATTACHSQL_OPTION_NO_SCHEMA);
  PyModule_AddIntConstant(module, "OPTION_SSL_NO_VERIFY", ATTACHSQL_OPTION_SSL_NO_VERIFY);
  PyModule_AddIntConstant(module, "OPTION_SEMI_BLOCKING", ATTACHSQL_OPTION_SEMI_BLOCKING);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_DECIMAL", ATTACHSQL_COLUMN_TYPE_DECIMAL);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_TINY", ATTACHSQL_COLUMN_TYPE_TINY);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_SHORT", ATTACHSQL_COLUMN_TYPE_SHORT);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_LONG", ATTACHSQL_COLUMN_TYPE_LONG);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_FLOAT", ATTACHSQL_COLUMN_TYPE_FLOAT);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_DOUBLE", ATTACHSQL_COLUMN_TYPE_DOUBLE);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_NULL", ATTACHSQL_COLUMN_TYPE_NULL);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_TIMESTAMP", ATTACHSQL_COLUMN_TYPE_TIMESTAMP);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_LONGLONG", ATTACHSQL_COLUMN_TYPE_LONGLONG);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_INT24", ATTACHSQL_COLUMN_TYPE_INT24);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_DATE", ATTACHSQL_COLUMN_TYPE_DATE);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_TIME", ATTACHSQL_COLUMN_TYPE_TIME);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_DATETIME", ATTACHSQL_COLUMN_TYPE_DATETIME);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_YEAR", ATTACHSQL_COLUMN_TYPE_YEAR);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_VARCHAR", ATTACHSQL_COLUMN_TYPE_VARCHAR);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_BIT", ATTACHSQL_COLUMN_TYPE_BIT);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_NEWDECIMAL", ATTACHSQL_COLUMN_TYPE_NEWDECIMAL);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_ENUM", ATTACHSQL_COLUMN_TYPE_ENUM);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_SET", ATTACHSQL_COLUMN_TYPE_SET);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_TINY_BLOB", ATTACHSQL_COLUMN_TYPE_TINY_BLOB);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_MEDIUM_BLOB", ATTACHSQL_COLUMN_TYPE_MEDIUM_BLOB);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_LONG_BLOB", ATTACHSQL_COLUMN_TYPE_LONG_BLOB);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_BLOB", ATTACHSQL_COLUMN_TYPE_BLOB);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_VARSTRING", ATTACHSQL_COLUMN_TYPE_VARSTRING);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_STRING", ATTACHSQL_COLUMN_TYPE_STRING);
  PyModule_AddIntConstant(module, "COLUMN_TYPE_GEOMETRY", ATTACHSQL_COLUMN_TYPE_GEOMETRY);

  PyModule_AddIntConstant(module, "EVENT_CONNECTED", ATTACHSQL_EVENT_CONNECTED);
  PyModule_AddIntConstant(module, "EVENT_ERROR", ATTACHSQL_EVENT_ERROR);
  PyModule_AddIntConstant(module, "EVENT_EOF", ATTACHSQL_EVENT_EOF);
  PyModule_AddIntConstant(module, "EVENT_ROW_READY", ATTACHSQL_EVENT_ROW_READY);
}
