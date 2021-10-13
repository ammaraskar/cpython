#include "Python.h"
#include "../Parser/tokenizer.h"

static struct PyModuleDef _tokenizemodule;

typedef struct {
    PyTypeObject *TokenizerIter;
} tokenize_state;

static tokenize_state *
get_tokenize_state(PyObject *module) {
    return (tokenize_state *)PyModule_GetState(module);
}

#define _tokenize_get_state_by_type(type) \
    get_tokenize_state(_PyType_GetModuleByDef(type, &_tokenizemodule))

#include "clinic/Python-tokenize.c.h"

/*[clinic input]
module _tokenizer
class _tokenizer.tokenizeriter "tokenizeriterobject *" "_tokenize_get_state_by_type(type)->TokenizerIter"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=96d98ee2fef7a8bc]*/

typedef struct
{
    PyObject_HEAD struct tok_state *tok;
} tokenizeriterobject;

/*[clinic input]
@classmethod
_tokenizer.tokenizeriter.__new__ as tokenizeriter_new

    source: str
[clinic start generated code]*/

static PyObject *
tokenizeriter_new_impl(PyTypeObject *type, const char *source)
/*[clinic end generated code: output=7fd9f46cf9263cbb input=4384b368407375c6]*/
{
    tokenizeriterobject *self = (tokenizeriterobject *)type->tp_alloc(type, 0);
    if (self == NULL) {
        return NULL;
    }
    PyObject *filename = PyUnicode_FromString("<string>");
    if (filename == NULL) {
        return NULL;
    }
    self->tok = PyTokenizer_FromUTF8(source, 1);
    if (self->tok == NULL) {
        Py_DECREF(filename);
        return NULL;
    }
    self->tok->filename = filename;
    return (PyObject *)self;
}

static PyObject *
tokenizeriter_next(tokenizeriterobject *it)
{
    const char *start;
    const char *end;
    int type = PyTokenizer_Get(it->tok, &start, &end);
    if (type == ERRORTOKEN && PyErr_Occurred()) {
        return NULL;
    }
    if (type == ERRORTOKEN || type == ENDMARKER) {
        PyErr_SetString(PyExc_StopIteration, "EOF");
        return NULL;
    }
    PyObject *str = NULL;
    if (start == NULL || end == NULL) {
        str = PyUnicode_FromString("");
    }
    else {
        str = PyUnicode_FromStringAndSize(start, end - start);
    }
    if (str == NULL) {
        return NULL;
    }

    Py_ssize_t size = it->tok->inp - it->tok->buf;
    PyObject *line = PyUnicode_DecodeUTF8(it->tok->buf, size, "replace");
    if (line == NULL) {
        Py_DECREF(str);
        return NULL;
    }
    const char *line_start = type == STRING ? it->tok->multi_line_start : it->tok->line_start;
    int lineno = type == STRING ? it->tok->first_lineno : it->tok->lineno;
    int end_lineno = it->tok->lineno;
    int col_offset = -1;
    int end_col_offset = -1;
    if (start != NULL && start >= line_start) {
        col_offset = (int)(start - line_start);
    }
    if (end != NULL && end >= it->tok->line_start) {
        end_col_offset = (int)(end - it->tok->line_start);
    }

    {
    PyObject* _builtResult1 = NULL;
    {
    _builtResult1 = PyTuple_New(7);
    if (_builtResult1 == NULL) {
        goto _builtResult1_cleanup;
    }
    {
    PyObject* _builtResult1_tupleMember0;
    PyObject* _objectArg = (PyObject*) str;
    if (_objectArg) {
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
    _builtResult1_tupleMember1 = PyLong_FromLong(type);
    if (_builtResult1_tupleMember1 == NULL) {
        Py_CLEAR(_builtResult1);
        goto _builtResult1_cleanup;
    }
    PyTuple_SET_ITEM(_builtResult1, 1, _builtResult1_tupleMember1);
    }
    {
    PyObject* _builtResult1_tupleMember2;
    _builtResult1_tupleMember2 = PyLong_FromLong(lineno);
    if (_builtResult1_tupleMember2 == NULL) {
        Py_CLEAR(_builtResult1);
        goto _builtResult1_cleanup;
    }
    PyTuple_SET_ITEM(_builtResult1, 2, _builtResult1_tupleMember2);
    }
    {
    PyObject* _builtResult1_tupleMember3;
    _builtResult1_tupleMember3 = PyLong_FromLong(end_lineno);
    if (_builtResult1_tupleMember3 == NULL) {
        Py_CLEAR(_builtResult1);
        goto _builtResult1_cleanup;
    }
    PyTuple_SET_ITEM(_builtResult1, 3, _builtResult1_tupleMember3);
    }
    {
    PyObject* _builtResult1_tupleMember4;
    _builtResult1_tupleMember4 = PyLong_FromLong(col_offset);
    if (_builtResult1_tupleMember4 == NULL) {
        Py_CLEAR(_builtResult1);
        goto _builtResult1_cleanup;
    }
    PyTuple_SET_ITEM(_builtResult1, 4, _builtResult1_tupleMember4);
    }
    {
    PyObject* _builtResult1_tupleMember5;
    _builtResult1_tupleMember5 = PyLong_FromLong(end_col_offset);
    if (_builtResult1_tupleMember5 == NULL) {
        Py_CLEAR(_builtResult1);
        goto _builtResult1_cleanup;
    }
    PyTuple_SET_ITEM(_builtResult1, 5, _builtResult1_tupleMember5);
    }
    {
    PyObject* _builtResult1_tupleMember6;
    PyObject* _objectArg = (PyObject*) line;
    if (_objectArg) {
        _builtResult1_tupleMember6 = _objectArg;
    } else {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_SystemError, "NULL object passed to Py_BuildValue");
        }
        _builtResult1_tupleMember6 = NULL;
    }
    if (_builtResult1_tupleMember6 == NULL) {
        Py_CLEAR(_builtResult1);
        goto _builtResult1_cleanup;
    }
    PyTuple_SET_ITEM(_builtResult1, 6, _builtResult1_tupleMember6);
    }
    _builtResult1_cleanup: ;
    }
    
    return _builtResult1;
    }
    
}

