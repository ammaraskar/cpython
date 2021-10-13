/*
 * Python UUID module that wraps libuuid or Windows rpcrt4.dll.
 * DCE compatible Universally Unique Identifier library.
 */

#define PY_SSIZE_T_CLEAN

#include "Python.h"
#ifdef HAVE_UUID_UUID_H
#include <uuid/uuid.h>
#elif defined(HAVE_UUID_H)
#include <uuid.h>
#endif

#ifdef MS_WINDOWS
#include <rpc.h>
#endif

#ifndef MS_WINDOWS

static PyObject *
py_uuid_generate_time_safe(PyObject *Py_UNUSED(context),
                           PyObject *Py_UNUSED(ignored))
{
    uuid_t uuid;
#ifdef HAVE_UUID_GENERATE_TIME_SAFE
    int res;

    res = uuid_generate_time_safe(uuid);
    {
    PyObject* _builtResult1 = NULL;
    {
    _builtResult1 = PyTuple_New(2);
    if (_builtResult1 == NULL) {
        goto _builtResult1_cleanup;
    }
    {
    PyObject* _builtResult1_tupleMember0;
    const char* _strArg = (const char*) (const char *) uuid;
    if (_strArg) {
        _builtResult1_tupleMember0 = PyBytes_FromStringAndSize(_strArg, sizeof(uuid));
    } else {
        _builtResult1_tupleMember0 = Py_None;
        Py_INCREF(_builtResult1_tupleMember0);
    }
    if (_builtResult1_tupleMember0 == NULL) {
        Py_CLEAR(_builtResult1);
        goto _builtResult1_cleanup;
    }
    PyTuple_SET_ITEM(_builtResult1, 0, _builtResult1_tupleMember0);
    }
    {
    PyObject* _builtResult1_tupleMember1;
    _builtResult1_tupleMember1 = PyLong_FromLong(res);
    if (_builtResult1_tupleMember1 == NULL) {
        Py_CLEAR(_builtResult1);
        goto _builtResult1_cleanup;
    }
    PyTuple_SET_ITEM(_builtResult1, 1, _builtResult1_tupleMember1);
    }
    _builtResult1_cleanup: ;
    }
    
    return _builtResult1;
    }
    
#elif defined(HAVE_UUID_CREATE)
    uint32_t status;
    uuid_create(&uuid, &status);
# if defined(HAVE_UUID_ENC_BE)
    unsigned char buf[sizeof(uuid)];
    uuid_enc_be(buf, &uuid);
    return Py_BuildValue("y#i", buf, sizeof(uuid), (int) status);
# else
    return Py_BuildValue("y#i", (const char *) &uuid, sizeof(uuid), (int) status);
# endif /* HAVE_UUID_CREATE */
#else /* HAVE_UUID_GENERATE_TIME_SAFE */
    uuid_generate_time(uuid);
    return Py_BuildValue("y#O", (const char *) uuid, sizeof(uuid), Py_None);
#endif /* HAVE_UUID_GENERATE_TIME_SAFE */
}

#else /* MS_WINDOWS */

static PyObject *
py_UuidCreate(PyObject *Py_UNUSED(context),
              PyObject *Py_UNUSED(ignored))
{
    UUID uuid;
    RPC_STATUS res;

    Py_BEGIN_ALLOW_THREADS
    res = UuidCreateSequential(&uuid);
    Py_END_ALLOW_THREADS

    switch (res) {
    case RPC_S_OK:
    case RPC_S_UUID_LOCAL_ONLY:
    case RPC_S_UUID_NO_ADDRESS:
        /*
        All success codes, but the latter two indicate that the UUID is random
        rather than based on the MAC address. If the OS can't figure this out,
        neither can we, so we'll take it anyway.
        */
        return Py_BuildValue("y#", (const char *)&uuid, sizeof(uuid));
    }
    PyErr_SetFromWindowsErr(res);
    return NULL;
}

#endif /* MS_WINDOWS */


static int
uuid_exec(PyObject *module) {
    assert(sizeof(uuid_t) == 16);
#if defined(MS_WINDOWS)
    int has_uuid_generate_time_safe = 0;
#elif defined(HAVE_UUID_GENERATE_TIME_SAFE)
    int has_uuid_generate_time_safe = 1;
#else
    int has_uuid_generate_time_safe = 0;
#endif
    if (PyModule_AddIntConstant(module, "has_uuid_generate_time_safe",
                                has_uuid_generate_time_safe) < 0) {
        return -1;
    }
    return 0;
}

static PyMethodDef uuid_methods[] = {
#if defined(HAVE_UUID_UUID_H) || defined(HAVE_UUID_H)
    {"generate_time_safe", py_uuid_generate_time_safe, METH_NOARGS, NULL},
#endif
#if defined(MS_WINDOWS)
    {"UuidCreate", py_UuidCreate, METH_NOARGS, NULL},
#endif
    {NULL, NULL, 0, NULL}           /* sentinel */
};

static PyModuleDef_Slot uuid_slots[] = {
    {Py_mod_exec, uuid_exec},
    {0, NULL}
};

static struct PyModuleDef uuidmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "_uuid",
    .m_size = 0,
    .m_methods = uuid_methods,
    .m_slots = uuid_slots,
};

PyMODINIT_FUNC
PyInit__uuid(void)
{
    return PyModuleDef_Init(&uuidmodule);
}
