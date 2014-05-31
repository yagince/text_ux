# -*- coding: utf-8 -*-
require 'spec_helper'

describe TextUx do
  let(:ux) { TextUx.new }
  let(:builded) { ux.build(%w(hoge foo bar), true) }
  
  describe :build do
    context "配列を渡した場合" do
      it { expect{ ux.build(%w(hoge foo bar), true) }.to_not raise_error }
    end

    context "配列以外を渡した場合" do
      it { expect{ ux.build( :hoge, true ) }.to raise_error }
    end
  end

  describe :save do
    let(:name) { "text.ux" }

    subject { builded.save(name) }

    after do
      File.exist?(name) and File.delete(name)
    end
    
    it "ファイルに保存される" do
      expect(subject).to be true
      expect(File.exist?(name)).to be true
    end
  end

  describe :prefix_search do

    context "結果がある場合" do
      it "一致するキーが取得できる" do
        expect(builded.prefix_search("hogeはfoo")).to eq("hoge")
      end
    end

    context "結果がない場合" do
      it { expect(builded.prefix_search("XXX")).to be_nil }
    end
  end
end

