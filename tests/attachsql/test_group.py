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

is_finished=0

def basic_callback(events, con, query, unused):
    global is_finished
    if events == attachsql.EVENT_EOF:
        is_finished = is_finished + 1
        return
    if events == attachsql.EVENT_ROW_READY:
        row = query.row_get()
        if row[0] <= 0:
            raise Exception("Bad connection ID in result")
        query.row_next()
        return
    if events != attachsql.EVENT_CONNECTED:
        raise Exception("Bad event", events)

def no_context_callback(events, con, query):
    global is_finished
    if events == attachsql.EVENT_EOF:
        is_finished = is_finished + 1
        return
    if events == attachsql.EVENT_ROW_READY:
        row = query.row_get()
        if row[0] <= 0:
            raise Exception("Bad connection ID in result")
        query.row_next()
        return
    if events != attachsql.EVENT_CONNECTED:
        raise Exception("Bad event", events)

class GroupTest(unittest.TestCase):
    def test_basic_query(self):
        global is_finished
        is_finished = 0
        group = attachsql.group(basic_callback, None)
        con1 = group.create_connection("localhost", "test", "test", "test", 3306)
        con2 = group.create_connection("localhost", "test", "test", "test", 3306)
        con3 = group.create_connection("localhost", "test", "test", "test", 3306)
        con1.query("SHOW PROCESSLIST")
        con2.query("SHOW PROCESSLIST")
        con3.query("SHOW PROCESSLIST")
        while is_finished < 3:
            try:
                group.run()
            except attachsql.ClientError:
                raise unittest.SkipTest("No MySQL server found")

    def test_no_context_query(self):
        global is_finished
        is_finished = 0
        group = attachsql.group(no_context_callback)
        con1 = group.create_connection("localhost", "test", "test", "test", 3306)
        con2 = group.create_connection("localhost", "test", "test", "test", 3306)
        con3 = group.create_connection("localhost", "test", "test", "test", 3306)
        con1.query("SHOW PROCESSLIST")
        con2.query("SHOW PROCESSLIST")
        con3.query("SHOW PROCESSLIST")
        while is_finished < 3:
            try:
                group.run()
            except attachsql.ClientError:
                raise unittest.SkipTest("No MySQL server found")
