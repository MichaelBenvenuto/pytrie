#include <Python.h>
#include <structmember.h>
#include <trie.h>


_declspec(dllexport) PyObject* trie_construct(PyObject* self, PyObject* args);
_declspec(dllexport) PyObject* trie_deserialize(PyObject* self, PyObject* args);
_declspec(dllexport) PyObject* trie_serialize(PyObject* self, PyObject* args);
_declspec(dllexport) PyObject* trie_addword(PyObject* self, PyObject* args);
_declspec(dllexport) PyObject* trie_prefixSearch(PyObject* self, PyObject* args);

static PyMethodDef searchmethods[] = {
	{ "trie_construct", trie_construct, METH_VARARGS, "Create a Trie data structure" },
	{ "trie_deserialize", trie_deserialize, METH_VARARGS, "Create a Trie from a serialized Trie data structure" },
	{ "trie_serialize", trie_serialize, METH_VARARGS, "Serialize a Trie data structure" },
	{ "trie_addword", trie_addword, METH_VARARGS, "Add a word to a Trie data structure" },
	{ "trie_prefixsearch", trie_prefixSearch, METH_VARARGS, "Search for all words prefixed with string" },
	{NULL,NULL,0,NULL}
};

static struct PyModuleDef searchmodule = {
	PyModuleDef_HEAD_INIT, "Searches", "Search functions that are c/c++ accelerated", -1, searchmethods
};

PyMODINIT_FUNC PyInit__searches(void){
	Py_Initialize();
	PyObject* m = PyModule_Create(&searchmodule);
	return m;
}

void trie_destruct(PyObject* capsule){
	Trie* trie;
	trie = (Trie*)PyCapsule_GetPointer(capsule, "TriePointer");

	if (!trie){
		return;
	}

	trie->~Trie();
	free(trie);
}

/*----Trie Functions----

These functions wrap the Trie class and its members

*/

_declspec(dllexport) PyObject* trie_construct(PyObject* self, PyObject* args){
	Trie* trie = (Trie*)calloc(1, sizeof(Trie));

	if (!trie){
		return PyErr_NoMemory();
	}

	PyObject* trie_capsule = PyCapsule_New((void*)trie, "TriePointer", trie_destruct);
	PyCapsule_SetPointer(trie_capsule, (void*)trie);

	return trie_capsule;
}

_declspec(dllexport) PyObject* trie_deserialize(PyObject* self, PyObject* args) {
	char* serialized;
	int size;

	if (!PyArg_ParseTuple(args, "s#", &serialized, &size)) {
		return Py_None;
	}

	Trie* trie = Trie::deserialize((uint8_t*)serialized, size);

	PyObject* trie_capsule = PyCapsule_New((void*)trie, "TriePointer", trie_destruct);
	PyCapsule_SetPointer(trie_capsule, (void*)trie);

	return trie_capsule;
}

_declspec(dllexport) PyObject* trie_addword(PyObject* self, PyObject* args){
	PyObject* trie_capsule;
	char* word;

	if (!PyArg_ParseTuple(args, "Os", &trie_capsule, &word)) {
		return Py_None;
	}

	Trie* trie = (Trie*)PyCapsule_GetPointer(trie_capsule, "TriePointer");

	return PyBool_FromLong(trie->addWord(word));
}

_declspec(dllexport) PyObject* trie_prefixSearch(PyObject* self, PyObject* args){
	PyObject* trie_capsule;
	char* prefix;

	PyArg_ParseTuple(args, "Os", &trie_capsule, &prefix);

	Trie* trie = (Trie*)PyCapsule_GetPointer(trie_capsule, "TriePointer");

	size_t size;
	char** c = trie->prefixSearch(prefix, size);

	if (!c) {
		return Py_None;
	}

	PyObject* tuple = PyTuple_New(size);
	for (int i = 0; i < size; i++){
		PyTuple_SET_ITEM(tuple, i, Py_BuildValue("s", c[i]));
	}


	return tuple;
}

_declspec(dllexport) PyObject* trie_serialize(PyObject* self, PyObject* args) {
	PyObject* trie_capsule;

	if (!PyArg_ParseTuple(args, "O", &trie_capsule)) {
		return Py_None;
	}

	Trie* trie = (Trie*)PyCapsule_GetPointer(trie_capsule, "TriePointer");

	size_t size;

	return PyBytes_FromStringAndSize((char*)trie->serialize(size), size);
}
/*

These functions wrap the TrieNode class and its members

*/