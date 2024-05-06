#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "needle.h"


static PyObject* to_list_object(List* li) {
    int n = length(li);
    PyObject* python_list = PyList_New(n);

    int i = 0;
    for (List* node = li; node != NULL; node = node->next){
        PyObject* word = PyUnicode_FromString(node->value);
        PyList_SetItem(python_list, i, word);
        i ++;
    }
    return python_list;
}

static PyObject* nwl_align(PyObject *self, PyObject *args) {
    const char* str1;
    const char* str2;

    if (!PyArg_ParseTuple(args, "ss", &str1, &str2)) {
        return NULL;
    }

    const int MATCH_AWARD = 20;
    const int GAP_PENALTY = -5;
    Alignment align = needle(str1, str2, MATCH_AWARD, GAP_PENALTY);
    
    PyObject* list1 = to_list_object(align.aligned1);
    PyObject* list2 = to_list_object(align.aligned2);
    return PyTuple_Pack(2, list1, list2);
}

static PyMethodDef NWLMethods[] = {
    {"align", nwl_align, METH_VARARGS, "Align sentences."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef nwlmodule = {
    PyModuleDef_HEAD_INIT,
    "nwl",
    NULL,
    -1,
    NWLMethods
};

PyMODINIT_FUNC PyInit_nwl(void) {
    return PyModule_Create(&nwlmodule);
}