DB API Cursor Class
===================

.. py:module:: attachdb

.. py:class:: Cursor

   .. py:attribute:: arraysize

      An integer representing the number of rows to be returned by :py:meth:`fetchmany` if not specified by the function call.  Defaults to ``1``

   .. py:method:: callproc(name, parameters=None)

      Call a stored procedure

      :param str name: the name of the stored procedure
      :param tuple parameters: parameters for the stored procedure

   .. py:method:: close()

      Close the cursor.  This will read through to the end of the result set first if there is one active

   .. py:method:: execute(query, parameters=[])

      Execute a query with optional prepared statement style parameters.  Parameter markers should be question marks (``?``).

      :param str query: the query to execute
      :param list parameters: parameters for the query

   .. py:method:: executemany(operation, sequence)

      Execute a query multiple times with each of a sequences of parameters.  For use with ``INSERT`` or ``UPDATE`` queries.

      :param str operation: the query to execute
      :param list sequence: a list containing lists of parameters

   .. py:method:: fetchone()

      Fetch the next result from the result set.  Returns ``None`` if there are now rows left.

      :return: The next row in the result set (or ``None``)
      :rtype: tuple

   .. py:method:: fetchmany(size=None)

      Fetch many rows from a result set.  The maximum amount to return is set by the size parameter or :py:attr:`arraysize` if this is not set.

      :param int size: the amount of rows to return
      :return: a list of row tuples
      :rtype: list

   .. py:method:: fetchall()

      Fetch all the rows in a result set.

      :return: a list of row tuples
      :rtype: list

   .. py:method:: nextset()

      Start receiving the next result set in a multiple result set query

      :return: ``True`` if successful, ``None`` is there is no more result sets
      :rtype: boolean

   .. py:method:: setinputsizes(sizes)

      Not implemented

   .. py:method:: setoutputsizes(size, column=None)

      Not implemented
