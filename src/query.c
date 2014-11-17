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

PyObject *_attachsql_ConnectionObject_query(_attachsql_ConnectionObject *self, PyObject *args)
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
    return NULL;
  }

  if (!param_list)
  {
    ret= attachsql_query(self->conn, query_length, query, 0, NULL, &error);
    if (error)
    {
      _attachsql_Exception(error);
      return NULL;
    }
    return PyBool_FromLong((long)ret);
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
      return NULL;
    }
    type= PyDict_GetItemString(param_dict, "type");
    value= PyDict_GetItemString(param_dict, "data");
    is_unsigned= PyDict_GetItemString(param_dict, "is_unsigned");
    if (!type || !value || !PyInt_Check(type))
    {
      PyErr_SetString(PyExc_TypeError, "Bad type or value in dict");
      free(asql_params);
      return NULL;
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
  ret= attachsql_query(self->conn, query_length, query, param_count, asql_params, &error);
  free(asql_params);
  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  return PyBool_FromLong((long)ret);
}
