#include <gtest/gtest.h>
#include "../src/library.h"
#include <exception>
#include <memory>

TEST(StudyPubDescription, Init) {
	StudyPubDescription desc;
	ASSERT_EQ(desc.type(), "study");

	desc.get_description();
	desc.get_amount();
}

TEST(StudyPubDescription, Amount) {
	StudyPubDescription desc;
	desc.set_amount(100);
	ASSERT_EQ(desc.get_amount(), 100);
}

TEST(StudyPubDescription, Courses) {
	StudyPubDescription desc;
	desc.add_course("Calculus I");
	EXPECT_THROW(desc.add_course("Calculus II"), std::logic_error);
	desc.edit_course("Calculus I", "Calculus II");
	ASSERT_EQ(desc.get_courses().size(), 1);
	ASSERT_EQ(desc.get_courses()[0], "Calculus II");
}

TEST(SciPubDescription, Courses) {
	SciPubDescription desc;
	desc.add_course("Calculus I");
	desc.add_course("Calculus II");

	for (auto const &course_name : desc.get_courses()) {
		ASSERT_TRUE(course_name == "Calculus I" || course_name == "Calculus II");
	}
	ASSERT_EQ(desc.get_courses().size(), 2);
}

TEST(PubTable, AddGet) {
	PubTable table;
	auto desc1 = std::make_shared<StudyPubDescription>();
	desc1->add_course("Differential Equations");
	table.add("DIFFEQ", desc1);
	std::shared_ptr<StudyPubDescription> desc1ref = std::dynamic_pointer_cast<StudyPubDescription>(table.get("DIFFEQ"));
	ASSERT_EQ(desc1ref->get_courses().size(), 1);
	ASSERT_EQ(desc1ref->get_courses()[0], "Differential Equations");
}

TEST(PubTable, NoSlicing) {
	PubTable table;
	
	auto desc1 = std::make_shared<StudyPubDescription>();
	auto desc2 = std::make_shared<FictionPubDescription>();

	desc1->add_course("Mananya Chubaba");
	desc2->set_genre("Mecha");

	table.add("MANANYA", desc1);
	table.add("EVA", desc2);

	for (const auto & [_, value] : table) {
		if (value->type() == "fiction") {
			auto desc_fiction = std::dynamic_pointer_cast<FictionPubDescription>(value);
			ASSERT_EQ(desc_fiction->get_genre(), "Mecha");
		} else if (value->type() == "study") {
			auto desc_study = std::dynamic_pointer_cast<StudyPubDescription>(value);
			ASSERT_EQ(desc_study->get_courses()[0], "Mananya Chubaba");
		} else {
			FAIL();
		}
	}
}

TEST(PubTable, Delete) {
	PubTable table;

	auto desc1 = std::make_shared<StudyPubDescription>();
	desc1->add_course("1234");
	auto desc2 = std::make_shared<FictionPubDescription>();
	desc2->set_genre("qwer");

	table.add("HELLO", desc1);
	table.add("GOODBYE", desc2);

	bool res = table.remove("GOODBYE");
	ASSERT_TRUE(res);

	res = table.remove("PRIVET");
	ASSERT_FALSE(res);
	
	auto desc2ref = std::dynamic_pointer_cast<StudyPubDescription>(table.get("HELLO"));
	ASSERT_EQ(desc2ref->get_courses()[0], "1234");

	EXPECT_THROW(table.get("GOODBYE"), std::out_of_range);
}

TEST(PubTable, Output) {
}
