pyAttachSQL Module Basics
=========================

.. py:module:: attachsql

Exceptions
----------

.. py:exception:: ClientError

   An exception on the client side

.. py:exception:: ServerError

   As exception on the server side

Constants
---------

.. _return-constants:

Return Constants
^^^^^^^^^^^^^^^^

.. py:data:: RETURN_NONE

   No return status

.. py:data:: RETURN_NOT_CONNECTED

   The client is not yet connected to the server

.. py:data:: RETURN_PROCESSING

   Data processing / transfer is in-progress

.. py:data:: RETURN_ROW_DATA

   A row is in the buffer ready for processing

.. py:data:: RETURN_ERROR

   An error occurred on the connection

.. py:data:: RETURN_EOF

   A query EOF, typically all rows have been returned

.. _escape-constants:

Escape Constants
^^^^^^^^^^^^^^^^

.. py:data:: ESCAPE_TYPE_NONE

   Nothing to escape

.. py:data:: ESCAPE_TYPE_CHAR

   Escape string data adding quote marks around the string

.. py:data:: ESCAPE_TYPE_CHAR_LIKE

   Escape string data without quote marks around the string (for use in LIKE syntax)

.. py:data:: ESCAPE_TYPE_INT

   Insert integer data into the query

.. py:data:: ESCAPE_TYPE_BIGINT

   Insert 64bit integer data into the query

.. py:data:: ESCAPE_TYPE_FLOAT

   Insert float data into the query

.. py:data:: ESCAPE_TYPE_DOUBLE

   Insert double data into the query

.. _option-constants:

Option Constants
^^^^^^^^^^^^^^^^

.. py:data:: OPTION_COMPRESS

   Protocol compression option

.. py:data:: OPTION_FOUND_ROWS

   Found rows counter for results

.. py:data:: OPTION_IGNORE_SIGPIPE

   Ignore sigpipe (not used, sigpipe is ignored anyway)

.. py:data:: OPTION_INTERACTIVE

   Client is interactive

.. py:data:: OPTION_LOCAL_FILES

   Enable ``LOAD LOCAL INFILE``

.. py:data:: OPTION_MULTI_STATEMENTS

   Enable multi-statement queries

.. py:data:: OPTION_NO_SCHEMA

   Disable the ``schema_name.table_name.column_name`` syntax (for ODBC)

.. py:data:: OPTION_SSL_NO_VERIFY

   Currently unused

.. py:data:: OPTION_SEMI_BLOCKING

   Enable semi-blocking mode

.. _type-constants:

Column Type Constants
^^^^^^^^^^^^^^^^^^^^^

.. py:data:: COLUMN_TYPE_DECIMAL

.. py:data:: COLUMN_TYPE_TINY

.. py:data:: COLUMN_TYPE_SHORT

.. py:data:: COLUMN_TYPE_LONG

.. py:data:: COLUMN_TYPE_FLOAT

.. py:data:: COLUMN_TYPE_DOUBLE

.. py:data:: COLUMN_TYPE_NULL

.. py:data:: COLUMN_TYPE_TIMESTAMP

.. py:data:: COLUMN_TYPE_LONGLONG

.. py:data:: COLUMN_TYPE_INT24

.. py:data:: COLUMN_TYPE_DATE

.. py:data:: COLUMN_TYPE_TIME

.. py:data:: COLUMN_TYPE_DATETIME

.. py:data:: COLUMN_TYPE_YEAR

.. py:data:: COLUMN_TYPE_VARCHAR

.. py:data:: COLUMN_TYPE_BIT

.. py:data:: COLUMN_TYPE_NEWDECIMAL

.. py:data:: COLUMN_TYPE_ENUM

.. py:data:: COLUMN_TYPE_SET

.. py:data:: COLUMN_TYPE_TINY_BLOB

.. py:data:: COLUMN_TYPE_MEDIUM_BLOB

.. py:data:: COLUMN_TYPE_LONG_BLOB

.. py:data:: COLUMN_TYPE_BLOB

.. py:data:: COLUMN_TYPE_VARSTRING

.. py:data:: COLUMN_TYPE_STRING

.. py:data:: COLUMN_TYPE_GEOMETRY

.. _event-constants:

Callback Event Constants
------------------------

.. py:data:: EVENT_CONNECTED

.. py:data:: EVENT_ERROR

.. py:data:: EVENT_EOF

.. py:data:: EVENT_ROW_READY

Functions
---------

.. py:function:: connect(hostname, user, password, database, port)

   :param str hostname: The hostname to the server
   :param str user: The user name to connect with
   :param str password: The password to connect with
   :param str database: The default database for the connection
   :param int port: The port to connect on or ``0`` for a Unix Domain Socket connection
   :return: An instance of the :py:class:`connection`

.. py:function:: get_library_version()

   Gets the version of libAttachSQL used for pyAttachSQL

   :return: A string representation of the version number
   :rtype: str

Callback Function Prototypes
----------------------------

.. py:function:: my_callback(events, con, query, unused):

   A user defined callback used for :py:class:`group`

   :param int events: The event which triggered the callback from :ref:`event-constants`
   :param object con: The connection object which triggered the callback
   :param object query: The query object which triggered the callback
   :param object context: The user supplied context
