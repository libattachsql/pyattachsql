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
import gc

class QueryTest(unittest.TestCase):
    # TODO: execute parameters test
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

    def test_query_fetchall(self):
        con = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        cursor = con.cursor()
        cursor.execute("SHOW PROCESSLIST")
        rows = cursor.fetchall()
        self.assertGreater(len(rows), 0)
        for row in rows:
            self.assertGreaterEqual(len(row), 4)
            self.assertNotEqual(row[0], 0)

    def test_query_fetchmany_default(self):
        con = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        cursor = con.cursor()
        cursor.execute("SHOW PROCESSLIST")
        rows = cursor.fetchmany()
        self.assertEqual(len(rows), 1)
        for row in rows:
            self.assertGreaterEqual(len(row), 4)
            self.assertNotEqual(row[0], 0)

    def test_query_fetchmany_fixed(self):
        con = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        cursor = con.cursor()
        cursor.execute("SHOW PROCESSLIST")
        rows = cursor.fetchmany(5)
        self.assertLessEqual(len(rows), 5)
        for row in rows:
            self.assertGreaterEqual(len(row), 4)
            self.assertNotEqual(row[0], 0)

    def test_query_executemany(self):
        con = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        cursor = con.cursor()
        cursor.execute("DROP TABLE IF EXISTS dbapi_exec_many")
        cursor.close()
        cursor = con.cursor()
        cursor.execute("CREATE TABLE dbapi_exec_many (a int, b int)")
        cursor.close()
        gc.collect()
        ncon = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        ncursor = ncon.cursor()
        ncursor.executemany("INSERT INTO dbapi_exec_many VALUES (?,?)", [[1,2], [2,3],[3,4]])
        ncursor.close()
        cursor = con.cursor()
        cursor.execute("SELECT * FROM dbapi_exec_many")
        row = cursor.fetchone()
        self.assertEqual([1,2], row)
        row = cursor.fetchone()
        self.assertEqual([2,3], row)
        row = cursor.fetchone()
        self.assertEqual([3,4], row)

    def test_query_fail(self):
        con = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        cursor = con.cursor()
        with self.assertRaises(attachdb.errors.OperationalError):
            cursor.execute("SELECT BAD_QUERY")


