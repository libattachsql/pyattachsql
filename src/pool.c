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

#include "pool.h"

extern PyTypeObject _attachsql_QueryObject_Type;

static PyMethodDef _attachsql_PoolObject_methods[]= {
  {
    "create_connection",
    (PyCFunction)_attachsql_PoolObject_create_connection,
    METH_VARARGS,
    "Create a new connection in the pool"
  },
  {
    "run",
    (PyCFunction)_attachsql_PoolObject_run,
    METH_NOARGS,
    "Run a connection pool"
  },
  { NULL, NULL }
};

int _attachsql_PoolObject_Initialize(_attachsql_PoolObject *self, PyObject *args, PyObject *kwargs)
{
  PyObject *cb_func= NULL;
  PyObject *cb_args= NULL;
  static char *kwlist[]= {"cbfunction", "cbargs", NULL};
  attachsql_error_t *error= NULL;

  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|O", kwlist, &cb_func, &cb_args))
  {
    return -1;
  }
  if (cb_func)
  {
    Py_XINCREF(cb_func);
  }
  if (cb_args)
  {
    Py_XINCREF(cb_args);
  }
  self->cb_func= cb_func;
  self->cb_args= cb_args;
  self->pool= attachsql_pool_create(&error);
  self->conn_list= PyList_New(0);
  self->context_list= PyList_New(0);
  if (error)
  {
    _attachsql_Exception(error);
    return -1;
  }
  return 0;
}

void _attachsql_callback(attachsql_connect_t *con, attachsql_events_t events, void *context, attachsql_error_t *error)
{
  PyObject *ctext_data= (PyObject*) context;
  _attachsql_ConnectionObject *pycon= (_attachsql_ConnectionObject*) PyTuple_GetItem(ctext_data, 0);
  _attachsql_PoolObject *self= (_attachsql_PoolObject*) PyTuple_GetItem(ctext_data, 1);
  PyGILState_STATE gstate;

  if (self->cb_func)
  {
    PyObject *cbargs;
    if (self->cb_args)
    {
      cbargs= Py_BuildValue("iOOO", events, pycon, pycon->query, self->cb_args);
    }
    else
    {
      cbargs= Py_BuildValue("iOO", events, pycon, pycon->query); 
    }
    gstate = PyGILState_Ensure();
    if (error)
    {
      _attachsql_Exception(error);
      PyGILState_Release(gstate);
      return;
    }
    PyObject_CallObject(self->cb_func, cbargs);
    Py_DECREF(cbargs);
    PyGILState_Release(gstate);
  }
}

PyObject *_attachsql_PoolObject_create_connection(_attachsql_PoolObject *self, PyObject *args)
{
  attachsql_error_t *error= NULL;
  _attachsql_ConnectionObject *con= NULL;
  PyObject *context= NULL;

  con= (_attachsql_ConnectionObject*) _attachsql_connect(NULL, args, NULL);
  if (!con)
  {
    return NULL;
  }
  context= PyTuple_New(2);
  PyTuple_SET_ITEM(context, 0, (PyObject*)con);
  PyTuple_SET_ITEM(context, 1, (PyObject*)self);
  attachsql_connect_set_callback(con->conn, _attachsql_callback, context);
  attachsql_pool_add_connection(self->pool, con->conn, &error);
  con->in_pool= true;
  PyList_Append(self->context_list, context);
  if (error)
  {
    Py_XDECREF(con);
    return NULL;
  }
  PyList_Append(self->conn_list, (PyObject*)con);
  return (PyObject*)con;
}

PyObject *_attachsql_PoolObject_run(_attachsql_PoolObject *self, PyObject *unused)
{
  attachsql_pool_run(self->pool);
  if (PyErr_Occurred())
  {
    return NULL;
  }
  Py_RETURN_NONE;
}

void _attachsql_PoolObject_dealloc(_attachsql_PoolObject *self)
{
  ssize_t listsize, listpos;
  PyObject_GC_UnTrack(self);
  listsize= PyList_Size(self->context_list);
  for (listpos= 0; listpos < listsize; listpos++)
  {
    Py_XDECREF(PyList_GetItem(self->context_list, listpos));
  }
  Py_XDECREF(self->context_list);

  listsize= PyList_Size(self->conn_list);
  for (listpos= 0; listpos < listsize; listpos++)
  {
    Py_XDECREF(PyList_GetItem(self->conn_list, listpos));
  }
  Py_XDECREF(self->conn_list);
  attachsql_pool_destroy(self->pool);
  self->ob_type->tp_free(self);
}

PyObject *_attachsql_PoolObject_getattr(_attachsql_PoolObject *self, char *name)
{
  PyObject *res;
  //struct PyMemberDef *def;

  res= Py_FindMethod(_attachsql_PoolObject_methods, (PyObject *)self, name);
  if (res != NULL)
  {
    return res;
  }
  PyErr_SetString(PyExc_AttributeError, name);
  return NULL;
}

int _attachsql_PoolObject_setattr(_attachsql_PoolObject *self, char *name, PyObject *v)
{
  //struct PyMemberDef *def;

  if (v == NULL)
  {
    PyErr_SetString(PyExc_AttributeError, "can't set attribute");
    return -1;
  }
  /* No members yet */
  /*
  for (def= _attachsql_ConnectionObject_memberlist; def->name != NULL; def++)
  {
    if (strcmp(def->name, name) == 0)
    {
      return PyMember_SetOne((char *)self, def, v);
    }
  }
  */
  PyErr_SetString(PyExc_AttributeError, name);
  return -1;
}

PyObject *_attachsql_PoolObject_repr(_attachsql_PoolObject *self)
{
  char buffer[300];
  snprintf(buffer, 300, "<attachsql.pool at %lx>", (long)self);
  return PyString_FromString(buffer);
}


int _attachsql_PoolObject_traverse(_attachsql_PoolObject *self, visitproc visit, void *arg)
{
  return 0;
}

int _attachsql_PoolObject_clear(_attachsql_PoolObject *self)
{
  return 0;
}

PyTypeObject _attachsql_PoolObject_Type= {
  PyObject_HEAD_INIT(NULL)
  0,
  "attachsql.pool",
  sizeof(_attachsql_PoolObject),
  0,
  (destructor)_attachsql_PoolObject_dealloc,
  0,
  (getattrfunc)_attachsql_PoolObject_getattr,
  (setattrfunc)_attachsql_PoolObject_setattr,
  0,
  (reprfunc)_attachsql_PoolObject_repr,

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
  "Returns a pool connection object",
  (traverseproc)_attachsql_PoolObject_traverse,
  (inquiry)_attachsql_PoolObject_clear,

  0,

  0,

  0,
  0,

  (struct PyMethodDef *)_attachsql_PoolObject_methods,
  0,

  0,
  0,
  0,
  0,
  0,
  0,
  (initproc)_attachsql_PoolObject_Initialize,
  NULL,
  NULL,
  NULL,
  0,
  0,
  0
};
