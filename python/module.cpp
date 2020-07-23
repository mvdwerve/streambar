#include <stdio.h>
#include <Python.h>
#include <streambar.h>

#include <cstring>
#include <cerrno>

/**
 *  Process it into a bar
 */
size_t convert(Processor &processor, const std::string &input, const std::string &output)
{
    // open the files 
    std::ifstream in(input);
    if (!in.good()) throw std::runtime_error("failed to open input file: " + std::string(strerror(errno)));

    std::ofstream out(output, std::ios::trunc);
    if (!out.good()) throw std::runtime_error("failed to open output file: " + std::string(strerror(errno)));

    // printer
    BarPrinter printer(out);

    // create the barmaker
    BarMaker barmaker(&printer, &processor);

    // process
    Util::processTape(barmaker, in);

    // flush the barmaker
    barmaker.flush();

    // return number of bars
    return printer.number();
}

template <class P>
static PyObject* sizedbar(PyObject *self, PyObject *args, PyObject* kwargs) {
    // input and output are both required
    const char *input;
    const char *output;
    int size = 100;

    // number of bars written
    size_t numbars = 0;

    // the keywords, only size is applicable
    static const char* keywords[] = {"", "", "size", NULL};

    // we may fail to parse the keywords, or something else might go wrong
    try
    {
        // allow the arguments
        if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss$i", const_cast<char**>(keywords), &input, &output, &size)) throw std::runtime_error("Invalid arguments, expected size:int");

        // make the bar processor
        P processor(size);

        // open the files
        numbars = convert(processor, input, output);
    }

    // catch the runtime error we might have thrown
    catch (const std::runtime_error &e)
    {
        // clear previous error
        PyErr_Clear();

        // set the string
        PyErr_SetString(PyExc_TypeError, e.what());

        // failed
        return nullptr;
    }

    //printf("Hello, %s!\n", name);
    return PyLong_FromUnsignedLong(numbars);
}

static PyObject* dollarbar(PyObject *self, PyObject *args, PyObject* kwargs) {
    // input and output are both required
    const char *input;
    const char *output;
    float size = 100;

    // number of bars written
    size_t numbars = 0;

    // the keywords, only size is applicable
    static const char* keywords[] = {"", "", "size", NULL};

    // we may fail to parse the keywords, or something else might go wrong
    try
    {
        // allow the arguments
        if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss$f", const_cast<char**>(keywords), &input, &output, &size)) throw std::runtime_error("Invalid arguments, expected size:float");

        // make the bar processor
        DollarBarProcessor processor(size);

        // open the files
        numbars = convert(processor, input, output);
    }

    // catch the runtime error we might have thrown
    catch (const std::runtime_error &e)
    {
        // clear previous error
        PyErr_Clear();

        // set the string
        PyErr_SetString(PyExc_TypeError, e.what());

        // failed
        return nullptr;
    }

    //printf("Hello, %s!\n", name);
    return PyLong_FromUnsignedLong(numbars);
}

static PyObject* performance(PyObject *self, PyObject *args, PyObject* kwargs) {
    // input and output are both required
    const char *file;
    const char *actions;
    const char *output;

    // the keywords, only size is applicable
    static const char* keywords[] = {"", "", NULL};

    // amount of shares bought
    size_t stocks = 0;
    float money = 0.0;

    // we may fail to parse the keywords, or something else might go wrong
    try
    {
        // allow the arguments
        if (!PyArg_ParseTupleAndKeywords(args, kwargs, "sss", const_cast<char**>(keywords), &file, &actions)) throw std::runtime_error("Invalid arguments");

        // open the files 
        std::ifstream s(file);
        if (!s.good()) throw std::runtime_error("failed to open tape file: " + std::string(strerror(errno)));

        // the actions file
        std::ifstream a(actions);
        if (!a.good()) throw std::runtime_error("failed to open action file: " + std::string(strerror(errno)));

        // output file, where at every trading junction the current state is appended
        std::ofstream o(output, std::ios::app);
        if (!o.good()) throw std::runtime_error("failed to open output file: " + std::string(strerror(errno)));
        
        // the simulated data, has actions and an output
        Simulated simulated(a, o);

        // process the simulated data
        Util::process(simulated, s);
    }

    // catch the runtime error we might have thrown
    catch (const std::runtime_error &e)
    {
        // clear previous error
        PyErr_Clear();

        // set the string
        PyErr_SetString(PyExc_TypeError, e.what());

        // failed
        return nullptr;
    }

    //printf("Hello, %s!\n", name);
    return PyLong_FromUnsignedLong(1);
}

