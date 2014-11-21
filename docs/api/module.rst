Module Basics
=============

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

.. py:data:: RETURN_IDLE

   The connection is idle

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

Functions
---------

.. py:function:: connect(hostname, user, password, database, port)

   :param str hostname: The hostname to the server
   :param str user: The user name to connect with
   :param str password: The password to connect with
   :param str database: The default database for the connection
   :param int port: The port to connect on or ``0`` for a Unix Domain Socket connection
   :return: An instance of the :py:class:`connection`
