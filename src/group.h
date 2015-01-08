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

int _attachsql_GroupObject_Initialize(_attachsql_GroupObject *self, PyObject *args, PyObject *kwargs);

void _attachsql_callback(attachsql_connect_t *con, attachsql_events_t events, void *context, attachsql_error_t *error);

PyObject *_attachsql_GroupObject_create_connection(_attachsql_GroupObject *self, PyObject *args);

PyObject *_attachsql_GroupObject_run(_attachsql_GroupObject *self, PyObject *unused);

void _attachsql_GroupObject_dealloc(_attachsql_GroupObject *self);

int _attachsql_GroupObject_setattr(_attachsql_GroupObject *self, char *name, PyObject *v);

PyObject *_attachsql_GroupObject_repr(_attachsql_GroupObject *self);

int _attachsql_GroupObject_traverse(_attachsql_GroupObject *self, visitproc visit, void *arg);

int _attachsql_GroupObject_clear(_attachsql_GroupObject *self);
