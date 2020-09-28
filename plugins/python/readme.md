## Python plugin for klish

This plugin integrates python into klish as a command language.

It also contains reasonably complete language bindings that make it possible to construct dynamic applications.

Both python2 and python3 are supported - even in the same build tree, but not in the same process.

### Design

This plugin shares language bindings between the clish plugin and a python package:

 * Binding library (libclish_python)
   * Contains language bindings and common support code
   * Provided as an independent shared library
 * Plugin (clish_python_plugin.so)
   * Integrates python as a clish plugin
   * Uses the binding library
   * Independent of python library search
 * Python package (import clish)
   * Provides direct API bindings
   * Wrapper around the binding library
   * Can also be used safely within clish

Doing things like this allows for two primary use-cases:

 * Python as a language in clish
   * More versatile than the shell backend
   * Opens up a huge library ecosystem to clish
   * API bindings allow advanced interactive features
 * Python using clish as a library
   * Use libclish dynamically
   * Only partly functional at the moment

