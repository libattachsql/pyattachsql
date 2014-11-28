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

#include "statement.h"
#include <datetime.h>

extern PyTypeObject PyDateTime_DateType;
extern PyTypeObject PyDateTime_TimeType;

static PyMethodDef _attachsql_StatementObject_methods[]= {
  {
    "execute",
    (PyCFunction)_attachsql_StatementObject_execute,
    METH_NOARGS,
    "Execute the statement"
  },
  {
    "reset",
    (PyCFunction)_attachsql_StatementObject_reset,
    METH_NOARGS,
    "Reset the statement"
  },
  {
    "send_long_data",
    (PyCFunction)_attachsql_StatementObject_send_long_data,
    METH_VARARGS,
    "Send a long data parameter"
  },
  {
    "param_count",
    (PyCFunction)_attachsql_StatementObject_param_count,
    METH_NOARGS,
    "Get a parameter count for the statement"
  },
  {
    "set_int",
    (PyCFunction)_attachsql_StatementObject_set_int,
    METH_VARARGS,
    "Sets an integer parameter"
  },
  {
    "set_float",
    (PyCFunction)_attachsql_StatementObject_set_float,
    METH_VARARGS,
    "Sets a float parameter"
  },
  {
    "set_string",
    (PyCFunction)_attachsql_StatementObject_set_string,
    METH_VARARGS,
    "Sets a string parameter"
  },
  {
    "set_null",
    (PyCFunction)_attachsql_StatementObject_set_null,
    METH_VARARGS,
    "Sets a null parameter"
  },
  {
    "set_datetime",
    (PyCFunction)_attachsql_StatementObject_set_datetime,
    METH_VARARGS,
    "Sets a datetime, timestamp or date parameter"
  },
  {
    "set_time",
    (PyCFunction)_attachsql_StatementObject_set_time,
    METH_VARARGS,
    "Sets a time parameter"
  },
  {
    "row_get",
    (PyCFunction)_attachsql_StatementObject_row_get,
    METH_NOARGS,
    "Process the row in the buffer"
  },
  {
    "get_int",
    (PyCFunction)_attachsql_StatementObject_get_int,
    METH_VARARGS,
    "Get an integer from a row"
  },
  {
    "get_bigint",
    (PyCFunction)_attachsql_StatementObject_get_bigint,
    METH_VARARGS,
    "Get a bigint from a row"
  },
  {
    "get_float",
    (PyCFunction)_attachsql_StatementObject_get_float,
    METH_VARARGS,
    "Get a float from a row"
  },
  {
    "get_char",
    (PyCFunction)_attachsql_StatementObject_get_char,
    METH_VARARGS,
    "Get a char or varchar from a row"
  },
  {
    "get_column_type",
    (PyCFunction)_attachsql_StatementObject_get_column_type,
    METH_VARARGS,
    "Get the type for a column"
  },
  {
    "row_next",
    (PyCFunction)_attachsql_StatementObject_row_next,
    METH_NOARGS,
    "Start recieving the next row"
  },
  {
    "get_column_count",
    (PyCFunction)_attachsql_StatementObject_get_column_count,
    METH_NOARGS,
    "Get the number of columns in the result"
  },
  {NULL, NULL}
};

int _attachsql_StatementObject_Initialize(_attachsql_StatementObject *self, PyObject *args, PyObject *kwargs)
{
  char *statement= NULL;
  int length= 0;
  static char *kwlist[]= {"statement", NULL};
  attachsql_error_t *error= NULL;

  if (!self->pycon)
  {
    PyErr_SetString(PyExc_AttributeError, "Statement needs a connection");
    return -1;
  }
  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s#", kwlist, &statement, &length))
  {
    return -1;
  }
  self->stmt= strndup(statement, length);
  attachsql_statement_prepare(self->pycon->conn, length, self->stmt, &error);
  self->set_ref_store= PyDict_New();
  if (error)
  {
    _attachsql_Exception(error);
    return -1;
  }
  return 0;
}

PyObject *_attachsql_StatementObject_execute(_attachsql_StatementObject *self, PyObject *unused)
{
  attachsql_error_t *error= NULL;

  attachsql_statement_execute(self->pycon->conn, &error);

  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  Py_RETURN_TRUE;
}

PyObject *_attachsql_StatementObject_reset(_attachsql_StatementObject *self, PyObject *unused)
{
  attachsql_error_t *error= NULL;

  attachsql_statement_reset(self->pycon->conn, &error);

  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  Py_RETURN_TRUE;
}

