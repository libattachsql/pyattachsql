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

class QueryTest(unittest.TestCase):
    def test_query_fetchone(self):
        con = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        cursor = con.cursor()
        cursor.execute("SHOW PROCESSLIST")
        row = []
        while row is not None:
            row = cursor.fetchone()
            if row:
                self.assertGreaterEqual(len(row), 4)
                self.assertNotEqual(row[0], 0)

    def test_query_fail(self):
        con = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        cursor = con.cursor()
        with self.assertRaises(attachdb.errors.OperationalError):
            cursor.execute("SELECT BAD_QUERY")


