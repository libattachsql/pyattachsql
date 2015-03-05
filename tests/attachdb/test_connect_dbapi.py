# Copyright 2014 Hewlett-Packard Development Company, L.P.
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may
# not use this file except in compliance with the License. You may obtain
# a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations
# under the License.

import unittest
import attachdb

class ConnectTest(unittest.TestCase):
    def test_function_connect(self):
        con = attachdb.connect(host="localhost", user="test", password="test", database="test", port=3306)
        self.assertNotEqual(con.connection_id, 0)

    def test_connect(self):
        con = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        self.assertNotEqual(con.connection_id, 0)

    def test_connect_fail(self):
        with self.assertRaises(attachdb.errors.ProgrammingError):
            con = attachdb.Connection(host="badhost", user="test", password="test", database="test", port=3306)
