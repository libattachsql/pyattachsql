DB API Connection Class
=======================

.. py:module:: attachdb

.. py:class:: Connection(host, port=3306, user='', password='', database='', autocommit=False)

   :param str host: The hostname to the server or path to Unix Domain Socket
   :param int port: The port number for the server or ``0`` for Unix Domain Socket
   :param str user: The username for the server
   :param str password: The password for the server
   :param str database: The default database for the connection
   :param bool autocommit: Enable/disable autocommit (``None`` for server default)

   .. py:method:: autocommit(setting)

      Enables/disables autocommit for the connection

      :param bool setting: ``True`` to enable, ``False`` to disable

   .. py:method:: commit()

      Commit a transaction

   .. py:method:: rollback()

      Roll back a transaction

   .. py:method:: close()

      Close the connection

   .. py:method:: cursor()

      Create a new cursor for the connection to execute queries

      :return: The new cursor
      :rtype: Cursor
