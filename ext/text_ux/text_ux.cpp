#include <new>
#include <ruby.h>
#include <ux/ux.hpp>
#include <vector>
#include <string>

VALUE rb_cTextUx;

static void  ux_free(ux::Trie* ptr);
static VALUE ux_alloc(VALUE klass);
static VALUE ux_initialize(VALUE self);
static VALUE ux_build(VALUE self, VALUE words, VALUE is_tail_ux);
static VALUE ux_save(VALUE self, VALUE file_name);
static VALUE ux_prefix_search(VALUE self, VALUE word);

extern "C" void Init_text_ux(void) {
  rb_cTextUx = rb_define_class("TextUx", rb_cObject);
  rb_define_alloc_func(rb_cTextUx, ux_alloc);
  rb_define_private_method(rb_cTextUx, "initialize", RUBY_METHOD_FUNC(ux_initialize), 0);
  rb_define_method(rb_cTextUx, "build", RUBY_METHOD_FUNC(ux_build), 2);
  rb_define_method(rb_cTextUx, "save", RUBY_METHOD_FUNC(ux_save), 1);
  rb_define_method(rb_cTextUx, "prefix_search", RUBY_METHOD_FUNC(ux_prefix_search), 1);

  rb_define_const(rb_cTextUx, "NOTFOUND", ux::NOTFOUND);
}

static ux::Trie* getUx(VALUE self) {
  ux::Trie* p;
  Data_Get_Struct(self, ux::Trie, p);
  return p;
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

static VALUE
ux_save(VALUE self, VALUE file_name)
{
  Check_Type(file_name, T_STRING);

  std::string f = StringValuePtr(file_name);
  ux::Trie* trie = getUx(self);
  if (trie->save(f.c_str()) != ux::Trie::SUCCESS ) {
    rb_exc_raise(rb_str_new2("SaveFailed."));
  }
  return Qtrue;
}

static VALUE
ux_prefix_search(VALUE self, VALUE word)
{
  Check_Type(word, T_STRING);

  std::string w = StringValuePtr(word);

  ux::Trie* trie;
  Data_Get_Struct(self, ux::Trie, trie);
  size_t ret_len;
  ux::id_t id = trie->prefixSearch(w.c_str(), w.size(), ret_len);
  if ( id == ux::NOTFOUND ) {
    return Qnil;
  } else {
    std::string key = trie->decodeKey(id);
    return rb_str_new2(key.c_str());
  }
}
