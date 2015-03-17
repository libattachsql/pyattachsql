DB API Module Basics
====================

.. py:module:: attachdb

Exceptions
----------

.. py:exception:: Warning

   A warning exception, a subclass of :py:exc:`StandardError`

   .. note:: currently unused

.. py:exception:: Error

   A base error exception, a subclass of :py:exc:`StandardError`

.. py:exception:: InterfaceError

   An error in the interface, a subclass of :py:exc:`Error`

   .. note:: currently unused

.. py:exception:: DatabaseError

   A base exception for errors in the database, a subclass of :py:exc:`Error`

.. py:exception:: DataError

   An error in the processed data, a subclass of :py:exc:`DatabaseError`

   .. note:: currently unused

.. py:exception:: OperationalError

   An error in the database server, a subclass of :py:exc:`DatabaseError`

.. py:exception:: IntegrityError

   An error in foreign key, a subclass of :py:exc:`DatabaseError`

   .. note:: currently unused

.. py:exception:: InternalError

   An internal error in the database server, a subclass of :py:exc:`DatabaseError`

   .. note:: currently unused

.. py:exception:: ProgrammingError

   An error in the client side application, a subclass of :py:exc:`DatabaseError`

.. py:exception:: NotSupportedError

   Feature not supported error, a subclass of :py:exc:`DatabaseError`

   .. note:: currently unused

Functions
---------

.. py:function:: connection(host, port=3306, user='', password='', database='', autocommit=False)

   An alias to create an instance of :py:class:`Connection`

