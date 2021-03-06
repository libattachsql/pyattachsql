# Copyright 2015 Hewlett-Packard Development Company, L.P.
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

import attachsql
from cursor import Cursor
from exceptions import process_exception, OperationalError

class Connection(object):
    connected = False

    def __init__(self, host, port=3306, user='', password='', database='',
                 autocommit=False):
        """
        host - string, hostname
        port - integer, port number
        user - string, username
        password - string, password
        database - string, default database
        autocommit - boolean, enable/disable autocommit
                     (None for server default)
        """

        self.con = attachsql.connect(host, user, password, database, port)
        try:
            self.con.connect()
            self.__poll_connect_loop()
        except Exception as e:
            process_exception(e)
        self.connected = True
        if autocommit is not None:
            self.autocommit(autocommit)
        self.connection_id = self.con.connection_id()

    def __poll_connect_loop(self):
        ret = 0
        while ret != attachsql.RETURN_EOF:
            ret = self.con.poll()

    def autocommit(self, setting):
        if not self.connected:
            raise OperationalError("Not connected to database server")
        try:
            if setting == True:
                query = self.con.query("SET autocommit = 1")
            else:
                query = self.con.query("SET autocommit = 0")

            self.__poll_no_data_loop(query)
        except Exception as e:
            process_exception(e)

    def __poll_no_data_loop(self, query):
        ret = 0
        while ret != attachsql.RETURN_EOF:
            ret = self.con.poll()
            if ret == attachsql.RETURN_ROW_READY:
                query.row_next()

    def commit(self):
        if not self.connected:
            raise OperationalError("Not connected to database server")
        try:
            query = self.con.query("COMMIT")
            self.__poll_no_data_loop(query)
        except Exception as e:
            process_exception(e)

    def rollback(self):
        if not self.connected:
            raise OperationalError("Not connected to database server")
        try:
            query = self.con.query("ROLLBACK")
            self.__poll_no_data_loop(query)
        except Exception as e:
            process_exception(e)

    def close(self):
        del self.con
        self.connected = False

    def __del__(self):
        self.close()

    def cursor(self):
        if not self.connected:
            raise OperationalError("Not connected to database server")
        return Cursor(self)
