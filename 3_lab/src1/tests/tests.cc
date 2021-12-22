#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <iostream>

#include "../trie.h"
#include "../word_array.h"


TEST(Trie, Init) {
	Trie trie;
}

TEST(Trie, AddIn) {
	Trie trie;
	trie.add("asdf", 0);
	ASSERT_TRUE(trie.contains("asdf"));
}

TEST(Trie, AddMultiple) {
	Trie trie;
	trie.add("asdf", 0);
	ASSERT_FALSE(trie.contains("asd"));
	trie.add("qwer", 1);
	ASSERT_EQ(trie.get_index("qwer"), 1);
	trie.add("a", 2);
	ASSERT_EQ(trie.get_index("a"), 2);
}

TEST(Trie, GetByPrefix) {
	Trie trie;
	trie.add("aaaa", 0);
	trie.add("aaab", 1);
	trie.add("aaac", 2);
	trie.add("aabb", 3);

	std::vector<unsigned int> indices = trie.get_indices_by_prefix("aaa");
	sort(indices.begin(), indices.end());
	std::vector<unsigned int> expected = {0, 1, 2};
	ASSERT_TRUE(indices == expected);
}

TEST(Trie, SetIndex) {
	Trie trie;
	trie.add("a", 0);
	ASSERT_EQ(trie.get_index("a"), 0);
	trie.set_index("a", 1);
	ASSERT_EQ(trie.get_index("a"), 1);
}

TEST(Trie, CopyConstructor) {
	Trie trie1;
	trie1.add("ab", 0);
	trie1.add("ba", 1);
	trie1.add("ac", 2);

	Trie trie2(trie1);

	ASSERT_TRUE(trie2.contains("ab"));
	ASSERT_TRUE(trie2.get_index("ba") == 1);
	ASSERT_NE(&trie1, &trie2);
}

TEST(Trie, MoveConstructor) {
	std::vector<Trie> vec;
	vec.push_back(Trie());
	vec.push_back(Trie());

	vec[0].add("asdf", 0);
	vec[1].add("qwer", 0);

	ASSERT_TRUE(vec[0].contains("asdf"));
	ASSERT_TRUE(vec[1].contains("qwer"));
}

TEST(Trie, EqOperator) {
	Trie trie1;
	Trie trie2;

	trie1.add("asdf", 0);
	trie2 = trie1;
	
	ASSERT_NE(&trie1, &trie2);
	ASSERT_TRUE(trie2.contains("asdf"));
}

Trie op() {
	Trie trie;
	trie.add("asdf", 0);
	return trie;
}

TEST(Trie, ReturnFromFunction) {
	Trie trie = op();
	ASSERT_TRUE(trie.contains("asdf"));
}

TEST(WordArray, Init) {
	WordArray word_array;
}

TEST(WordArray, InitWithWord) {
	WordArray word_array("asdf");
}

TEST(WordArray, InitWithMultiple) {
	const char* words[3] = {"asdf", "qwer", "zxcv"};
	WordArray word_array(3, words);
}

TEST(WordArray, Add) {
	WordArray word_array;
	word_array.add("qwer");
}

TEST(WordArray, Index) {
	WordArray word_array;
	word_array.add("qwer");
	ASSERT_EQ(word_array.index("qwer"), 0);
	ASSERT_EQ(word_array.index("nonexistent"), -1);
}

TEST(WordArray, Get) {
	WordArray word_array;
	word_array.add("qwer");
	ASSERT_EQ(word_array.get(0), "qwer");
}

TEST(WordArray, Prefix) {
	WordArray word_array;
	word_array.add("aaaa");
	word_array.add("aaab");
	word_array.add("aaac");
	word_array.add("bbbb");

	WordArray* new_array = word_array.prefix("aaa");
	ASSERT_TRUE(new_array->index("aaaa") != -1);
	ASSERT_TRUE(new_array->index("aaab") != -1);
	ASSERT_TRUE(new_array->index("aaac") != -1);
	ASSERT_TRUE(new_array->index("bbbb") == -1);

	delete new_array;
}

TEST(WordArray, Unique) {
	WordArray word_array;
	word_array.add("aaaa");
	word_array.add("aaaa");

	ASSERT_EQ(word_array.length(), 1);
	ASSERT_EQ(word_array.index("aaaa"), 0);
}

TEST(WordArray, Output) {
	WordArray word_array;
	word_array.add("qwer");
	word_array.add("asdf");
	word_array.add("zxcv");

	std::cout << word_array;
}

TEST(WordArray, ImplicitCopy) {
	WordArray word_array1;

	word_array1.add("aaaa");
	word_array1.add("bbbb");

	WordArray word_array2(word_array1);

	ASSERT_TRUE(word_array2.index("aaaa") != -1);
	ASSERT_NE(&word_array1, &word_array2);
}

TEST(WordArray, ImplicitMove) {
	std::vector<WordArray> vec;
	vec.push_back(WordArray());
	vec.push_back(WordArray());

	vec[0].add("asdf");
	vec[1].add("qwer");

	ASSERT_TRUE(vec[0].index("asdf") != -1);
	ASSERT_TRUE(vec[1].index("qwer") != -1);
}

TEST(WordArray, ImplicitEq) {
	WordArray word_array1;
	word_array1.add("asdf");
	WordArray word_array2 = word_array1;

	ASSERT_TRUE(word_array2.index("asdf") != -1);
	ASSERT_TRUE(word_array2.index("zxcv") == -1);
}

TEST(WordArray, PlusOp) {
	WordArray word_array1;
	WordArray word_array2;

	word_array1.add("asdf");
	word_array2.add("qwer");

	WordArray word_array3 = word_array1 + word_array2;

	ASSERT_TRUE(word_array3.index("asdf") != -1);
	ASSERT_TRUE(word_array3.index("qwer") != -1);
}

TEST(WordArray, EqualsOp) {
	WordArray word_array1;
	WordArray word_array2;

	word_array1.add("asdf");
	word_array2.add("asdf");

	ASSERT_TRUE(word_array1 == word_array2);
	
	word_array2.add("qwer");

	ASSERT_TRUE(word_array1 != word_array2);

	word_array1.add("zxcv");

	ASSERT_TRUE(word_array1 != word_array2);
}

TEST(WordArray, MinusOp) {
	WordArray word_array1;
	WordArray word_array2;

	word_array1.add("asdf");
	word_array1.add("qwer");

	word_array2.add("asdf");

	WordArray word_array3 = word_array1 - word_array2;

	ASSERT_TRUE(word_array3.index("asdf") == -1);
	ASSERT_TRUE(word_array3.index("qwer") != -1);
}

TEST(WordArray, AndOp) {
	WordArray word_array1;
	WordArray word_array2;

	word_array1.add("asdf");
	word_array1.add("qwer");

	word_array2.add("asdf");

	WordArray word_array3 = word_array1 && word_array2;

	ASSERT_TRUE(word_array3.index("asdf") != -1);
	ASSERT_TRUE(word_array3.index("qwer") == -1);
}

TEST(WordArray, AccessOps) {
	WordArray word_array;
	word_array.add("qwer");
	ASSERT_EQ(word_array[0], "qwer");
}