static void
tokenizeriter_dealloc(tokenizeriterobject *it)
{
    PyTypeObject *tp = Py_TYPE(it);
    PyTokenizer_Free(it->tok);
    tp->tp_free(it);
    Py_DECREF(tp);
}

static PyType_Slot tokenizeriter_slots[] = {
    {Py_tp_new, tokenizeriter_new},
    {Py_tp_dealloc, tokenizeriter_dealloc},
    {Py_tp_getattro, PyObject_GenericGetAttr},
    {Py_tp_iter, PyObject_SelfIter},
    {Py_tp_iternext, tokenizeriter_next},
    {0, NULL},
};

static PyType_Spec tokenizeriter_spec = {
    .name = "_tokenize.TokenizerIter",
    .basicsize = sizeof(tokenizeriterobject),
    .flags = (Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IMMUTABLETYPE),
    .slots = tokenizeriter_slots,
};

static int
tokenizemodule_exec(PyObject *m)
{
    tokenize_state *state = get_tokenize_state(m);
    if (state == NULL) {
        return -1;
    }

    state->TokenizerIter = (PyTypeObject *)PyType_FromModuleAndSpec(m, &tokenizeriter_spec, NULL);
    if (state->TokenizerIter == NULL) {
        return -1;
    }
    if (PyModule_AddType(m, state->TokenizerIter) < 0) {
        return -1;
    }

    return 0;
}

static PyMethodDef tokenize_methods[] = {
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static PyModuleDef_Slot tokenizemodule_slots[] = {
    {Py_mod_exec, tokenizemodule_exec},
    {0, NULL}
};

static int
tokenizemodule_traverse(PyObject *m, visitproc visit, void *arg)
{
    tokenize_state *state = get_tokenize_state(m);
    Py_VISIT(state->TokenizerIter);
    return 0;
}

static int
tokenizemodule_clear(PyObject *m)
{
    tokenize_state *state = get_tokenize_state(m);
    Py_CLEAR(state->TokenizerIter);
    return 0;
}

static void
tokenizemodule_free(void *m)
{
    tokenizemodule_clear((PyObject *)m);
}

static struct PyModuleDef _tokenizemodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "_tokenize",
    .m_size = sizeof(tokenize_state),
    .m_slots = tokenizemodule_slots,
    .m_methods = tokenize_methods,
    .m_traverse = tokenizemodule_traverse,
    .m_clear = tokenizemodule_clear,
    .m_free = tokenizemodule_free,
};

PyMODINIT_FUNC
PyInit__tokenize(void)
{
    return PyModuleDef_Init(&_tokenizemodule);
}
