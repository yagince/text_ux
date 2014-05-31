# -*- coding: utf-8 -*-
require 'spec_helper'

describe TextUx do
  let(:ux) { TextUx.new }
  
  describe :build do
    context "配列を渡した場合" do
      it { expect{ ux.build(%w(hoge foo bar), true) }.to_not raise_error }
    end

    context "配列以外を渡した場合" do
      it { expect{ ux.build( :hoge, true ) }.to raise_error }
    end
  end
end