static PyObject* mml_to_tape(PyObject *self, PyObject *args, PyObject *kwargs) {
    // input and output are both required
    const char *input = nullptr;
    const char *output = nullptr;
    size_t offset = 0;

    // the keywords, only size is applicable
    // the keywords, only size is applicable
    static const char* keywords[] = {"", "", "", NULL};

    // we may fail to parse the keywords, or something else might go wrong
    try
    {
        // allow the arguments
        if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ssL", const_cast<char**>(keywords), &input, &output, &offset)) return nullptr;

        // open the files 
        std::ifstream i(input);
        if (!i.good()) throw std::runtime_error("failed to open mml file: " + std::string(strerror(errno)));

        // the actions file
        std::ofstream o(output, std::ofstream::trunc);
        if (!o.good()) throw std::runtime_error("failed to open output file: " + std::string(strerror(errno)));

        // tape maker
        MmlTapeMaker maker(o, offset, Util::offset("09:30:00.000000"), Util::offset("15:55:00.000000"));

        // process the simulated data
        Util::process(maker, i);
    }

    // catch the runtime error we might have thrown
    catch (const std::runtime_error &e)
    {
        // clear previous error
        PyErr_Clear();

        // set the string
        PyErr_SetString(PyExc_TypeError, e.what());

        // failed
        return nullptr;
    }

    //printf("Hello, %s!\n", name);
    return PyLong_FromUnsignedLong(1);
}

static PyObject* negspreadtrades(PyObject *self, PyObject *args, PyObject *kwargs) {
    // input and output are both required
    const char *input = nullptr;
    const char *output = nullptr;

    // the keywords, only size is applicable
    // the keywords, only size is applicable
    static const char* keywords[] = {"", "", NULL};

    // we may fail to parse the keywords, or something else might go wrong
    try
    {
        // allow the arguments
        if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss", const_cast<char**>(keywords), &input, &output)) return nullptr;

        // open the files 
        std::ifstream i(input);
        if (!i.good()) throw std::runtime_error("failed to open tape file: " + std::string(strerror(errno)));

        // the actions file
        std::ofstream o(output, std::ofstream::trunc);
        if (!o.good()) throw std::runtime_error("failed to open output file: " + std::string(strerror(errno)));

        // tape maker
        NegSpreads maker(o);

        // process the simulated data
        Util::processTape(maker, i);
    }

    // catch the runtime error we might have thrown
    catch (const std::runtime_error &e)
    {
        // clear previous error
        PyErr_Clear();

        // set the string
        PyErr_SetString(PyExc_TypeError, e.what());

        // failed
        return nullptr;
    }

    //printf("Hello, %s!\n", name);
    return PyLong_FromUnsignedLong(1);
}

// Method definition object for this extension, these argumens mean:
// ml_name: The name of the method
// ml_meth: Function pointer to the method implementation
// ml_flags: Flags indicating special features of this method, such as
//          accepting arguments, accepting keyword arguments, being a
//          class method, or being a static method of a class.
// ml_doc:  Contents of this method's docstring
static PyMethodDef methods[] = { 
    {   
        "tick", (PyCFunction)sizedbar<TickBarProcessor>, METH_VARARGS | METH_KEYWORDS,
        "Generate tick bars from a given file. size=trades:int"
    },  
    {   
        "volume", (PyCFunction)sizedbar<VolumeBarProcessor>, METH_VARARGS | METH_KEYWORDS,
        "Generate volume bars from a given file. size=volume:int"
    },  
    {   
        "time", (PyCFunction)sizedbar<TimeBarProcessor>, METH_VARARGS | METH_KEYWORDS,
        "Generate time bars from a given file. size=seconds:int"
    },  
    {   
        "change", (PyCFunction)sizedbar<ChangeBarProcessor>, METH_VARARGS | METH_KEYWORDS,
        "Generate change bars from a given file. size=bips:int"
    },  
    {   
        "bachange", (PyCFunction)sizedbar<BAChangeBarProcessor>, METH_VARARGS | METH_KEYWORDS,
        "Generate change bars from a given file. size=bips:int"
    }, 
    {   
        "dollar", (PyCFunction)dollarbar, METH_VARARGS | METH_KEYWORDS,
        "Generate change bars from a given file. size=dollars:float"
    },  
    {
        "performance", (PyCFunction)performance, METH_VARARGS | METH_KEYWORDS,
        "Evaluate performance of a strategy."
    },
    {
        "mml_to_tape", (PyCFunction)mml_to_tape, METH_VARARGS | METH_KEYWORDS,
        "Convert MML file to tape file."
    },
    {
        "negspreads", (PyCFunction)negspreadtrades, METH_VARARGS | METH_KEYWORDS,
        "Find all negative spreads in a tape."
    },
    {NULL, NULL, 0, NULL}
};

// Module definition
// The arguments of this structure tell Python what to call your extension,
// what it's methods are and where to look for it's method definitions
static struct PyModuleDef definition = { 
    PyModuleDef_HEAD_INIT,
    "_streambar",
    "A Python module for fast bar generation using C++.",
    -1, 
    methods
};

// Module initialization
// Python calls this function when importing your extension. It is important
// that this function is named PyInit_[[your_module_name]] exactly, and matches
// the name keyword argument in setup.py's setup() call.
PyMODINIT_FUNC PyInit__streambar(void) {
    Py_Initialize();
    return PyModule_Create(&definition);
}