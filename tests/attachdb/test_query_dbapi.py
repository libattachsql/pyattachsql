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
        cursor = con.cursor()
        cursor.executemany("INSERT INTO dbapi_exec_many VALUES (?,?)", [[1,2], [2,3],[3,4]])
        cursor.close()
        cursor = con.cursor()
        cursor.execute("SELECT * FROM dbapi_exec_many")
        row = cursor.fetchone()
        self.assertEqual(('1','2'), row)
        row = cursor.fetchone()
        self.assertEqual(('2','3'), row)
        row = cursor.fetchone()
        self.assertEqual(('3','4'), row)

    def test_query_fail(self):
        con = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        cursor = con.cursor()
        with self.assertRaises(attachdb.errors.OperationalError):
            cursor.execute("SELECT BAD_QUERY")

    def test_query_callproc(self):
        con = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        cursor = con.cursor()
        cursor.execute("DROP PROCEDURE IF EXISTS dbapi_multiply")
        cursor.close()
        cursor = con.cursor()
        cursor.execute("CREATE PROCEDURE dbapi_multiply (IN param1 INT, IN param2 INT, OUT param3 INT) BEGIN SET param3 := param1 * param2; END")
        cursor.close()
        cursor = con.cursor()
        cursor.callproc('dbapi_multiply', ('3', '2', '@dbout'))
        cursor.close()
        cursor = con.cursor()
        cursor.execute("SELECT @dbout")
        row = cursor.fetchone()
        self.assertEqual(('6',), row)
        cursor.close()

    def test_query_callproc_noparams(self):
        con = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        cursor = con.cursor()
        cursor.execute("DROP PROCEDURE IF EXISTS dbapi_static")
        cursor.close()
        cursor = con.cursor()
        cursor.execute("CREATE PROCEDURE dbapi_static () BEGIN SELECT 1 as a, 2 as b; END")
        cursor.close()
        cursor = con.cursor()
        cursor.callproc('dbapi_static')
        row = cursor.fetchone()
        self.assertEqual(('1', '2'), row)
        cursor.close()

    def test_query_nextset(self):
        con = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        cursor = con.cursor()
        cursor.execute("SELECT 1 as a; SELECT 2 as b;")
        row = cursor.fetchone()
        self.assertEqual(('1',), row)
        self.assertTrue(cursor.nextset())
        row = cursor.fetchone()
        self.assertEqual(('2',), row)
        cursor.close()

    def test_query_nextset_fail(self):
        con = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        cursor = con.cursor()
        cursor.execute("SELECT 1 as a")
        row = cursor.fetchone()
        self.assertEqual(('1',), row)
        self.assertIsNone(cursor.nextset())
        cursor.close()

    def test_query_datetime(self):
        con = attachdb.Connection(host="localhost", user="test", password="test", database="test", port=3306)
        ts = attachdb.TimestampFromTicks(1425509750)
        cursor = con.cursor()
        cursor.execute("SELECT ? as ts", [ts])
        row = cursor.fetchone()
        self.assertEqual(('2015-03-04 22:55:50',), row)
        cursor.close()
