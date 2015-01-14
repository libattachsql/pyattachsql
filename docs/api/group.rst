Group Class
===========

.. py:module:: attachsql

.. py:class:: group(callback_function, callback_context)

   Creates a group of connections to execute statements in the same event loop.  In this mode events trigger callbacks instead of the normal polling so a callback function is required.

   :param object callback_function: The function name to use for the callback
   :param object callback_context: An object of arbitrary data to send to the callback function

   .. py:method:: create_connection(hostname, user, password, database, port)

      Creates a new connection in the connection group and returns the connection object.  The connection itself is not made at this time.

      :param str hostname: The hostname to the server
      :param str user: The user name to connect with
      :param str password: The password to connect with
      :param int port: The port to connect on or ``0`` for a Unix Domain Socket connection
      :return: A connection class which is attached to this group
      :rtype: connection

   .. py:method:: run()

      Runs a single iteration of the event loop.  If an event is triggered the py:function:`my_callback` will be fired before this returns.

      :return: None
