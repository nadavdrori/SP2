
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "cap.h"

static PyObject *fit(PyObject *self, PyObject *args) // function called from Python file
{
    int iter, eps, k, vec_num, vec_size, i, arr_size, centroid;
    double *coor_arr, *final_centroids;
    int *centroids;
    double num;
    PyObject *item, *temp_coor_arr, *temp_centroids;
    /* This parses the Python arguments into a double (d)  variable named z and int (i) variable named n*/
    if (!PyArg_ParseTuple(args, "iiiiiOO", &iter, &eps, &k, &vec_num, &vec_size, &temp_coor_arr, &temp_centroids))
    {
        printf("An Error Has Occurred");
        exit(1);
        return NULL; /*In the CPython API, a NULL value is never valid for a PyObject* so it is used to signal that an error has occurred. */
    }

    arr_size = vec_num * vec_size;
    centroids = (int *)malloc(k * sizeof(int));
    coor_arr = (double *)malloc(arr_size * sizeof(double));

    if (centroids == NULL || coor_arr == NULL)
    {
        printf("An Error Has Occurred");
        exit(1);
        return NULL;
    }

    for (i = 0; i < arr_size; i++)
    {
        item = PyList_GetItem(temp_coor_arr, i);
        num = PyFloat_AsDouble(item);
        coor_arr[i] = num;
    }

    for (i = 0; i < k; i++)
    {
        item = PyList_GetItem(temp_centroids, i);
        centroid = (int)PyLong_AsLong(item);
        centroids[i] = num;
    }

    final_centroids = kmeans(coor_arr, centroids, vec_size, arr_size, k, iter, eps);

    PyObject *centroids_list = PyList_New(arr_size);
    for (i = 0; i < k * vec_size; i++)
    {
        PyList_Append(centroids_list, PyFloat_FromDouble(final_centroids[i]));
    }

    return centroids_list;
    /* This builds the answer ("d" = Convert a C double to a Python floating point number) back into a python object
    return Py_BuildValue("[f]", /*NAME OF FUNC FROM HM1 geo_c(z, n));  Py_BuildValue(...) returns a PyObject*  */
}

static PyMethodDef kmeansMethods[] = {
    // declaring the functions in C to the Python file

    {"fit",                           /* the Python method name that will be used */
     (PyCFunction)fit,                /* the C-function that implements the Python function and returns static PyObject*  */
     METH_VARARGS,                    /* flags indicating parameters accepted for this function */
     PyDoc_STR("kmeans algorithem")}, /*  The docstring for the function */
    {NULL, NULL, 0, NULL}             /* The last entry must be all NULL as shown to act as a
                                         sentinel. Python looks for this entry to know that all
                                         of the functions for the module have been defined. */
};

static struct PyModuleDef kmeans_module = {
    // initiating the module

    PyModuleDef_HEAD_INIT,
    "kmeansmodule", /* name of module */
    NULL,           /* module documentation, may be NULL */
    -1,             /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    kmeansMethods   /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_kmeansmodule(void)
// the function that will be called when the module is imported in python

{
    PyObject *m;
    m = PyModule_Create(&kmeans_module);
    if (!m)
    {
        return NULL;
    }
    return m;
}