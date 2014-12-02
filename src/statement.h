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

int _attachsql_StatementObject_Initialize(_attachsql_StatementObject *self, PyObject *args, PyObject *kwargs);

PyObject *_attachsql_StatementObject_execute(_attachsql_StatementObject *self, PyObject *unused);

void _attachsql_StatementObject_dealloc(_attachsql_StatementObject *self);

PyObject *_attachsql_StatementObject_getattr(_attachsql_StatementObject *self, char *name);

int _attachsql_StatementObject_setattr(_attachsql_StatementObject *self, char *name, PyObject *v);

PyObject *_attachsql_StatementObject_repr(_attachsql_StatementObject *self);

int _attachsql_StatementObject_traverse(_attachsql_StatementObject *self, visitproc visit, void *arg);

int _attachsql_StatementObject_clear(_attachsql_StatementObject *self);

PyObject *_attachsql_StatementObject_reset(_attachsql_StatementObject *self, PyObject *unused);

PyObject *_attachsql_StatementObject_send_long_data(_attachsql_StatementObject *self, PyObject *args);

PyObject *_attachsql_StatementObject_param_count(_attachsql_StatementObject *self, PyObject *unused);

PyObject *_attachsql_StatementObject_set_int(_attachsql_StatementObject *self, PyObject *args);

PyObject *_attachsql_StatementObject_set_float(_attachsql_StatementObject *self, PyObject *args);

PyObject *_attachsql_StatementObject_set_string(_attachsql_StatementObject *self, PyObject *args);

PyObject *_attachsql_StatementObject_set_null(_attachsql_StatementObject *self, PyObject *args);

PyObject *_attachsql_StatementObject_set_datetime(_attachsql_StatementObject *self, PyObject *args);

PyObject *_attachsql_StatementObject_set_time(_attachsql_StatementObject *self, PyObject *args);

PyObject *_attachsql_StatementObject_row_get(_attachsql_StatementObject *self, PyObject *unused);

PyObject *_attachsql_StatementObject_get_int(_attachsql_StatementObject *self, PyObject *args);

PyObject *_attachsql_StatementObject_get_bigint(_attachsql_StatementObject *self, PyObject *args);

PyObject *_attachsql_StatementObject_get_float(_attachsql_StatementObject *self, PyObject *args);

PyObject *_attachsql_StatementObject_get_char(_attachsql_StatementObject *self, PyObject *args);

PyObject *_attachsql_StatementObject_get_column_type(_attachsql_StatementObject *self, PyObject *args);

PyObject *_attachsql_StatementObject_row_next(_attachsql_StatementObject *self, PyObject *unused);

PyObject *_attachsql_StatementObject_get_column_count(_attachsql_StatementObject *self, PyObject *unused);
