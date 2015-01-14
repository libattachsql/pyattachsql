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

#include "group.h"

extern PyTypeObject _attachsql_QueryObject_Type;

static PyMethodDef _attachsql_GroupObject_methods[]= {
  {
    "create_connection",
    (PyCFunction)_attachsql_GroupObject_create_connection,
    METH_VARARGS,
    "Create a new connection in the group"
  },
  {
    "run",
    (PyCFunction)_attachsql_GroupObject_run,
    METH_NOARGS,
    "Run a connection group"
  },
  { NULL, NULL }
};

int _attachsql_GroupObject_Initialize(_attachsql_GroupObject *self, PyObject *args, PyObject *kwargs)
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
  self->group= attachsql_group_create(&error);
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
  _attachsql_GroupObject *self= (_attachsql_GroupObject*) PyTuple_GetItem(ctext_data, 1);
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

PyObject *_attachsql_GroupObject_create_connection(_attachsql_GroupObject *self, PyObject *args)
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
  attachsql_group_add_connection(self->group, con->conn, &error);
  con->in_group= true;
  PyList_Append(self->context_list, context);
  if (error)
  {
    Py_XDECREF(con);
    return NULL;
  }
  PyList_Append(self->conn_list, (PyObject*)con);
  return (PyObject*)con;
}

PyObject *_attachsql_GroupObject_run(_attachsql_GroupObject *self, PyObject *unused)
{
  attachsql_group_run(self->group);
  if (PyErr_Occurred())
  {
    return NULL;
  }
  Py_RETURN_NONE;
}

void _attachsql_GroupObject_dealloc(_attachsql_GroupObject *self)
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
  attachsql_group_destroy(self->group);
  self->ob_type->tp_free(self);
}

PyObject *_attachsql_GroupObject_getattr(_attachsql_GroupObject *self, char *name)
{
  PyObject *res;
  //struct PyMemberDef *def;

  res= Py_FindMethod(_attachsql_GroupObject_methods, (PyObject *)self, name);
  if (res != NULL)
  {
    return res;
  }
  PyErr_SetString(PyExc_AttributeError, name);
  return NULL;
}

int _attachsql_GroupObject_setattr(_attachsql_GroupObject *self, char *name, PyObject *v)
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

PyObject *_attachsql_GroupObject_repr(_attachsql_GroupObject *self)
{
  char buffer[300];
  snprintf(buffer, 300, "<attachsql.group at %lx>", (long)self);
  return PyString_FromString(buffer);
}


int _attachsql_GroupObject_traverse(_attachsql_GroupObject *self, visitproc visit, void *arg)
{
  return 0;
}

int _attachsql_GroupObject_clear(_attachsql_GroupObject *self)
{
  return 0;
}

PyTypeObject _attachsql_GroupObject_Type= {
  PyObject_HEAD_INIT(NULL)
  0,
  "attachsql.group",
  sizeof(_attachsql_GroupObject),
  0,
  (destructor)_attachsql_GroupObject_dealloc,
  0,
  (getattrfunc)_attachsql_GroupObject_getattr,
  (setattrfunc)_attachsql_GroupObject_setattr,
  0,
  (reprfunc)_attachsql_GroupObject_repr,

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
  "Returns a group connection object",
  (traverseproc)_attachsql_GroupObject_traverse,
  (inquiry)_attachsql_GroupObject_clear,

  0,

  0,

  0,
  0,

  (struct PyMethodDef *)_attachsql_GroupObject_methods,
  0,

  0,
  0,
  0,
  0,
  0,
  0,
  (initproc)_attachsql_GroupObject_Initialize,
  NULL,
  NULL,
  NULL,
  0,
  0,
  0
};
