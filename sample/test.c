#include <ux/ux.hpp>
#include <vector>

vector keyList;
keyList.push_back(string("レッドブル")) // 辞書登録したいキーワード集合
keyList.push_back(string("ウォッカ")) // 辞書登録したいキーワード集合

ux::Trie trie(keyList);
