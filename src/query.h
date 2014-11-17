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

PyObject *_attachsql_ConnectionObject_query(_attachsql_ConnectionObject *self, PyObject *args);

PyObject *_attachsql_ConnectionObject_query_close(_attachsql_ConnectionObject *self, PyObject *unused);

PyObject *_attachsql_ConnectionObject_query_column_count(_attachsql_ConnectionObject *self, PyObject *unused);

PyObject *_attachsql_ConnectionObject_query_row_get(_attachsql_ConnectionObject *self, PyObject *unused);

PyObject *_attachsql_ConnectionObject_query_row_next(_attachsql_ConnectionObject *self, PyObject *unused);

PyObject *_attachsql_ConnectionObject_last_insert_id(_attachsql_ConnectionObject *self, PyObject *unused);
