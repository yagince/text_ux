#include <ruby.h>

static VALUE hello(VALUE self) {
  return rb_str_new2("hello world");
}

void Init_text_ux(void) {
  VALUE rb_cHoge = rb_define_class("Hoge", rb_cObject);
  rb_define_method(rb_cHoge, "hello", hello, 0);
}
