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
from exceptions import process_exception

class Cursor(object):

    arraysize = 1
    rowcount = 0
    description = []
    first_row = True

    def __init__(self, con):
        self.con = con.con

    def callproc(self, name, *args):
        pass

    def close(self):
        del self.query

    def __del__(self):
        self.close()

    def execute(self, query, *args):
        try:
            self.query = self.con.query(query, *args)
            self.__poll_row_read()
        except Exception as e:
            process_exception(e)

    def __poll_row_read(self):
        ret = 0
        while ret not in [attachsql.RETURN_EOF, attachsql.RETURN_ROW_READY]:
            ret = self.con.poll()

        return ret

    def executemany(self, operation, sequence):
        pass

    def fetchone(self):
        try:
            if (not self.first_row):
                self.query.row_next()
            else:
                self.first_row = False
            ret = self.__poll_row_read()
            if ret == attachsql.RETURN_EOF:
                return None
            return self.query.row_get()
        except Exception as e:
            process_exception(e)

    def fetchmany(self, size=None):
        pass

    def fetchall(self):
        pass

    def nextset(self):
        pass

    def setinputsizes(self, sizes):
        pass

    def setoutputsize(self, size, column=None):
        pass

