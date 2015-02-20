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
import attachsql
import datetime

class StatementTest(unittest.TestCase):
    def test_no_param(self):
        ret = 0
        con = attachsql.connect("localhost", "test", "test", "test", 3306)
        stmt = con.statement_prepare("SHOW PROCESSLIST")
        try:
            while ret != attachsql.RETURN_EOF:
                ret = con.poll()
        except attachsql.ClientError as e:
            if e[0] == 2002:
                raise unittest.SkipTest("No MySQL server found")
            else:
                raise e
        stmt.execute()
        ret= 0
        while ret != attachsql.RETURN_EOF:
            ret = con.poll()
            if ret == attachsql.RETURN_ROW_READY:
                self.assertNotEqual(stmt.get_column_count(), 0)
                stmt.row_get()
                uid= stmt.get_int(0)
                user= stmt.get_char(1)
                self.assertNotEqual(uid, 0)
                self.assertNotEqual(user, "")
                stmt.row_next()

    def test_param(self):
        ret = 0
        con = attachsql.connect("localhost", "test", "test", "test", 3306)
        stmt = con.statement_prepare("SELECT ? as a, ? as b, CONVERT_TZ(FROM_UNIXTIME(1196440219),@@session.time_zone,'+00:00') as c")
        try:
            while ret != attachsql.RETURN_EOF:
                ret = con.poll()
        except attachsql.ClientError as e:
            if e[0] == 2002:
                raise unittest.SkipTest("No MySQL server found")
            else:
                raise e
        stmt.set_string(0, "hello world")
        stmt.set_int(1, 123456)
        stmt.execute()
        ret = 0
        while ret != attachsql.RETURN_EOF:
            ret = con.poll()
            if ret == attachsql.RETURN_ROW_READY:
                self.assertNotEqual(stmt.get_column_count(), 0)
                stmt.row_get()
                self.assertEqual(stmt.get_char(0), "hello world")
                self.assertEqual(stmt.get_char(1), "123456")
                self.assertEqual(stmt.get_int(1), 123456)
                self.assertEqual(stmt.get_char(2), "2007-11-30 16:30:19")
                stmt.row_next()

    def test_statement_reuse(self):
        ret = 0
        con = attachsql.connect("localhost", "test", "test", "test", 3306)
        stmt = con.statement_prepare("SELECT ? as a, ? as b, CONVERT_TZ(FROM_UNIXTIME(1196440219),@@session.time_zone,'+00:00') as c")
        try:
            while ret != attachsql.RETURN_EOF:
                ret = con.poll()
        except attachsql.ClientError as e:
            if e[0] == 2002:
                raise unittest.SkipTest("No MySQL server found")
            else:
                raise e
        stmt.set_string(0, "hello world")
        stmt.set_int(1, 123456)
        stmt.execute()
        ret = 0
        while ret != attachsql.RETURN_EOF:
            ret = con.poll()
            if ret == attachsql.RETURN_ROW_READY:
                self.assertNotEqual(stmt.get_column_count(), 0)
                stmt.row_get()
                self.assertEqual(stmt.get_char(0), "hello world")
                self.assertEqual(stmt.get_char(1), "123456")
                self.assertEqual(stmt.get_int(1), 123456)
                self.assertEqual(stmt.get_char(2), "2007-11-30 16:30:19")
                stmt.row_next()
        stmt.set_string(0, "dlrow olleh")
        stmt.set_int(1, 654321)
        stmt.execute()
        ret = 0
        while ret != attachsql.RETURN_EOF:
            ret = con.poll()
            if ret == attachsql.RETURN_ROW_READY:
                self.assertNotEqual(stmt.get_column_count(), 0)
                stmt.row_get()
                self.assertEqual(stmt.get_char(0), "dlrow olleh")
                self.assertEqual(stmt.get_char(1), "654321")
                self.assertEqual(stmt.get_int(1), 654321)
                self.assertEqual(stmt.get_char(2), "2007-11-30 16:30:19")
                stmt.row_next()

    def test_statement_twice(self):
        ret = 0
        con = attachsql.connect("localhost", "test", "test", "test", 3306)
        stmt = con.statement_prepare("SELECT ? as a, ? as b, CONVERT_TZ(FROM_UNIXTIME(1196440219),@@session.time_zone,'+00:00') as c")
        try:
            while ret != attachsql.RETURN_EOF:
                ret = con.poll()
        except attachsql.ClientError as e:
            if e[0] == 2002:
                raise unittest.SkipTest("No MySQL server found")
            else:
                raise e
        stmt.set_string(0, "hello world")
        stmt.set_int(1, 123456)
        stmt.execute()
        ret = 0
        while ret != attachsql.RETURN_EOF:
            ret = con.poll()
            if ret == attachsql.RETURN_ROW_READY:
                self.assertNotEqual(stmt.get_column_count(), 0)
                stmt.row_get()
                self.assertEqual(stmt.get_char(0), "hello world")
                self.assertEqual(stmt.get_char(1), "123456")
                self.assertEqual(stmt.get_int(1), 123456)
                self.assertEqual(stmt.get_char(2), "2007-11-30 16:30:19")
                stmt.row_next()
        del(stmt)
        stmt = con.statement_prepare("SELECT ? as a, ? as b, CONVERT_TZ(FROM_UNIXTIME(1196440219),@@session.time_zone,'+00:00') as c")
        ret = 0
        try:
            while ret != attachsql.RETURN_EOF:
                ret = con.poll()
        except attachsql.ClientError as e:
            if e[0] == 2002:
                raise unittest.SkipTest("No MySQL server found")
            else:
                raise e
        datestamp = datetime.datetime(2014, 10, 6, 21, 50, 20, 324560)
        stmt.set_datetime(1, datestamp)
        stmt.set_float(0, 3.14159)
        stmt.execute()
        ret = 0
        while ret != attachsql.RETURN_EOF:
            ret = con.poll()
            if ret == attachsql.RETURN_ROW_READY:
                self.assertNotEqual(stmt.get_column_count(), 0)
                stmt.row_get()
                self.assertEqual(stmt.get_char(1), "2014-10-06 21:50:20.324560")
                self.assertEqual(stmt.get_char(0)[0:7], "3.14159")
                self.assertEqual(stmt.get_float(0), 3.14159)
                stmt.row_next()
