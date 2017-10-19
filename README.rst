Amber OS
=========

Amber OS simulates the runtime behavior of an AUTOSAR OS under Windows and Linux.

This project is part of a bigger initiative that involves several projects related to AUTOSAR.

The goal is to make it possible to develop fully functional AUTOSAR software components (SWCs) and test them out
on Linux or Windows machines by placing the SWCs in sort of virtual container.

The AUTOSAR virtual container (including its SWCs) can later be deployed onto an embedded Linux system where the SWCs will run as it would
on a traditional AUTOSAR system. Vehicle signal communication is done using the `APX <https://github.com/cogu/apx>`_ solution.

This project
------------

This project contains the OS simulation layer that runs on top of Linux or Windows. It currently just contains a scheduler.

Related projects
----------------

- `autosar <https://github.com/cogu/autosar>`_: A set of python3 modules for creating and working with AUTOSAR SWCs.
- `c-apx <https://github.com/cogu/c-apx>`_: APX is used as AUTOSAR Com module.
- `py-apx <https://github.com/cogu/py-apx>`_: Python3 module that contains the toolchain for APX.
- `autosar-demo <https://github.com/cogu/autosar-demo>`_: A demo project that shows how all pieces fit together.

Helper projects
---------------

- `adt <https://github.com/cogu/adt>`_: Abstract data structures for the C language.
- `dtl_type <https://github.com/cogu/dtl_type>`_: Dynamic language data types for C.
- `cfile <https://github.com/cogu/cfile>`_: A C code generator written in Python3.
- `msocket <https://github.com/cogu/msocket>`_: Event-driven socket wrapper for Linux and Windows.