PyObject *_attachsql_StatementObject_send_long_data(_attachsql_StatementObject *self, PyObject *args)
{
  PyObject *data= NULL;
  uint16_t param_no= 0;
  char *str_data= NULL;
  ssize_t length= 0;
  attachsql_error_t *error= NULL;

  if (!PyArg_ParseTuple(args, "IO!", &param_no, &PyString_Type, &data))
  {
    return NULL;
  }

  if (!PyDict_SetItem(self->set_ref_store, PyInt_FromLong((unsigned long) param_no), data))
  {
    return NULL;
  }

  if (!PyString_AsStringAndSize(data, &str_data, &length))
  {
    return NULL;
  }

  attachsql_statement_send_long_data(self->pycon->conn, param_no, length, str_data, &error);
  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  Py_RETURN_TRUE;
}

PyObject *_attachsql_StatementObject_param_count(_attachsql_StatementObject *self, PyObject *unused)
{
  uint16_t param_count;

  param_count= attachsql_statement_get_param_count(self->pycon->conn);

  return PyInt_FromLong((long)param_count);
}

PyObject *_attachsql_StatementObject_set_int(_attachsql_StatementObject *self, PyObject *args)
{
  PyObject *data= NULL;
  uint16_t param_no= 0;
  unsigned char is_unsigned= 0;
  attachsql_error_t *error= NULL;

  if (!PyArg_ParseTuple(args, "IO!|b", &param_no, &PyLong_Type, &data, &is_unsigned))
  {
    return NULL;
  }

  if (!PyDict_SetItem(self->set_ref_store, PyInt_FromLong((unsigned long) param_no), data))
  {
    return NULL;
  }

  if (is_unsigned)
  {
    long long_data= PyLong_AsLong(data);
    attachsql_statement_set_int(self->pycon->conn, param_no, long_data, &error);
  }
  else
  {
    unsigned long long_data= PyLong_AsUnsignedLong(data);
    attachsql_statement_set_unsigned_int(self->pycon->conn, param_no, long_data, &error);
  }
  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  Py_RETURN_TRUE;
}

PyObject *_attachsql_StatementObject_set_bigint(_attachsql_StatementObject *self, PyObject *args)
{
  PyObject *data= NULL;
  uint16_t param_no= 0;
  unsigned char is_unsigned= 0;
  attachsql_error_t *error= NULL;

  if (!PyArg_ParseTuple(args, "IO!|b", &param_no, &PyLong_Type, &data, &is_unsigned))
  {
    return NULL;
  }

  if (!PyDict_SetItem(self->set_ref_store, PyInt_FromLong((unsigned long) param_no), data))
  {
    return NULL;
  }

  if (is_unsigned)
  {
    int64_t long_data= PyLong_AsLongLong(data);
    attachsql_statement_set_bigint(self->pycon->conn, param_no, long_data, &error);
  }
  else
  {
    uint64_t long_data= PyLong_AsUnsignedLongLong(data);
    attachsql_statement_set_unsigned_bigint(self->pycon->conn, param_no, long_data, &error);
  }
  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  Py_RETURN_TRUE;
}

PyObject *_attachsql_StatementObject_set_float(_attachsql_StatementObject *self, PyObject *args)
{
  PyObject *data= NULL;
  uint16_t param_no= 0;
  attachsql_error_t *error= NULL;
  double double_data= 0;

  if (!PyArg_ParseTuple(args, "IO!", &param_no, &PyFloat_Type, &data))
  {
    return NULL;
  }

  if (!PyDict_SetItem(self->set_ref_store, PyInt_FromLong((unsigned long) param_no), data))
  {
    return NULL;
  }

  double_data= PyFloat_AsDouble(data);
  attachsql_statement_set_double(self->pycon->conn, param_no, double_data, &error);

  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  Py_RETURN_TRUE;
}

PyObject *_attachsql_StatementObject_set_string(_attachsql_StatementObject *self, PyObject *args)
{
  PyObject *data= NULL;
  uint16_t param_no= 0;
  char *str_data= NULL;
  ssize_t length= 0;
  attachsql_error_t *error= NULL;

  if (!PyArg_ParseTuple(args, "IO!", &param_no, &PyString_Type, &data))
  {
    return NULL;
  }

  if (!PyDict_SetItem(self->set_ref_store, PyInt_FromLong((unsigned long) param_no), data))
  {
    return NULL;
  }

  if (!PyString_AsStringAndSize(data, &str_data, &length))
  {
    return NULL;
  }

  attachsql_statement_set_string(self->pycon->conn, param_no, length, str_data, &error);
  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  Py_RETURN_TRUE;
}

