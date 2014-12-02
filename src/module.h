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

#pragma once

#include <Python.h>
#include <libattachsql-1.0/attachsql.h>

#define MAX_OPTION_SIZE 60

typedef struct
{
  PyObject_HEAD
  attachsql_connect_t *conn;
  char host[MAX_OPTION_SIZE];
  in_port_t port;
  char user[MAX_OPTION_SIZE];
  char pass[MAX_OPTION_SIZE];
  char db[MAX_OPTION_SIZE];
} _attachsql_ConnectionObject;

typedef struct
{
  PyObject_HEAD
  _attachsql_ConnectionObject *pycon;
  char *stmt;
  PyObject *set_ref_store;
} _attachsql_StatementObject;

PyObject *_attachsql_get_library_version(PyObject *self, PyObject *unused);

PyObject *_attachsql_connect(PyObject *self, PyObject *args, PyObject *kwargs);

PyObject *_attachsql_Exception(attachsql_error_t *error);
