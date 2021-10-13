/* microprotocols.c - minimalist and non-validating protocols implementation
 *
 * Copyright (C) 2003-2004 Federico Di Gregorio <fog@debian.org>
 *
 * This file is part of psycopg and was adapted for pysqlite. Federico Di
 * Gregorio gave the permission to use it within pysqlite under the following
 * license:
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <Python.h>

#include "cursor.h"
#include "microprotocols.h"
#include "prepare_protocol.h"


/* pysqlite_microprotocols_init - initialize the adapters dictionary */

int
pysqlite_microprotocols_init(PyObject *module)
{
    /* create adapters dictionary and put it in module namespace */
    pysqlite_state *state = pysqlite_get_state(module);
    state->psyco_adapters = PyDict_New();
    if (state->psyco_adapters == NULL) {
        return -1;
    }

    return PyModule_AddObjectRef(module, "adapters", state->psyco_adapters);
}


/* pysqlite_microprotocols_add - add a reverse type-caster to the dictionary */

int
pysqlite_microprotocols_add(PyTypeObject *type, PyObject *proto, PyObject *cast)
{
    PyObject* key;
    int rc;

    assert(type != NULL);
    assert(proto != NULL);
    {
    PyObject* _builtResult1 = NULL;
    {
    _builtResult1 = PyTuple_New(2);
    if (_builtResult1 == NULL) {
        goto _builtResult1_cleanup;
    }
    {
    PyObject* _builtResult1_tupleMember0;
    PyObject* _objectArg = (PyObject*) (PyObject*)type;
    if (_objectArg) {
        Py_INCREF(_objectArg);
        _builtResult1_tupleMember0 = _objectArg;
    } else {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_SystemError, "NULL object passed to Py_BuildValue");
        }
        _builtResult1_tupleMember0 = NULL;
    }
    if (_builtResult1_tupleMember0 == NULL) {
        Py_CLEAR(_builtResult1);
        goto _builtResult1_cleanup;
    }
    PyTuple_SET_ITEM(_builtResult1, 0, _builtResult1_tupleMember0);
    }
    {
    PyObject* _builtResult1_tupleMember1;
    PyObject* _objectArg = (PyObject*) proto;
    if (_objectArg) {
        Py_INCREF(_objectArg);
        _builtResult1_tupleMember1 = _objectArg;
    } else {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_SystemError, "NULL object passed to Py_BuildValue");
        }
        _builtResult1_tupleMember1 = NULL;
    }
    if (_builtResult1_tupleMember1 == NULL) {
        Py_CLEAR(_builtResult1);
        goto _builtResult1_cleanup;
    }
    PyTuple_SET_ITEM(_builtResult1, 1, _builtResult1_tupleMember1);
    }
    _builtResult1_cleanup: ;
    }
    key = _builtResult1;
    } 
    if (!key) {
        return -1;
    }

    pysqlite_state *state = pysqlite_get_state(NULL);
    rc = PyDict_SetItem(state->psyco_adapters, key, cast);
    Py_DECREF(key);

    return rc;
}

/* pysqlite_microprotocols_adapt - adapt an object to the built-in protocol */

PyObject *
pysqlite_microprotocols_adapt(pysqlite_state *state, PyObject *obj,
                              PyObject *proto, PyObject *alt)
{
    _Py_IDENTIFIER(__adapt__);
    _Py_IDENTIFIER(__conform__);
    PyObject *adapter, *key, *adapted;

    /* we don't check for exact type conformance as specified in PEP 246
       because the PrepareProtocolType type is abstract and there is no
       way to get a quotable object to be its instance */

    /* look for an adapter in the registry */
    {
    PyObject* _builtResult2 = NULL;
    {
    _builtResult2 = PyTuple_New(2);
    if (_builtResult2 == NULL) {
        goto _builtResult2_cleanup;
    }
    {
    PyObject* _builtResult2_tupleMember0;
    PyObject* _objectArg = (PyObject*) (PyObject*)Py_TYPE(obj);
    if (_objectArg) {
        Py_INCREF(_objectArg);
        _builtResult2_tupleMember0 = _objectArg;
    } else {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_SystemError, "NULL object passed to Py_BuildValue");
        }
        _builtResult2_tupleMember0 = NULL;
    }
    if (_builtResult2_tupleMember0 == NULL) {
        Py_CLEAR(_builtResult2);
        goto _builtResult2_cleanup;
    }
    PyTuple_SET_ITEM(_builtResult2, 0, _builtResult2_tupleMember0);
    }
    {
    PyObject* _builtResult2_tupleMember1;
    PyObject* _objectArg = (PyObject*) proto;
    if (_objectArg) {
        Py_INCREF(_objectArg);
        _builtResult2_tupleMember1 = _objectArg;
    } else {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_SystemError, "NULL object passed to Py_BuildValue");
        }
        _builtResult2_tupleMember1 = NULL;
    }
    if (_builtResult2_tupleMember1 == NULL) {
        Py_CLEAR(_builtResult2);
        goto _builtResult2_cleanup;
    }
    PyTuple_SET_ITEM(_builtResult2, 1, _builtResult2_tupleMember1);
    }
    _builtResult2_cleanup: ;
    }
    key = _builtResult2;
    } 
    if (!key) {
        return NULL;
    }
    adapter = PyDict_GetItemWithError(state->psyco_adapters, key);
    Py_DECREF(key);
    if (adapter) {
        Py_INCREF(adapter);
        adapted = PyObject_CallOneArg(adapter, obj);
        Py_DECREF(adapter);
        return adapted;
    }
    if (PyErr_Occurred()) {
        return NULL;
    }

    /* try to have the protocol adapt this object */
    if (_PyObject_LookupAttrId(proto, &PyId___adapt__, &adapter) < 0) {
        return NULL;
    }
    if (adapter) {
        adapted = PyObject_CallOneArg(adapter, obj);
        Py_DECREF(adapter);

        if (adapted == Py_None) {
            Py_DECREF(adapted);
        }
        else if (adapted || !PyErr_ExceptionMatches(PyExc_TypeError)) {
            return adapted;
        }
        else {
            PyErr_Clear();
        }
    }

    /* and finally try to have the object adapt itself */
    if (_PyObject_LookupAttrId(obj, &PyId___conform__, &adapter) < 0) {
        return NULL;
    }
    if (adapter) {
        adapted = PyObject_CallOneArg(adapter, proto);
        Py_DECREF(adapter);

        if (adapted == Py_None) {
            Py_DECREF(adapted);
        }
        else if (adapted || !PyErr_ExceptionMatches(PyExc_TypeError)) {
            return adapted;
        }
        else {
            PyErr_Clear();
        }
    }

    if (alt) {
        return Py_NewRef(alt);
    }
    /* else set the right exception and return NULL */
    PyErr_SetString(state->ProgrammingError, "can't adapt");
    return NULL;
}
