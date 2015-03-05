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

from time import localtime
from datetime import date, datetime, time

Date = date
Time = time
Timestamp = datetime

def DateFromTicks(ticks):
    return date.fromtimestamp(ticks)

def TimeFromTicks(ticks):
    ts = localtime(ticks)
    return time(ts.tm_hour, ts.tm_min, ts.tm_sec)

def TimestampFromTicks(ticks):
    return datetime.fromtimestamp(ticks)

def Binary(string):
    return str(string)

STRING = str
BINARY = str
NUMBER = int
DATETIME = Timestamp
ROWID = int
