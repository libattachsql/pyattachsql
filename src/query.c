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

#include "query.h"

extern PyTypeObject _attachsql_QueryObject_Type;
extern PyObject *_attachsql_ClientError;

static PyMethodDef _attachsql_QueryObject_methods[]= {
  {
    "column_count",
    (PyCFunction)_attachsql_QueryObject_column_count,
    METH_NOARGS,
    "Get the column count for a query"
  },
  {
    "row_get",
    (PyCFunction)_attachsql_QueryObject_row_get,
    METH_NOARGS,
    "Get the current row"
  },
  {
    "row_next",
    (PyCFunction)_attachsql_QueryObject_row_next,
    METH_NOARGS,
    "Start retrieving the next row"
  },
  {
    "insert_id",
    (PyCFunction)_attachsql_QueryObject_last_insert_id,
    METH_NOARGS,
    "Get the last insert id for the connection"
  },
  {
    "affected_rows",
    (PyCFunction)_attachsql_QueryObject_affected_rows,
    METH_NOARGS,
    "Get the number of affected rows"
  },
  {
    "warning_count",
    (PyCFunction)_attachsql_QueryObject_warning_count,
    METH_NOARGS,
    "Get the number of warnings"
  },
  {
    "info",
    (PyCFunction)_attachsql_QueryObject_info,
    METH_NOARGS,
    "Get the query info"
  },
  {
    "row_count",
    (PyCFunction)_attachsql_QueryObject_row_count,
    METH_NOARGS,
    "Get the row count for a buffered query"
  },
  {
    "next_result",
    (PyCFunction)_attachsql_QueryObject_next_result,
    METH_NOARGS,
    "Start retrieving the next result set"
  },
  {
    "buffer_rows",
    (PyCFunction)_attachsql_QueryObject_buffer_rows,
    METH_NOARGS,
    "Enable the buffering of query results"
  },
  {
    "buffer_row_get",
    (PyCFunction)_attachsql_QueryObject_buffer_row_get,
    METH_NOARGS,
    "Get the next row in a buffered result set"
  },
  {
    "row_get_offset",
    (PyCFunction)_attachsql_QueryObject_row_get_offset,
    METH_VARARGS,
    "Get a specified row from a buffered result set"
  }
};

int _attachsql_QueryObject_Initialize(_attachsql_QueryObject *self, PyObject *args, PyObject *kwargs)
{
  bool ret;
  char *query= NULL;
  int query_length;
  int param_count= 0;
  int i;
  int64_t tmp_int;
  uint64_t tmp_uint;
  double tmp_double;
  attachsql_query_parameter_st *asql_params= NULL;
  PyObject *param_list= NULL;
  PyObject *param_dict= NULL;
  PyObject *type= NULL;
  PyObject *value= NULL;
  PyObject *is_unsigned= NULL;
  attachsql_error_t *error= NULL;
  if (!PyArg_ParseTuple(args, "s#|O!", &query, &query_length, &PyList_Type, &param_list))
  {
    return -1;
  }

  if (!param_list)
  {
    ret= attachsql_query(self->pycon->conn, query_length, query, 0, NULL, &error);
    if (error)
    {
      _attachsql_Exception(error);
      return -1;
    }
    return 0;
  }
  /* For parameters the user should use:
   * .query("SELECT * FROM test WHERE a=? and b=?", [{'type': ESCAPE_TYPE_CHAR, 'data': 'hello'}, {'type': ESCAPE_TYPE_INT, 'data': 1}])
   */
  param_count= PyList_Size(param_list);
  asql_params= (attachsql_query_parameter_st*)malloc(sizeof(attachsql_query_parameter_st) * param_count);

  for (i= 0; i < param_count; i++)
  {
    param_dict= PyList_GetItem(param_list, i);
    if (!PyDict_Check(param_dict))
    {
      PyErr_SetString(PyExc_TypeError, "Dict not found in list");
      free(asql_params);
      return -1;
    }
    type= PyDict_GetItemString(param_dict, "type");
    value= PyDict_GetItemString(param_dict, "data");
    is_unsigned= PyDict_GetItemString(param_dict, "is_unsigned");
    if (!type || !value || !PyInt_Check(type))
    {
      PyErr_SetString(PyExc_TypeError, "Bad type or value in dict");
      free(asql_params);
      return -1;
    }
    asql_params[i].type= PyInt_AsLong(type);
    switch (asql_params[i].type)
    {
      case ATTACHSQL_ESCAPE_TYPE_NONE:
        asql_params[i].data= NULL;
        break;
      case ATTACHSQL_ESCAPE_TYPE_CHAR:
      case ATTACHSQL_ESCAPE_TYPE_CHAR_LIKE:
        asql_params[i].data= PyString_AsString(value);
        asql_params[i].length= PyString_Size(value);
        break;
      case ATTACHSQL_ESCAPE_TYPE_INT:
        if (!is_unsigned && !PyInt_AsLong(is_unsigned))
        {
          tmp_int= PyLong_AsLong(value);
          asql_params[i].data= &tmp_int;
          asql_params[i].is_unsigned= false;
        }
        else
        {
          tmp_uint= PyLong_AsUnsignedLong(value);
          asql_params[i].data= &tmp_uint;
          asql_params[i].is_unsigned= true;
        }
        break;
      case ATTACHSQL_ESCAPE_TYPE_BIGINT:
        if (!is_unsigned && !PyInt_AsLong(is_unsigned))
        {
          tmp_int= PyLong_AsLongLong(value);
          asql_params[i].data= &tmp_int;
          asql_params[i].is_unsigned= false;
        }
        else
        {
          tmp_uint= PyLong_AsUnsignedLongLong(value);
          asql_params[i].data= &tmp_uint;
          asql_params[i].is_unsigned= true;
        }
        break;
      case ATTACHSQL_ESCAPE_TYPE_FLOAT:
      case ATTACHSQL_ESCAPE_TYPE_DOUBLE:
        tmp_double= PyFloat_AsDouble(value);
        asql_params[i].data= &tmp_double;
        break;
    }
  }
  ret= attachsql_query(self->pycon->conn, query_length, query, param_count, asql_params, &error);
  free(asql_params);
  if (error)
  {
    _attachsql_Exception(error);
    return -1;
  }
  return 0;
}

