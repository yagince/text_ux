# TextUx

More Succinct Trie Data structure (binding for ux-trie)

## Installation

Add this line to your application's Gemfile:

    gem 'text_ux'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install text_ux

## Usage

```ruby
require "text_ux"

ux = TextUx.new
ux.build(%w( hoge foo bar ), true)

ux.prefix_search("hoge") # => hoge
ux.prefix_search("baz") # => nil
```

## Contributing

1. Fork it ( https://github.com/[my-github-username]/text_ux/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
