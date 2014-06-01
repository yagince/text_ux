#include <new>
#include <ruby.h>
#include <ux/ux.hpp>
#include <vector>
#include <string>
#include <sstream>

VALUE rb_cTextUx;

static void  ux_free(ux::Trie* ptr);
static VALUE ux_alloc(VALUE klass);
static VALUE ux_initialize(VALUE self);
static VALUE ux_build(int argc, VALUE *argv, VALUE self);
static VALUE ux_save(VALUE self, VALUE file_name);
static VALUE ux_load(VALUE self, VALUE file_name);

static VALUE ux_prefix_search(VALUE self, VALUE word);
static VALUE ux_common_prefix_search(int argc, VALUE *argv, VALUE self);
static VALUE ux_predictive_search(int argc, VALUE *argv, VALUE self);
static VALUE ux_decode_key(VALUE self, VALUE index);

static VALUE ux_size(VALUE self);
static VALUE ux_clear(VALUE self);
static VALUE ux_alloc_size(VALUE self);
static VALUE ux_alloc_stat(VALUE self);

extern "C" void Init_text_ux(void) {
  rb_cTextUx = rb_define_class("TextUx", rb_cObject);
  rb_define_alloc_func(rb_cTextUx, ux_alloc);
  rb_define_private_method(rb_cTextUx, "initialize", RUBY_METHOD_FUNC(ux_initialize), 0);
  rb_define_method(rb_cTextUx, "build", RUBY_METHOD_FUNC(ux_build), -1);
  rb_define_method(rb_cTextUx, "save", RUBY_METHOD_FUNC(ux_save), 1);
  rb_define_method(rb_cTextUx, "load", RUBY_METHOD_FUNC(ux_load), 1);

  rb_define_method(rb_cTextUx, "prefix_search", RUBY_METHOD_FUNC(ux_prefix_search), 1);
  rb_define_method(rb_cTextUx, "common_prefix_search", RUBY_METHOD_FUNC(ux_common_prefix_search), -1);
  rb_define_method(rb_cTextUx, "predictive_search", RUBY_METHOD_FUNC(ux_predictive_search), -1);
  rb_define_method(rb_cTextUx, "decode_key", RUBY_METHOD_FUNC(ux_decode_key), 1);

  rb_define_method(rb_cTextUx, "size", RUBY_METHOD_FUNC(ux_size), 0);
  rb_define_method(rb_cTextUx, "clear", RUBY_METHOD_FUNC(ux_clear), 0);
  rb_define_method(rb_cTextUx, "alloc_size", RUBY_METHOD_FUNC(ux_alloc_size), 0);
  rb_define_method(rb_cTextUx, "alloc_stat", RUBY_METHOD_FUNC(ux_alloc_stat), 0);

  rb_define_const(rb_cTextUx, "DEFAULT_LIMIT", ux::LIMIT_DEFAULT);
}

