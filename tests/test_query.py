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

class QueryTest(unittest.TestCase):
    def test_basic_query(self):
        ret = 0
        con = attachsql.connect("localhost", "test", "test", "test", 3306)
        con.query("SHOW PROCESSLIST")
        try:
            while ret != attachsql.RETURN_ROW_READY:
                ret=con.poll()
        except attachsql.ClientError:
            raise unittest.SkipTest("No MySQL server found")
    def test_escape_query(self):
        ret = 0
        con = attachsql.connect("localhost", "test", "test", "test", 3306)
        con.query("SELECT ? as a, ? as b", [{'type': attachsql.ESCAPE_TYPE_CHAR, 'data':'hello'}, {'type': attachsql.ESCAPE_TYPE_INT, 'data': 123456}])
        try:
            while ret != attachsql.RETURN_ROW_READY:
                ret=con.poll()
        except attachsql.ClientError:
            raise unittest.SkipTest("No MySQL server found")

