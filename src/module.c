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

static PyMethodDef _attachsql_methods[] = {
  {
    "get_library_version",
    (PyCFunction)_attachsql_get_library_version,
    METH_NOARGS,
    "get_library_version() -- Returns a string representing the library version."
  }
};

PyMODINIT_FUNC
initattachsql(void)
{
  PyObject *module;

  module = Py_InitModule3("attachsql", _attachsql_methods, "a wrapper for the libAttachSQL C API");
  if (!module)
  {
    return;
  }
  /* TODO: add types */
}
