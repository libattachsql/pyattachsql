Prepared Statement Class
========================

.. py:module:: attachsql

.. py:class:: statement

   .. py:method:: execute()

      Start execution a prepared statement

      :return: True on success
      :rtype: boolean

   .. py:method:: reset()

      Reset a prepared statement

      :return: True on success
      :rtype: boolean

   .. py:method:: send_long_data(param_no, data)

      Send a long data packet as a server parameter

      :param int param_no: The parameter to set (starting from 0)
      :param str data: The data to set
      :return: True on success
      :rtype: boolean

   .. py:method:: param_count()

      The number of parameters in the query to be set

      :return: The number of parameters
      :rtype: int

   .. py:method:: set_int(param_no, data, [is_unsigned])

      Sets a parameter as an integer

      :param int param_no: The parameter to set (starting from 0)
      :param int data: The data to set
      :param boolean is_unsigned: Whether or not the data is unsigned (False by default)
      :return: True on success
      :rtype: boolean

   .. py:method:: set_bigint(param_no, data, [is_unsigned])

      Sets a parameter as a bigint

      :param int param_no: The parameter to set (starting from 0)
      :param longlong data: The data to set
      :param boolean is_unsigned: Whether or not the data is unsigned (False by default)
      :return: True on success
      :rtype: boolean

   .. py:method:: set_float(param_no, data)

      Sets a double precision floating point number

      :param int param_no: The parameter to set (starting from 0)
      :param float data: The data to set
      :return: True on success
      :rtype: boolean

   .. py:method:: set_string(param_no, data)

      Sets a string parameter

      :param int param_no: The parameter to set (starting from 0)
      :param str data: The data to set
      :return: True on success
      :rtype: boolean

   .. py:method:: set_null(param_no)

      Sets a parameter to NULL

      :param int param_no: The parameter to set (starting from 0)
      :return: True on success
      :rtype: boolean

   .. py:method:: set_datetime(param_no, data)

      Sets a parameter to a datetime.datetime or datetime.date object

      :param int param_no: The parameter to set (starting from 0)
      :param datetime data: The date / datetime to set
      :return: True on success
      :rtype: boolean

   .. py:method:: set_time(param_no, data)

      Sets a parameter to a datetime.time object

      :param int param_no: The parameter to set (starting from 0)
      :param time data: The time to set
      :return: True on success
      :rtype: boolean

   .. py:method:: row_get()

      Retrieve a ready row from the buffer

      :return: True on success
      :rtype: boolean

   .. py:method:: get_int(column_no, [get_unsigned])

      Get an int from the row

      :param int column_no: The column number to get (starting from 0)
      :param boolean get_unsigned: Whether or not to get the data as unsigned (False by default)
      :return: The integer data
      :rtype: long

   .. py:method:: get_bigint(column_no, [get_unsigned])

      Get a bigint from the row

      :param int column_no: The column number to get (starting from 0)
      :param boolean get_unsigned: Whether or not to get the data as unsigned (False by default)
      :return: The bigint data
      :rtype: longlong

   .. py:method:: get_float(column_no)

      Get a float from the row

      :param int column_no: The column number to get (starting from 0)
      :return: The float data
      :rtype: float

   .. py:method:: get_char(column_no)

      Get a character string from the row

      :param int column_no: The column number to get (starting from 0)
      :return: The char data
      :rtype: str

   .. py:method:: get_column_type(column_no)

      Get the native column type for a column, types are to be compared with :ref:`type-constants`

      :param int column_no: The column number to get (starting from 0)
      :return: The column type
      :rtype: int

   .. py:method:: row_next()

      Start retrieving the next row of the results

      :return: None

   .. py:method:: get_column_count()

      Get the number of columns in the results

      :return: The number of columns
      :rtype: int
