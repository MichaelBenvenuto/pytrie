#include <Python.h>
#include <trie.h>

_declspec(dllexport) PyObject* trie_construct(PyObject* self, PyObject* args);
_declspec(dllexport) PyObject* trie_addword(PyObject* self, PyObject* args);
_declspec(dllexport) PyObject* trie_prefixSearch(PyObject* self, PyObject* args);

static PyMethodDef searchmethods[] = {
	{ "trie_construct", trie_construct, METH_VARARGS, "Create a Trie data structure" },
	{ "trie_addword", trie_addword, METH_VARARGS, "Add a word to a Trie data structure" },
	{ "trie_prefixsearch", trie_prefixSearch, METH_VARARGS, "Search for all words prefixed with string" },
	{0,0,0,0}
};

static struct PyModuleDef searchmodule = {
	PyModuleDef_HEAD_INIT, "Searches", NULL, -1, searchmethods
};

PyMODINIT_FUNC PyInit__searches(void){
	PyObject* m = PyModule_Create(&searchmodule);
	if (m == NULL){
		return 0;
	}

	static PyObject* s_err;

	s_err = PyErr_NewException("search.error", NULL, NULL);
	Py_INCREF(s_err);
	PyModule_AddObject(m, "error", s_err);
	return m;
}

int main(int argc, char** argv){
	wchar_t *program = Py_DecodeLocale(argv[0], NULL);
	if (program == NULL) {
		printf("Fatal error: cannot decode argv[0]\n");
		exit(1);
	}

	PyImport_AppendInittab("Searches", PyInit__searches);
	Py_SetProgramName(program);
	Py_Initialize();

	PyMem_RawFree(program);
	return 0;
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

_declspec(dllexport) PyObject* trie_construct(PyObject* self, PyObject* args){
	Trie* trie = (Trie*)calloc(1, sizeof(Trie));

	if (!trie){
		return PyErr_NoMemory();
	}

	PyObject* trie_capsule = PyCapsule_New((void*)trie, "TriePointer", 0);
	PyCapsule_SetPointer(trie_capsule, (void*)trie);

	return trie_capsule;
}

_declspec(dllexport) PyObject* trie_addword(PyObject* self, PyObject* args){
	PyObject* trie_capsule;
	char* word;

	PyArg_ParseTuple(args, "Os", &trie_capsule, &word);

	Trie* trie = (Trie*)PyCapsule_GetPointer(trie_capsule, "TriePointer");

	return (trie->addWord(word)) ? Py_True : Py_False;
}

_declspec(dllexport) PyObject* trie_prefixSearch(PyObject* self, PyObject* args){
	PyObject* trie_capsule;
	char* prefix;

	PyArg_ParseTuple(args, "Os", &trie_capsule, &prefix);

	Trie* trie = (Trie*)PyCapsule_GetPointer(trie_capsule, "TriePointer");

	size_t size;
	char** c = trie->prefixSearch(prefix, size);

	PyObject* tuple = PyTuple_New(size);
	for (int i = 0; i < size; i++){
		PyTuple_SET_ITEM(tuple, i, Py_BuildValue("s", c[i]));
	}


	return tuple;
}
