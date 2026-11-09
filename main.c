#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdint.h>
#include <Python.h>

#define RATIO_X 1
#define RATIO_Y 2

int main(void)
{
  PyObject *name, *load_module, *func, *callfunc, *args;
  Py_Initialize();
  PyObject *sys_path = PySys_GetObject("path");
  PyObject *current_dir = PyUnicode_FromString(".");
  if (sys_path && current_dir)
  {
    PyList_Append(sys_path, current_dir);
  }
  Py_XDECREF(current_dir);
  name = PyUnicode_FromString("sample");
  load_module = PyImport_Import(name);
  Py_DECREF(name);
  func = PyObject_GetAttrString(load_module, "function");

  struct winsize ws;
  ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);

  int h = ws.ws_row / 2 - 2;
  int w = ws.ws_col / 2;
  for (int y = h; y > -h; y--)
  {
    for (int x = -w; x < ws.ws_col - w; x++)
    {
      args = Py_BuildValue("(dddd)", (double)x, (double)y, (double)RATIO_X, (double)RATIO_Y);
      callfunc = PyObject_CallObject(func, args);

      int yAxis = round(PyFloat_AsDouble(callfunc));
      Py_DECREF(args);
      Py_DECREF(callfunc);
      uint8_t whatsToWrite = 0;
      whatsToWrite |= (y == 0) << 7;
      whatsToWrite |= (x == 0) << 6;
      whatsToWrite |= (yAxis == y) << 5;
      if (whatsToWrite & 1u << 5)
        printf("▩");
      else if (whatsToWrite & 1u << 6 && whatsToWrite & 1u << 7)
        printf("┼");
      else if (whatsToWrite & 1u << 6)
        printf("│");
      else if (whatsToWrite & 1u << 7)
        printf("─");
      else 
        printf(" ");
    }
    printf("\n");
  }

  Py_DECREF(args);
  Py_DECREF(callfunc);
  Py_DECREF(func);
  Py_DECREF(load_module);
  Py_Finalize();
  return (0);
}


