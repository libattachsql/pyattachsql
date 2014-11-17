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
#include "module.h"

int _attachsql_ConnectionObject_Initialize(_attachsql_ConnectionObject *self, PyObject *args, PyObject *kwargs);

PyObject *_attachsql_ConnectionObject_connection_id(_attachsql_ConnectionObject *self, PyObject *unused);

PyObject *_attachsql_ConnectionObject_connect(_attachsql_ConnectionObject *self, PyObject *unused);

PyObject *_attachsql_ConnectionObject_poll(_attachsql_ConnectionObject *self, PyObject *unused);

PyObject *_attachsql_ConnectionObject_get_server_version(_attachsql_ConnectionObject *self, PyObject *unused);

void _attachsql_ConnectionObject_dealloc(_attachsql_ConnectionObject *self);

PyObject *_attachsql_ConnectionObject_getattr(_attachsql_ConnectionObject *self, char *name);

int _attachsql_ConnectionObject_setattr(_attachsql_ConnectionObject *self, char *name, PyObject *v);

PyObject *_attachsql_ConnectionObject_repr(_attachsql_ConnectionObject *self);

int _attachsql_ConnectionObject_traverse(_attachsql_ConnectionObject *self, visitproc visit, void *arg);

int _attachsql_ConnectionObject_clear(_attachsql_ConnectionObject *self);
