#include <new>
#include <ruby.h>
#include <ux/ux.hpp>
#include <vector>
#include <string>

VALUE rb_cHoge;
VALUE rb_cTextUx;

static VALUE hello(VALUE self) {
  return rb_str_new2("hello world");
}

static void  ux_free(ux::Trie* ptr);
static VALUE ux_alloc(VALUE klass);
static VALUE ux_initialize(VALUE self);
static VALUE ux_build(VALUE self, VALUE words, VALUE is_tail_ux);

extern "C" void Init_text_ux(void) {
  rb_cHoge = rb_define_class("Hoge", rb_cObject);
  rb_define_method(rb_cHoge, "hello", RUBY_METHOD_FUNC(hello), 0);

  rb_cTextUx = rb_define_class("TextUx", rb_cObject);
  rb_define_alloc_func(rb_cTextUx, ux_alloc);
  rb_define_private_method(rb_cTextUx, "initialize", RUBY_METHOD_FUNC(ux_initialize), 0);
  rb_define_method(rb_cTextUx, "build", RUBY_METHOD_FUNC(ux_build), 2);

  // std::string line = "hoge";
  // size_t retLen = 0;
  // id_t id = trie.prefixSearch(line.c_str(), line.size(), retLen);
  // printf("================\n");
  // printf("%d\n", id);
  // printf("================\n");
}

static void
ux_free(ux::Trie* ptr)
{
  ptr->~Trie();
  ruby_xfree(ptr);
}

static VALUE
ux_alloc(VALUE klass)
{
  ux::Trie *ptr = new ux::Trie();
  return Data_Wrap_Struct(klass, NULL, ux_free, ptr);
}

static VALUE
ux_initialize(VALUE self)
{
  // TODO: 配列で辞書データを受け取る
  return Qnil;
}

static VALUE
ux_build(VALUE self, VALUE words, VALUE is_tail_ux)
{
  Check_Type(words, T_ARRAY);

  std::vector<std::string> keyList;
  long len = RARRAY_LEN(words);
  for (long i = 0; i < len; i++) {
    VALUE rb_word = rb_ary_entry(words, i);
    std::string word = StringValuePtr(rb_word);
    keyList.push_back(std::string(word));
  }

  ux::Trie* trie;
  Data_Get_Struct(self, ux::Trie, trie);
  trie->build(keyList, true);

  return self;
}
