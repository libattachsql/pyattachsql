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

int _attachsql_QueryObject_Initialize(_attachsql_QueryObject *self, PyObject *args, PyObject *kwargs);

int _attachsql_QueryObject_clear(_attachsql_QueryObject *self);

int _attachsql_QueryObject_traverse(_attachsql_QueryObject *self, visitproc visit, void *arg);

PyObject *_attachsql_QueryObject_repr(_attachsql_QueryObject *self);

int _attachsql_QueryObject_setattr(_attachsql_QueryObject *self, char *name, PyObject *v);

PyObject *_attachsql_QueryObject_getattr(_attachsql_QueryObject *self, char *name);

void _attachsql_QueryObject_dealloc(_attachsql_QueryObject *self);

PyObject *_attachsql_QueryObject_column_count(_attachsql_QueryObject *self, PyObject *unused);

PyObject *_attachsql_QueryObject_row_get(_attachsql_QueryObject *self, PyObject *unused);

PyObject *_attachsql_QueryObject_row_next(_attachsql_QueryObject *self, PyObject *unused);

PyObject *_attachsql_QueryObject_last_insert_id(_attachsql_QueryObject *self, PyObject *unused);

PyObject *_attachsql_QueryObject_affected_rows(_attachsql_QueryObject *self, PyObject *unused);

PyObject *_attachsql_QueryObject_warning_count(_attachsql_QueryObject *self, PyObject *unused);

PyObject *_attachsql_QueryObject_info(_attachsql_QueryObject *self, PyObject *unused);

PyObject *_attachsql_QueryObject_row_count(_attachsql_QueryObject *self, PyObject *unused);

PyObject *_attachsql_QueryObject_next_result(_attachsql_QueryObject *self, PyObject *unused);

PyObject *_attachsql_QueryObject_buffer_rows(_attachsql_QueryObject *self, PyObject *unused);

PyObject *_attachsql_QueryObject_buffer_row_get(_attachsql_QueryObject *self, PyObject *unused);

PyObject *_attachsql_QueryObject_row_get_offset(_attachsql_QueryObject *self, PyObject *args);
