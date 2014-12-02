Connection Class
================

.. py:module:: attachsql

.. py:class:: connection(hostname, user, password, database, port)

   :param str hostname: The hostname to the server
   :param str user: The user name to connect with
   :param str password: The password to connect with
   :param str database: The default database for the connection
   :param int port: The port to connect on or ``0`` for a Unix Domain Socket connection

   .. py:method:: connection_id()

      Returns the connection ID of the connection.  If there is no established connection this will return ``0``.

      :return: The ID for the connection
      :rtype: int

   .. py:method:: connect()

      Start connection to the server.  :py:meth:`poll` needs to be called (usually more than once) for the connection to actually occur.

      :return: ``True`` on success
      :rtype: bool

   .. py:method:: poll()

      Poll a connection to see if there is more data.

      :return: A numeric status to be compared with :ref:`return-constants`
      :rtype: int

   .. py:method:: get_server_version()

      Returns the version string for the server

      :return: The version string
      :rtype: str

   .. py:method:: query(query, [parameters])

      Send a query to the server.  Parameters can be given to fill in ``?`` markers in a query.  This should be in a Python list containing dictionaries formatted as follows as follows:

      .. code-block:: python

         [{'type': attachsql.ESCAPE_TYPE_CHAR, 'data':'hello'}, {'type': attachsql.ESCAPE_TYPE_INT, 'data': 123456, is_unsigned: True}]

      For a full list of types see :ref:`escape-constants`

      :param str query: The query statement to send
      :param list parameters:
      :return: A :py:class:`query` class
      :rtype: query

   .. py:method:: set_ssl(key, cert, ca, capath, cipher, [verify])

      Sets the SSL certifications and enables SSL connections

      :param str key: The path for the key file
      :param str cert: The path for the certificate
      :param str ca: The path for the CA
      :param str capath: The path containing many CAs
      :param str cipher: A list of ciphers to allow
      :param bool verify: Set to verify the SSL connection
      :return: ``True`` on success
      :rtype: boolean

   .. py:method:: set_option(option, [unused])

      Sets a connection option, the list of possible options can be found in :ref:`option-constants`

      :param int option: The option to set
      :param unused: This parameter is for future use
      :return: ``True`` on success
      :rtype: boolean

   .. py:method:: prepare_statement(statement)

      Initialize and start sending a prepared statement

      :param str statement: The statement to send
      :return: An instance of :py:class:`statement`
      :rtype: statement