PyObject *_attachsql_QueryObject_column_count(_attachsql_QueryObject *self, PyObject *unused)
{
  uint16_t count;
  count= attachsql_query_column_count(self->pycon->conn);
  return PyInt_FromLong((long) count);
}

PyObject *_attachsql_QueryObject_row_get(_attachsql_QueryObject *self, PyObject *unused)
{
  attachsql_error_t *error= NULL;
  attachsql_query_row_st *row;
  uint16_t column_count;
  uint16_t column;
  PyObject *py_row= NULL;
  PyObject *data= NULL;

  column_count= attachsql_query_column_count(self->pycon->conn);
  if (!column_count)
  {
    Py_RETURN_NONE;
  }
  row= attachsql_query_row_get(self->pycon->conn, &error);
  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  if (!row)
  {
    Py_RETURN_NONE;
  }
  py_row= PyTuple_New(column_count);
  if (!py_row)
  {
    return NULL;
  }

  for (column= 0; column < column_count; column++)
  {
    data= PyString_FromStringAndSize(row[column].data, row[column].length);
    PyTuple_SET_ITEM(py_row, column, data);
  }
  return py_row;
}

PyObject *_attachsql_QueryObject_row_next(_attachsql_QueryObject *self, PyObject *unused)
{
  attachsql_query_row_next(self->pycon->conn);
  Py_RETURN_NONE;
}

PyObject *_attachsql_QueryObject_last_insert_id(_attachsql_QueryObject *self, PyObject *unused)
{
  uint64_t insert_id;

  insert_id= attachsql_connection_last_insert_id(self->pycon->conn);

  return PyLong_FromUnsignedLongLong(insert_id);
}

PyObject *_attachsql_QueryObject_affected_rows(_attachsql_QueryObject *self, PyObject *unused)
{
  uint64_t affected_rows;

  affected_rows= attachsql_query_affected_rows(self->pycon->conn);

  return PyLong_FromUnsignedLongLong(affected_rows);
}

PyObject *_attachsql_QueryObject_warning_count(_attachsql_QueryObject *self, PyObject *unused)
{
  uint32_t warning_count;

  warning_count= attachsql_query_warning_count(self->pycon->conn);

  return PyLong_FromUnsignedLong(warning_count);
}

PyObject *_attachsql_QueryObject_info(_attachsql_QueryObject *self, PyObject *unused)
{
  const char *query_info;

  query_info= attachsql_query_info(self->pycon->conn);

  return PyString_FromString(query_info);
}

PyObject *_attachsql_QueryObject_row_count(_attachsql_QueryObject *self, PyObject *unused)
{
  uint64_t row_count;

  row_count= attachsql_query_row_count(self->pycon->conn);

  return PyLong_FromUnsignedLongLong(row_count);
}

PyObject *_attachsql_QueryObject_next_result(_attachsql_QueryObject *self, PyObject *unused)
{
  attachsql_return_t ret;

  ret= attachsql_query_next_result(self->pycon->conn);

  return PyInt_FromLong((long)ret);
}

PyObject *_attachsql_QueryObject_buffer_rows(_attachsql_QueryObject *self, PyObject *unused)
{
  bool success;
  PyObject *tuple;

  if (!(tuple= PyTuple_New(2)))
  {
    return NULL;
  }

  success= attachsql_query_buffer_rows(self->pycon->conn, true);

  if (!success)
  {
    PyTuple_SET_ITEM(tuple, 0, PyInt_FromLong(-1L));
    PyTuple_SET_ITEM(tuple, 1, PyString_FromString("Cannot set buffered rows at this stage"));
    PyErr_SetObject(_attachsql_ClientError, tuple);
    Py_DECREF(tuple);
    return NULL;
  }

  Py_RETURN_TRUE;
}

