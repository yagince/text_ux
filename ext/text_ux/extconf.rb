require 'mkmf'

if have_library('ux')
  create_makefile("text_ux/text_ux")
end
