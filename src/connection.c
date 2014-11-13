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

static PyMethodDef _attachsql_ConnectionObject_methods[]= {
  {
    "connection_id",
    (PyCFunction)_attachsql_ConnectionObject_connection_id,
    METH_NOARGS,
    "Gets the current connection ID"
  },
  {NULL, NULL}
};

/*
static struct PyMemberDef _attachsql_ConnectionObject_memberlist[]= {
  {NULL}
};
*/

int _attachsql_ConnectionObject_Initialize(_attachsql_ConnectionObject *self, PyObject *args, PyObject *kwargs)
{
  char *host= NULL, *user= NULL, *pass= NULL, *db= NULL;
  in_port_t port;
  static char *kwlist[]= {"host", "user", "pass", "db", "port", NULL};
  attachsql_error_t *error= NULL;

  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|ssssi:connect",
        kwlist,
        &host, &user, &pass, &db, &port
        ))
  {
    return -1;
  }
  self->conn= attachsql_connect_create(host, port, user, pass, db, &error);
  if (!self->conn)
  {
    _attachsql_Exception(error);
    return -1;
  }
  return 0;
}

PyObject *_attachsql_ConnectionObject_connection_id(_attachsql_ConnectionObject *self, PyObject *unused)
{
  uint32_t conn_id;

  Py_BEGIN_ALLOW_THREADS
  conn_id= attachsql_connect_get_connection_id(self->conn);
  Py_END_ALLOW_THREADS
  return PyInt_FromLong((long)conn_id);
}

void _attachsql_ConnectionObject_dealloc(_attachsql_ConnectionObject *self)
{
  PyObject_GC_UnTrack(self);
  attachsql_connect_destroy(self->conn);
  self->ob_type->tp_free(self);
}

PyObject *_attachsql_ConnectionObject_getattr(_attachsql_ConnectionObject *self, char *name)
{
  PyObject *res;
  struct PyMemberDef *def;

  res= Py_FindMethod(_attachsql_ConnectionObject_methods, (PyObject *)self, name);
  if (res != NULL)
  {
    return res;
  }
  PyErr_SetString(PyExc_AttributeError, name);
  return NULL;
}

int _attachsql_ConnectionObject_setattr(_attachsql_ConnectionObject *self, char *name, PyObject *v)
{
  struct PyMemberDef *def;

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

PyObject *_attachsql_ConnectionObject_repr(_attachsql_ConnectionObject *self)
{
  char buffer[300];
  /* TODO: store the host name / port for this */
  //snprintf(buffer, 300, "<attachsql.connection to '%.256s' port %d at %lx>", self->conn->con->host, self->conn->con->port);
  snprintf(buffer, 300, "<attachsql.connection at %lx>", (long)self);
  return PyString_FromString(buffer);
}

int _attachsql_ConnectionObject_traverse(_attachsql_ConnectionObject *self, visitproc visit, void *arg)
{
  return 0;
}

int _attachsql_ConnectionObject_clear(_attachsql_ConnectionObject *self)
{
  return 0;
}

PyTypeObject _attachsql_ConnectionObject_Type= {
  PyObject_HEAD_INIT(NULL)
  0,
  "attachsql.connection",
  sizeof(_attachsql_ConnectionObject),
  0,
  (destructor)_attachsql_ConnectionObject_dealloc,
  0,
  (getattrfunc)_attachsql_ConnectionObject_getattr,
  (setattrfunc)_attachsql_ConnectionObject_setattr,
  0,
  (reprfunc)_attachsql_ConnectionObject_repr,

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
  "Returns an AttachSQL connection object",
  (traverseproc)_attachsql_ConnectionObject_traverse,
  (inquiry)_attachsql_ConnectionObject_clear,

  0,

  0,

  0,
  0,

  (struct PyMethodDef *)_attachsql_ConnectionObject_methods,
  //(struct PyMemberDef *)_attachsql_ConnectionObject_memberlist,
  0,

  0,
  0,
  0,
  0,
  0,
  0,
  (initproc)_attachsql_ConnectionObject_Initialize,
  NULL,
  NULL,
  NULL,
  0,
  0,
  0
};