PyObject *_attachsql_StatementObject_set_null(_attachsql_StatementObject *self, PyObject *args)
{
  uint16_t param_no= 0;
  attachsql_error_t *error= NULL;

  if (!PyArg_ParseTuple(args, "I", &param_no))
  {
    return NULL;
  }

  attachsql_statement_set_null(self->pycon->conn, param_no, &error);

  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  Py_RETURN_TRUE;
}

PyObject *_attachsql_StatementObject_set_datetime(_attachsql_StatementObject *self, PyObject *args)
{
  PyObject *data= NULL;
  uint16_t param_no= 0;
  attachsql_error_t *error= NULL;
  int year, month, day, hour, minute, second, microsecond;

  if (!PyArg_ParseTuple(args, "IO", &param_no, &data))
  {
    return NULL;
  }

  if (!PyDate_Check(data))
  {
    return NULL;
  }

  if (!PyDict_SetItem(self->set_ref_store, PyInt_FromLong((unsigned long) param_no), data))
  {
    return NULL;
  }

  year= PyDateTime_GET_YEAR(data);
  month= PyDateTime_GET_MONTH(data);
  day= PyDateTime_GET_DAY(data);
  if (PyDateTime_Check(data))
  {
    hour= PyDateTime_DATE_GET_HOUR(data);
    minute= PyDateTime_DATE_GET_MINUTE(data);
    second= PyDateTime_DATE_GET_SECOND(data);
    microsecond= PyDateTime_DATE_GET_MICROSECOND(data);
  }
  else
  {
    hour= 0;
    minute= 0;
    second= 0;
    microsecond= 0;
  }
  attachsql_statement_set_datetime(self->pycon->conn, param_no, year, month, day, hour, minute, second, microsecond, &error);

  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  Py_RETURN_TRUE;
}

PyObject *_attachsql_StatementObject_set_time(_attachsql_StatementObject *self, PyObject *args)
{
  PyObject *data= NULL;
  uint16_t param_no= 0;
  attachsql_error_t *error= NULL;
  int hour, minute, second, microsecond;

  if (!PyArg_ParseTuple(args, "IO", &param_no, &data))
  {
    return NULL;
  }

  if (!PyTime_Check(data))
  {
    return NULL;
  }

  if (!PyDict_SetItem(self->set_ref_store, PyInt_FromLong((unsigned long) param_no), data))
  {
    return NULL;
  }

  hour= PyDateTime_TIME_GET_HOUR(data);
  minute= PyDateTime_TIME_GET_MINUTE(data);
  second= PyDateTime_TIME_GET_SECOND(data);
  microsecond= PyDateTime_TIME_GET_MICROSECOND(data);
  attachsql_statement_set_time(self->pycon->conn, param_no, hour, minute, second, microsecond, false, &error);

  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  Py_RETURN_TRUE;
}

PyObject *_attachsql_StatementObject_row_get(_attachsql_StatementObject *self, PyObject *args)
{
  attachsql_error_t *error= NULL;

  attachsql_statement_row_get(self->pycon->conn, &error);

  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  Py_RETURN_TRUE;
}

PyObject *_attachsql_StatementObject_get_int(_attachsql_StatementObject *self, PyObject *args)
{
  attachsql_error_t *error= NULL;
  unsigned int column_no= 0;
  unsigned char get_unsigned= 0;
  if (!PyArg_ParseTuple(args, "I|I", &column_no, &get_unsigned))
  {
    return NULL;
  }

  if (get_unsigned)
  {
    uint32_t result= attachsql_statement_get_int_unsigned(self->pycon->conn, column_no, &error);
    if (!error)
    {
      return PyLong_FromUnsignedLong((unsigned long)result);
    }
  }
  else
  {
    int32_t result= attachsql_statement_get_int(self->pycon->conn, column_no, &error);
    if (!error)
    {
      return PyLong_FromLong((long)result);
    }
  }
  _attachsql_Exception(error);
  return NULL;
}

PyObject *_attachsql_StatementObject_get_bigint(_attachsql_StatementObject *self, PyObject *args)
{
  attachsql_error_t *error= NULL;
  unsigned int column_no= 0;
  unsigned char get_unsigned= 0;
  if (!PyArg_ParseTuple(args, "I|I", &column_no, &get_unsigned))
  {
    return NULL;
  }

  if (get_unsigned)
  {
    uint64_t result= attachsql_statement_get_bigint_unsigned(self->pycon->conn, column_no, &error);
    if (!error)
    {
      return PyLong_FromUnsignedLongLong(result);
    }
  }
  else
  {
    int64_t result= attachsql_statement_get_bigint(self->pycon->conn, column_no, &error);
    if (!error)
    {
      return PyLong_FromLongLong(result);
    }
  }
  _attachsql_Exception(error);
  return NULL;
}

