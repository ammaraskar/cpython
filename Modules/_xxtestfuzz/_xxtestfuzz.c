#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdlib.h>
#include <inttypes.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);

static PyObject* _fuzz_run(PyObject* self, PyObject* args) {
    const char* buf;
    Py_ssize_t size;
    int _parseResult = 1;
    {
        Py_ssize_t _nargs = PyTuple_GET_SIZE(args);
        if (!_PyArg_CheckPositional("_fuzz_run", _nargs, 1, 1)) {
            _parseResult = 0; goto _parse_exit_label;
        }
        {
            Py_ssize_t* _len = &size;
            if (PyUnicode_Check(PyTuple_GET_ITEM(args, 0))) {
                const char* _sarg = PyUnicode_AsUTF8AndSize(PyTuple_GET_ITEM(args, 0), _len);
                if (_sarg == NULL) {
                    PyErr_SetString(PyExc_TypeError, "unicode conversion error");
                    _parseResult = 0; goto _parse_exit_label;
                }
                *&buf = _sarg;
            } else {
                PyBufferProcs *pb = Py_TYPE(PyTuple_GET_ITEM(args, 0))->tp_as_buffer;
                if (pb != NULL && pb->bf_releasebuffer != NULL) {
                    PyErr_Format(PyExc_TypeError, "must be read-only bytes-like object, not %.50s", Py_TYPE(PyTuple_GET_ITEM(args, 0))->tp_name);
                    _parseResult = 0; goto _parse_exit_label;
                }
                Py_buffer _view;
                if (PyObject_GetBuffer(PyTuple_GET_ITEM(args, 0), &_view, PyBUF_SIMPLE) != 0) {
                    PyErr_Format(PyExc_TypeError, "must be bytes-like object, not %.50s", Py_TYPE(PyTuple_GET_ITEM(args, 0))->tp_name);
                    _parseResult = 0; goto _parse_exit_label;
                }
                if (!PyBuffer_IsContiguous(&_view, 'C')) {
                    PyBuffer_Release(&_view);
                    PyErr_Format(PyExc_TypeError, "must be contiguous buffer, not %.50s", Py_TYPE(PyTuple_GET_ITEM(args, 0))->tp_name);
                    _parseResult = 0; goto _parse_exit_label;
                }
                *_len = _view.len;
                *&buf = _view.buf;
                PyBuffer_Release(&_view);
            }
        }
    }
    _parse_exit_label:
    if (!_parseResult) {
        return NULL;
    }
    int rv = LLVMFuzzerTestOneInput((const uint8_t*)buf, size);
    if (PyErr_Occurred()) {
        return NULL;
    }
    if (rv != 0) {
        // Nonzero return codes are reserved for future use.
        PyErr_Format(
            PyExc_RuntimeError, "Nonzero return code from fuzzer: %d", rv);
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyMethodDef module_methods[] = {
    {"run", (PyCFunction)_fuzz_run, METH_VARARGS, ""},
    {NULL},
};

static struct PyModuleDef _fuzzmodule = {
        PyModuleDef_HEAD_INIT,
        "_fuzz",
        NULL,
        0,
        module_methods,
        NULL,
        NULL,
        NULL,
        NULL
};

PyMODINIT_FUNC
PyInit__xxtestfuzz(void)
{
    return PyModule_Create(&_fuzzmodule);
}
