require 'spec_helper'

describe Hoge do
  subject { Hoge.new.hello }
  it { should eq "hello world" }
end
