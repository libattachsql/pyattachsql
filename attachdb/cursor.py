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
from attachdb.types import Date, Time, Timestamp, DateFromTicks, TimeFromTicks
from attachdb.types import TimestampFromTicks, Binary, STRING, BINARY, NUMBER
from attachdb.types import DATETIME, ROWID


class Cursor(object):

    arraysize = 1
    rowcount = 0
    description = []
    first_row = True
    query = None

    # TODO: if query hasn't been executed fetch show throw an error

    def __init__(self, con):
        self.con = con.con

    def callproc(self, name, parameters=None):
        if parameters is not None:
            query = "CALL {proc}({params})".format(proc=name, params=','.join(map(str, parameters)))
        else:
            query = "CALL {proc}()".format(proc=name)
        self.execute(query)

    def close(self):
        if self.query:
            self.__skip_remaining_rows()
            del self.query
            self.query = None

    def __del__(self):
        self.close()

    def execute(self, query, parameters=[]):
        query_args = []
        for parameter in parameters:
            if type(parameter) is int or type(parameter) is long or type(parameter) is NUMBER:
                if parameter > 2147483647 or parameter < -2147483648:
                    query_args.append({'type': attachsql.ESCAPE_TYPE_BIGINT, 'data': parameter})
                else:
                    query_args.append({'type': attachsql.ESCAPE_TYPE_INT, 'data': parameter})
            elif type(parameter) is float:
                query_args.append({'type': attachsql.ESCAPE_TYPE_DOUBLE, 'data': parameter})
            elif type(parameter) is None:
                query_args.append({'type': attachsql.ESCAPE_TYPE_NONE})
            elif type(parameter) is DATETIME or type(parameter) is Timestamp or type(parameter) is Date or type(parameter) is Time:
                query_args.append({'type': attachsql.ESCAPE_TYPE_CHAR, 'data': str(parameter)})
            else:
                query_args.append({'type': attachsql.ESCAPE_TYPE_CHAR, 'data': parameter})
        try:
            self.query = self.con.query(query, query_args)
            self.__poll_row_read()
        except Exception as e:
            process_exception(e)

    def __poll_row_read(self):
        while True:
            ret = self.con.poll()
            if ret in [attachsql.RETURN_EOF, attachsql.RETURN_ROW_READY]:
                break

        return ret

    def __skip_remaining_rows(self):
        ret = 0
        while ret != attachsql.RETURN_EOF:
            ret = self.con.poll()
            if ret == attachsql.RETURN_ROW_READY:
                self.query.row_next()

    def executemany(self, operation, sequence):
        #TODO at a later date for INSERT use VALUES as an optimisation
        for parameters in sequence:
            self.query = self.execute(operation, parameters)
            self.__skip_remaining_rows()

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
        ret = []
        if size is None:
            size = self.arraysize
        row_counter = 0
        while row_counter < size:
            row = self.fetchone()
            if row is None:
                break
            else:
                ret.append(row)
            row_counter += 1
        return ret

    def fetchall(self):
        ret = []
        while True:
            row = self.fetchone()
            if row is None:
                break
            else:
                ret.append(row)

        return ret

    def nextset(self):
        try:
            self.__skip_remaining_rows()
            ret = self.query.next_result()
            if ret == attachsql.RETURN_EOF:
                return None
            else:
                self.first_row = True
                self.__poll_row_read()
                return True
        except Exception as e:
            process_exception(e)

    def setinputsizes(self, sizes):
        # "Implementations are free to have this method do nothing" - PEP 249
        pass

    def setoutputsize(self, size, column=None):
        # "Implementations are free to have this method do nothing" - PEP 249
        pass