static ux::Trie* getTrie(VALUE self) {
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
// ux_build(VALUE self, VALUE words, VALUE is_tail_ux)
ux_build(int argc, VALUE *argv, VALUE self)
{
  VALUE words, is_tail_ux;

  rb_scan_args(argc, argv, "11", &words, &is_tail_ux);

  Check_Type(words, T_ARRAY);
  if ( is_tail_ux == Qnil ) {
    is_tail_ux = Qtrue;
  }

  std::vector<std::string> keyList;
  long len = RARRAY_LEN(words);
  for (long i = 0; i < len; i++) {
    VALUE rb_word = rb_ary_entry(words, i);
    std::string word = StringValuePtr(rb_word);
    keyList.push_back(std::string(word));
  }

  ux::Trie* trie = getTrie(self);
  trie->build(keyList, RTEST(is_tail_ux));

  return self;
}

static VALUE
ux_save(VALUE self, VALUE file_name)
{
  Check_Type(file_name, T_STRING);

  std::string f = StringValuePtr(file_name);
  ux::Trie* trie = getTrie(self);
  if (trie->save(f.c_str()) != ux::Trie::SUCCESS ) {
    rb_exc_raise(rb_str_new2("Save Error"));
  }
  return self;
}

static VALUE
ux_load(VALUE self, VALUE file_name)
{
  Check_Type(file_name, T_STRING);

  std::string f = StringValuePtr(file_name);
  ux::Trie* trie = getTrie(self);
  if (trie->load(f.c_str()) != ux::Trie::SUCCESS ) {
    rb_exc_raise(rb_str_new2("Load Error"));
  }
  return self;
}

static VALUE
ux_prefix_search(VALUE self, VALUE word)
{
  Check_Type(word, T_STRING);

  std::string w = StringValuePtr(word);

  ux::Trie* trie = getTrie(self);
  size_t ret_len;
  ux::id_t id = trie->prefixSearch(w.c_str(), w.size(), ret_len);
  if ( id == ux::NOTFOUND ) {
    return Qnil;
  } else {
    std::string key = trie->decodeKey(id);
    return rb_str_new2(key.c_str());
  }
}

static VALUE
ux_common_prefix_search(int argc, VALUE *argv, VALUE self)
{
  VALUE word, _limit;

  rb_scan_args(argc, argv, "11", &word, &_limit);

  Check_Type(word, T_STRING);
  std::string w = StringValuePtr(word);

  size_t limit;
  if (_limit == Qnil) {
    limit = ux::LIMIT_DEFAULT;
  } else {
    limit = NUM2INT(_limit);
  }

  ux::Trie* trie = getTrie(self);
  std::vector<ux::id_t> ret_ids;
  size_t keys_size  = trie->commonPrefixSearch(w.c_str(), w.size(), ret_ids, limit);
  VALUE ary = rb_ary_new();
  if (keys_size == 0) {
    return ary;
  }
  for (size_t i = 0; i < keys_size; i++) {
    std::string key = trie->decodeKey(ret_ids[i]);
    rb_ary_push(ary, rb_str_new2(key.c_str()));
  }
  return ary;
}

static VALUE
ux_predictive_search(int argc, VALUE *argv, VALUE self)
{
  VALUE word, _limit;

  rb_scan_args(argc, argv, "11", &word, &_limit);

  Check_Type(word, T_STRING);
  std::string w = StringValuePtr(word);

  size_t limit;
  if (_limit == Qnil) {
    limit = ux::LIMIT_DEFAULT;
  } else {
    limit = NUM2INT(_limit);
  }

  ux::Trie* trie = getTrie(self);
  std::vector<ux::id_t> ret_ids;
  size_t keys_size  = trie->predictiveSearch(w.c_str(), w.size(), ret_ids, limit);
  VALUE ary = rb_ary_new();
  if (keys_size == 0) {
    return ary;
  }
  for (size_t i = 0; i < keys_size; i++) {
    std::string key = trie->decodeKey(ret_ids[i]);
    rb_ary_push(ary, rb_str_new2(key.c_str()));
  }
  return ary;
}

static VALUE
ux_decode_key(VALUE self, VALUE index)
{
  
  ux::id_t idx = NUM2INT(index);
  ux::Trie* trie = getTrie(self);

  if (trie->size() <= idx) {
    return Qnil;
  }

  std::string key = trie->decodeKey(idx);
  return key.empty() ? Qnil : rb_str_new2(key.c_str());
}

static VALUE
ux_size(VALUE self)
{
  ux::Trie* trie = getTrie(self);
  size_t size = trie->size();
  return INT2NUM(size);
}
static VALUE
ux_clear(VALUE self)
{
  ux::Trie* trie = getTrie(self);
  trie->clear();
  return Qtrue;
}
static VALUE
ux_alloc_size(VALUE self)
{
  ux::Trie* trie = getTrie(self);
  size_t size = trie->getAllocSize();
  return INT2FIX(size);
}
static VALUE
ux_alloc_stat(VALUE self)
{
  ux::Trie* trie = getTrie(self);
  std::ostringstream os;
  trie->stat(os);

  return rb_str_new2(os.str().c_str());
}
