require 'spec_helper'

describe Hoge do
  subject { Hoge.new.hello }
  it { should be "hello world" }
end