PyObject *_attachsql_QueryObject_buffer_row_get(_attachsql_QueryObject *self, PyObject *unused)
{
  attachsql_query_row_st *row;
  uint16_t column_count;
  uint16_t column;
  PyObject *py_row= NULL;
  PyObject *data= NULL;

  column_count= attachsql_query_column_count(self->pycon->conn);
  if (!column_count)
  {
    Py_RETURN_NONE;
  }
  row= attachsql_query_buffer_row_get(self->pycon->conn);
  if (!row)
  {
    Py_RETURN_NONE;
  }
  py_row= PyTuple_New(column_count);
  if (!py_row)
  {
    return NULL;
  }

  for (column= 0; column < column_count; column++)
  {
    data= PyString_FromStringAndSize(row[column].data, row[column].length);
    PyTuple_SET_ITEM(py_row, column, data);
  }
  return py_row;
}

PyObject *_attachsql_QueryObject_row_get_offset(_attachsql_QueryObject *self, PyObject *args)
{
  attachsql_query_row_st *row;
  uint16_t column_count;
  uint16_t column;
  uint64_t row_no;
  PyObject *py_row= NULL;
  PyObject *data= NULL;
  PyObject *tuple= NULL;

  if (!PyArg_ParseTuple(args, "K", &row_no))
  {
    return NULL;
  }

  column_count= attachsql_query_column_count(self->pycon->conn);
  if (!column_count)
  {
    Py_RETURN_NONE;
  }
  row= attachsql_query_row_get_offset(self->pycon->conn, row_no);
  if (!row)
  {
    PyTuple_SET_ITEM(tuple, 0, PyInt_FromLong(-1L));
    PyTuple_SET_ITEM(tuple, 1, PyString_FromString("Row not found"));
    PyErr_SetObject(_attachsql_ClientError, tuple);
    Py_DECREF(tuple);
    return NULL;
  }
  py_row= PyTuple_New(column_count);
  if (!py_row)
  {
    return NULL;
  }

  for (column= 0; column < column_count; column++)
  {
    data= PyString_FromStringAndSize(row[column].data, row[column].length);
    PyTuple_SET_ITEM(py_row, column, data);
  }
  return py_row;
}

void _attachsql_QueryObject_dealloc(_attachsql_QueryObject *self)
{
  PyObject_GC_UnTrack(self);
  if (!self->pycon->in_group)
  {
    attachsql_query_close(self->pycon->conn);
  }
  self->pycon->query= NULL;
  Py_XDECREF(self->pycon);
  self->ob_type->tp_free(self);
}

PyObject *_attachsql_QueryObject_getattr(_attachsql_QueryObject *self, char *name)
{
  PyObject *res;
  //struct PyMemberDef *def;

  res= Py_FindMethod(_attachsql_QueryObject_methods, (PyObject *)self, name);
  if (res != NULL)
  {
    return res;
  }
  PyErr_SetString(PyExc_AttributeError, name);
  return NULL;
}

int _attachsql_QueryObject_setattr(_attachsql_QueryObject *self, char *name, PyObject *v)
{
  if (v == NULL)
  {
    PyErr_SetString(PyExc_AttributeError, "can't set attribute");
    return -1;
  }
  PyErr_SetString(PyExc_AttributeError, name);
  return -1;
}

PyObject *_attachsql_QueryObject_repr(_attachsql_QueryObject *self)
{
  char buffer[300];
  snprintf(buffer, 300, "<attachsql.query at %lx>", (long)self);
  return PyString_FromString(buffer);
}

int _attachsql_QueryObject_traverse(_attachsql_QueryObject *self, visitproc visit, void *arg)
{
  return 0;
}

int _attachsql_QueryObject_clear(_attachsql_QueryObject *self)
{
  return 0;
}

PyTypeObject _attachsql_QueryObject_Type= {
  PyObject_HEAD_INIT(NULL)
  0,
  "attachsql.query",
  sizeof(_attachsql_QueryObject),
  0,
  (destructor)_attachsql_QueryObject_dealloc,
  0,
  (getattrfunc)_attachsql_QueryObject_getattr,
  (setattrfunc)_attachsql_QueryObject_setattr,
  0,
  (reprfunc)_attachsql_QueryObject_repr,

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
  "Returns a query object",
  (traverseproc)_attachsql_QueryObject_traverse,
  (inquiry)_attachsql_QueryObject_clear,

  0,

  0,

  0,
  0,

  (struct PyMethodDef *)_attachsql_QueryObject_methods,
  0,

  0,
  0,
  0,
  0,
  0,
  0,
  (initproc)_attachsql_QueryObject_Initialize,
  NULL,
  NULL,
  NULL,
  0,
  0,
  0
};