PyObject *_attachsql_StatementObject_get_float(_attachsql_StatementObject *self, PyObject *args)
{
  attachsql_error_t *error= NULL;
  unsigned int column_no= 0;
  if (!PyArg_ParseTuple(args, "I", &column_no))
  {
    return NULL;
  }

  double result= attachsql_statement_get_double(self->pycon->conn, column_no, &error);
  if (!error)
  {
    return PyFloat_FromDouble(result);
  }
  _attachsql_Exception(error);
  return NULL;
}

PyObject *_attachsql_StatementObject_get_char(_attachsql_StatementObject *self, PyObject *args)
{
  attachsql_error_t *error= NULL;
  unsigned int column_no= 0;
  size_t length= 0;
  if (!PyArg_ParseTuple(args, "I", &column_no))
  {
    return NULL;
  }

  char *result= attachsql_statement_get_char(self->pycon->conn, column_no, &length, &error);
  if (!error)
  {
    return PyString_FromStringAndSize(result, length);
  }
  _attachsql_Exception(error);
  return NULL;
}

PyObject *_attachsql_StatementObject_get_column_type(_attachsql_StatementObject *self, PyObject *args)
{
  unsigned int column_no= 0;
  int result;
  if (!PyArg_ParseTuple(args, "I", &column_no))
  {
    return NULL;
  }
  result= attachsql_statement_get_column_type(self->pycon->conn, column_no);
  return PyInt_FromLong((long)result);
}

PyObject *_attachsql_StatementObject_row_next(_attachsql_StatementObject *self, PyObject *unused)
{
  attachsql_statement_row_next(self->pycon->conn);
  Py_RETURN_NONE;
}

PyObject *_attachsql_StatementObject_get_column_count(_attachsql_StatementObject *self, PyObject *unused)
{
  uint16_t column_count= attachsql_statement_get_column_count(self->pycon->conn);
  return PyLong_FromUnsignedLong((unsigned long)column_count);
}

void _attachsql_StatementObject_dealloc(_attachsql_StatementObject *self)
{
  PyObject_GC_UnTrack(self);
  attachsql_statement_close(self->pycon->conn);
  Py_DECREF(self->set_ref_store);
  Py_DECREF(self->pycon);
  free(self->stmt);
  self->ob_type->tp_free(self);
}

PyObject *_attachsql_StatementObject_getattr(_attachsql_StatementObject *self, char *name)
{
  PyObject *res;
  //struct PyMemberDef *def;

  res= Py_FindMethod(_attachsql_StatementObject_methods, (PyObject *)self, name);
  if (res != NULL)
  {
    return res;
  }
  PyErr_SetString(PyExc_AttributeError, name);
  return NULL;
}

int _attachsql_StatementObject_setattr(_attachsql_StatementObject *self, char *name, PyObject *v)
{
  if (v == NULL)
  {
    PyErr_SetString(PyExc_AttributeError, "can't set attribute");
    return -1;
  }
  PyErr_SetString(PyExc_AttributeError, name);
  return -1;
}

PyObject *_attachsql_StatementObject_repr(_attachsql_StatementObject *self)
{
  char buffer[300];
  snprintf(buffer, 300, "<attachsql.statement at %lx>", (long)self);
  return PyString_FromString(buffer);
}

int _attachsql_StatementObject_traverse(_attachsql_StatementObject *self, visitproc visit, void *arg)
{
  return 0;
}

int _attachsql_StatementObject_clear(_attachsql_StatementObject *self)
{
  return 0;
}

PyTypeObject _attachsql_StatementObject_Type= {
  PyObject_HEAD_INIT(NULL)
  0,
  "attachsql.statement",
  sizeof(_attachsql_StatementObject),
  0,
  (destructor)_attachsql_StatementObject_dealloc,
  0,
  (getattrfunc)_attachsql_StatementObject_getattr,
  (setattrfunc)_attachsql_StatementObject_setattr,
  0,
  (reprfunc)_attachsql_StatementObject_repr,

  0,
  0,
  0,

  0,
  0,
  0,
  0,
  0,

  0,

  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_BASETYPE,
  "Returns a prepared statement object",
  (traverseproc)_attachsql_StatementObject_traverse,
  (inquiry)_attachsql_StatementObject_clear,

  0,

  0,

  0,
  0,

  (struct PyMethodDef *)_attachsql_StatementObject_methods,
  0,

  0,
  0,
  0,
  0,
  0,
  0,
  (initproc)_attachsql_StatementObject_Initialize,
  NULL,
  NULL,
  NULL,
  0,
  0,
  0
};
