Library "jsobjects"
===================

This library provides an abstraction over JavaScriptCore and V8
to simplify development of extensions for those engines.

Instead of the classical approach with SWIG, here the data originates
from Javascript. Bidirectional data flow is simplified.
A C++ only implementation is provided to allow a pure C++ only testing
environment.

This work is considered to be a basis for implementation of SWIG's
"director" feature for the Javascript module.
