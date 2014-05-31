# -*- coding: utf-8 -*-
require 'spec_helper'

describe TextUx do
  let(:ux) { TextUx.new }
  let(:builded) { ux.build(%w(hoge foo bar baz footprint), true) }
  
  describe :build do
    context "配列を渡した場合" do
      it { expect{ ux.build(%w(hoge foo bar), true) }.to_not raise_error }
    end

    context "配列以外を渡した場合" do
      it { expect{ ux.build( :hoge, true ) }.to raise_error }
    end
  end

  describe :save do
    let(:name) { "test.ux" }

    subject { builded.save(name) }

    after do
      File.exist?(name) and File.delete(name)
    end
    
    it "ファイルに保存される" do
      expect(subject).to be builded
      expect(File.exist?(name)).to be true
    end
  end

  describe :load do
    let(:name) { "spec/fixtures/test.ux" }

    subject { ux.load(name) }

    it { expect(subject.prefix_search("hoge----foo")).to eq("hoge") }

    context "loadに失敗した場合" do
      it "例外がraiseされる" do
        expect{ ux.load("hoge") }.to raise_error
      end
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


  describe :common_prefix_search do

    context "結果がある場合" do
      it "一致するキーが全て取得できる" do
        expect(builded.common_prefix_search("footprint")).to match_array(%w(foo footprint))
      end
    end

    context "結果がない場合" do
      it { expect(builded.common_prefix_search("XXX")).to be_empty }
    end
  end

  describe :predictive_search do

    context "結果がある場合" do
      it "一致するキーが全て取得できる" do
        expect(builded.predictive_search("fo")).to match_array(%w(foo footprint))
      end
    end

    context "結果がない場合" do
      it { expect(builded.predictive_search("XXX")).to be_empty }
    end
  end

  describe :decode_key do
    context "結果がある場合" do
      it "一致するキーが取得できる" do
        expect(builded.decode_key(0)).to eq("hoge")
      end
    end

    context "結果がない場合" do
      it { expect(builded.decode_key(100000)).to be nil }
    end
  end

  describe :size do
    it { expect(builded.size).to eq(5) }
  end
  describe :clear do
    it "空になる" do
      expect(builded.clear).to be true
      expect(builded.size).to eq(0)
    end
  end
  describe :alloc_size do
    it { expect(builded.alloc_size).to be >= 0 }
  end
  describe :alloc_stat do
    it { expect(builded.alloc_stat).to_not be_empty }
  end

end

