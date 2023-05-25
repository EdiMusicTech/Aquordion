#include "m_pd.h"
#include <stdlib.h>
#include <stdbool.h>

static bool is_digit(char c)
{
  bool result = c >= '0' && c <= '9';
  return result;
}

static bool is_number(const char* s)
{
  bool result = true;

  for (const char* c = s; *c; c++)
    result = is_digit(*c);

  return result;
}

static t_class *getline_class;

typedef struct _getline {
  t_object  obj;
  t_outlet* out;

  char* buffer;
  int buffer_size;
  int buffer_idx;

  t_atom* list;
  int list_size;
  int list_idx;
} t_getline;

void *getline_new(void)
{
  t_getline *x = (t_getline *)pd_new(getline_class);
  x->buffer_size = 1024;
  x->buffer_idx = 0;
  x->buffer = (char*)malloc(x->buffer_size * sizeof(*x->buffer));

  x->list_size = 1024;
  x->list_idx = 0;
  x->list = (t_atom*)malloc(x->buffer_size * sizeof(*x->list));

  x->out = outlet_new(&x->obj, &s_list);

  return (void *)x;
}

void getline_free(t_getline* x)
{
  outlet_free(x->out);
  free(x->buffer);
  free(x->list);
}

void getline_float(t_getline* x, t_floatarg f)
{
  if (x->buffer_idx >= x->buffer_size)
  {
    x->buffer_size *= 2;
    x->buffer = realloc(x->buffer, x->buffer_size);
  }

  if (x->list_idx >= x->list_size)
  {
    x->list_size *= 2;
    x->list = realloc(x->list, x->list_size);
  }

  if (f == '\n' || f == '\r')
  {
    x->buffer[x->buffer_idx] = (char)0;

    if (x->buffer[0] != 0)
    {
      t_atom atom;
      atom.a_type = A_SYMBOL;
      atom.a_w.w_symbol = gensym(x->buffer);
      x->list[x->list_idx++] = atom;
    }

    if (x->list_idx > 0)
      outlet_list(x->out, &s_list, x->list_idx, x->list);

    x->list_idx = 0;
    x->buffer_idx = 0;
  }
  else
  {
    if (f == ' ')
    {
      x->buffer[x->buffer_idx] = (char)0;

      t_atom atom;

      if (is_number(x->buffer))
      {
        atom.a_type = A_FLOAT;
        atom.a_w.w_float = atoi(x->buffer);
      }
      else
      {
        atom.a_type = A_SYMBOL;
        atom.a_w.w_symbol = gensym(x->buffer);
      }

      x->list[x->list_idx++] = atom;

      x->buffer_idx = 0;
    }
    else
    {
      x->buffer[x->buffer_idx++] = (char)f;
    }
  }
}

void getline_setup(void)
{
  getline_class = class_new(gensym("getline"),
        (t_newmethod)getline_new,
        (t_method)getline_free, sizeof(t_getline),
        CLASS_DEFAULT, 0);

  class_addfloat(getline_class, getline_float);
};
