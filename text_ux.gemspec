# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'text_ux/version'

Gem::Specification.new do |spec|
  spec.name          = "text_ux"
  spec.version       = TextUx::VERSION
  spec.authors       = ["yagince"]
  spec.email         = ["straitwalk@gmail.com"]
  spec.summary       = %q{Text::Ux - More Succinct Trie Data structure (binding for ux-trie)}
  spec.description   = %q{Text::Ux - More Succinct Trie Data structure (binding for ux-trie)}
  spec.homepage      = ""
  spec.license       = "MIT"
  spec.extensions    = ["ext/text_ux/extconf.rb"]

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.6"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "rspec", '~> 2.14.1'
end
