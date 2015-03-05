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

#include "connection.h"
#include "query.h"
#include "statement.h"

extern PyObject *_attachsql_ClientError;
extern PyTypeObject _attachsql_StatementObject_Type;
extern PyTypeObject _attachsql_QueryObject_Type;

static PyMethodDef _attachsql_ConnectionObject_methods[]= {
  {
    "connection_id",
    (PyCFunction)_attachsql_ConnectionObject_connection_id,
    METH_NOARGS,
    "Gets the current connection ID"
  },
  {
    "connect",
    (PyCFunction)_attachsql_ConnectionObject_connect,
    METH_NOARGS,
    "Explicitly connect to a server"
  },
  {
    "poll",
    (PyCFunction)_attachsql_ConnectionObject_poll,
    METH_NOARGS,
    "Poll a connection"
  },
  {
    "get_server_version",
    (PyCFunction)_attachsql_ConnectionObject_get_server_version,
    METH_NOARGS,
    "Return server version"
  },
  {
    "query",
    (PyCFunction)_attachsql_ConnectionObject_query,
    METH_VARARGS,
    "Send a query to the server"
  },
  {
    "set_ssl",
    (PyCFunction)_attachsql_ConnectionObject_set_ssl,
    METH_VARARGS,
    "Set the SSL connection parameters"
  },
  {
    "set_option",
    (PyCFunction)_attachsql_ConnectionObject_set_option,
    METH_VARARGS,
    "Sets connection options"
  },
  {
    "statement_prepare",
    (PyCFunction)_attachsql_ConnectionObject_statement_prepare,
    METH_VARARGS,
    "Start a prepared statement"
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
  char *user= NULL, *hn= NULL, *pass= NULL, *db= NULL;
  unsigned int port;
  static char *kwlist[]= {"host", "user", "pass", "db", "port", NULL};
  attachsql_error_t *error= NULL;

  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ssssI",
        kwlist,
        &hn, &user, &pass, &db, &port
        ))
  {
    return -1;
  }
  strncpy(self->host, hn, MAX_OPTION_SIZE);
  self->port= port;
  strncpy(self->user, user, MAX_OPTION_SIZE);
  strncpy(self->pass, pass, MAX_OPTION_SIZE);
  strncpy(self->db, db, MAX_OPTION_SIZE);
  self->in_group= false;
  self->query= NULL;

  self->conn= attachsql_connect_create(self->host, self->port, self->user, self->pass, self->db, &error);
  if (!self->conn)
  {
    _attachsql_Exception(error);
    return -1;
  }
  attachsql_connect_set_option(self->conn, ATTACHSQL_OPTION_MULTI_STATEMENTS, NULL);
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

PyObject *_attachsql_ConnectionObject_connect(_attachsql_ConnectionObject *self, PyObject *unused)
{
  bool status;
  attachsql_error_t *error= NULL;
  Py_BEGIN_ALLOW_THREADS
  status= attachsql_connect(self->conn, &error);
  Py_END_ALLOW_THREADS
  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  return PyBool_FromLong((long)status);
}

PyObject *_attachsql_ConnectionObject_poll(_attachsql_ConnectionObject *self, PyObject *unused)
{
  attachsql_return_t ret;
  attachsql_error_t *error= NULL;
  Py_BEGIN_ALLOW_THREADS
  ret= attachsql_connect_poll(self->conn, &error);
  Py_END_ALLOW_THREADS
  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }
  return PyInt_FromLong((long)ret);
}

PyObject *_attachsql_ConnectionObject_set_ssl(_attachsql_ConnectionObject *self, PyObject *args)
{
  char *key, *cert, *ca, *capath, *cipher;
  unsigned int verify= 0;
  attachsql_error_t *error= NULL;

  if (!PyArg_ParseTuple(args, "sssss|b", &key, &cert, &ca, &capath, &cipher, &verify))
  {
    return NULL;
  }

  attachsql_connect_set_ssl(self->conn, key, cert, ca, capath, cipher, verify, &error);

  if (error)
  {
    _attachsql_Exception(error);
    return NULL;
  }

  Py_RETURN_TRUE;
}

PyObject *_attachsql_ConnectionObject_set_option(_attachsql_ConnectionObject *self, PyObject *args)
{
  int option;
  bool result;
  PyObject *unused= NULL;

  if (!PyArg_ParseTuple(args, "i|O", &option, &unused))
  {
    return NULL;
  }
  // At a later date this third parameter should be based on the "unused" parameter
  result= attachsql_connect_set_option(self->conn, option, NULL);
  return PyBool_FromLong((long)result);
}

PyObject *_attachsql_ConnectionObject_get_server_version(_attachsql_ConnectionObject *self, PyObject *unused)
{
  const char *version;
  Py_BEGIN_ALLOW_THREADS
  version= attachsql_connect_get_server_version(self->conn);
  Py_END_ALLOW_THREADS
  return PyString_FromString(version);
}

PyObject *_attachsql_ConnectionObject_statement_prepare(_attachsql_ConnectionObject *self, PyObject *args)
{
  // TODO: if there is an active statement don't let us create a second
  _attachsql_StatementObject *stmt= NULL;

  stmt= (_attachsql_StatementObject*) _attachsql_StatementObject_Type.tp_alloc(&_attachsql_StatementObject_Type, 0);
  if (stmt == NULL)
  {
    return NULL;
  }
  stmt->pycon= self;
  stmt->stmt= NULL;
  Py_INCREF(self);
  if (_attachsql_StatementObject_Initialize(stmt, args, NULL))
  {
    Py_DECREF(stmt);
    stmt= NULL;
  }
  return (PyObject *) stmt;
}

PyObject *_attachsql_ConnectionObject_query(_attachsql_ConnectionObject *self, PyObject *args)
{
  // TODO: if there is an active query don't let us create a second
  _attachsql_QueryObject *query= NULL;
  
  if (self->query)
  {
    // TODO: may need is_closed flag?  Could well use this logic for the TODO above
    attachsql_query_close(self->conn);
    Py_XDECREF(self->query);
  }

  query= (_attachsql_QueryObject*) _attachsql_QueryObject_Type.tp_alloc(&_attachsql_QueryObject_Type, 0);
  if (query == NULL)
  {
    return NULL;
  }
  query->pycon= self;
  Py_INCREF(self);
  if (_attachsql_QueryObject_Initialize(query, args, NULL) || PyErr_Occurred())
  {
    Py_XDECREF(query);
    query= NULL;
  }
  if (self->in_group)
  {
    self->query= (PyObject*)query;
    Py_XINCREF(query);
  }
  return (PyObject *) query;
}

void _attachsql_ConnectionObject_dealloc(_attachsql_ConnectionObject *self)
{
  PyObject_GC_UnTrack(self);
  if (!self->in_group)
  {
    attachsql_connect_destroy(self->conn);
  }
  
  if (self->query)
  {
    Py_XDECREF(self->query);
    self->query= NULL;
  }
  self->ob_type->tp_free(self);
}

PyObject *_attachsql_ConnectionObject_getattr(_attachsql_ConnectionObject *self, char *name)
{
  PyObject *res;
  //struct PyMemberDef *def;

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

PyObject *_attachsql_ConnectionObject_repr(_attachsql_ConnectionObject *self)
{
  char buffer[300];
  snprintf(buffer, 300, "<attachsql.connection to '%.256s' port %d at %lx>", self->host, self->port, (long)self);
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
