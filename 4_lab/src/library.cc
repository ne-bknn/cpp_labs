// PubDescription
#include "library.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// Impl::PubDescription

int PubDescription::get_amount() const { return this->amount; }
void PubDescription::set_amount(int n) { this->amount = n; }

std::string PubDescription::get_code() const { return this->code; }
void PubDescription::set_code(const std::string &code) { this->code = code; }

std::string PubDescription::get_author() const { return this->author; }
void PubDescription::set_author(const std::string &author) {
    this->author = author;
}

std::string PubDescription::get_title() const { return this->title; }
void PubDescription::set_title(const std::string &title) {
    this->title = title;
}

std::string PubDescription::get_publisher() const { return this->publisher; }
void PubDescription::set_publisher(const std::string &publisher) {
    this->publisher = publisher;
}

int PubDescription::get_pub_year() const { return this->pub_year; }
void PubDescription::set_pub_year(int n) { this->pub_year = n; }

// Impl::PubDescriptionWithCourses

void PubDescriptionWithCourses::add_course(std::string course_name) {
    if (this->courses.size() == this->max_courses()) {
	throw std::logic_error("Max amount of courses reached");
    }
    this->courses.push_back(course_name);
}

bool PubDescriptionWithCourses::edit_course(const std::string before,
                                            const std::string after) {
    for (auto i1 = this->courses.begin(); i1 != this->courses.end(); ++i1) {
	if (*i1 == before) {
	    *i1 = after;
	}
	return true;
    }
    return false;
}

std::vector<std::string> PubDescriptionWithCourses::get_courses() const {
    return this->courses;
}

// Impl::PubTable

void PubTable::add(const std::string &code,
                   std::shared_ptr<PubDescription> desc) {
    // TODO: possible memory leak if overwriting exisiting pub with this code
    this->_table[code] = desc;
}

std::shared_ptr<PubDescription> PubTable::get(const std::string &code) const {
    auto it = this->_table.find(code);
    if (it != this->_table.end()) {
	return (*it).second;
    } else {
	throw std::out_of_range("No such code exists");
    }
}

bool PubTable::remove(const std::string &code) {
    auto it = this->_table.find(code);
    if (it != this->_table.end()) {
	this->_table.erase(it);
	return true;
    } else {
	return false;
    }
}

std::map<std::string, std::shared_ptr<PubDescription>>::iterator
PubTable::begin() {
    return this->_table.begin();
}

std::map<std::string, std::shared_ptr<PubDescription>>::iterator
PubTable::end() {
    return this->_table.end();
}

// Impl::FictionPubDescription

std::string FictionPubDescription::get_genre() const { return this->genre; }

void FictionPubDescription::set_genre(const std::string &new_genre) {
    this->genre = new_genre;
}

const std::string FictionPubDescription::get_description() const {
    std::stringstream ss;
    ss << "Title: " << this->get_title() << std::endl;
    ss << "Author: " << this->get_author() << std::endl;
    ss << "Genre: " << this->get_genre() << std::endl;
    return ss.str();
}

const std::string SciPubDescription::get_description() const {
    std::stringstream ss;
    ss << "Title: " << this->get_title() << std::endl;
    ss << "Author: " << this->get_author() << std::endl;
    return ss.str();
}

const std::string StudyPubDescription::get_description() const {
    std::stringstream ss;
    ss << "Title: " << this->get_title() << std::endl;
    ss << "Author: " << this->get_author() << std::endl;
    return ss.str();
}

void to_json(json &j, const PubDescription &p) {
    if (p.type() == "fiction") {
	auto derived_ref = dynamic_cast<const FictionPubDescription &>(p);
	j = json{{"title", p.title},   {"author", p.author},
	         {"code", p.code},     {"genre", derived_ref.genre},
	         {"year", p.pub_year}, {"publisher", p.publisher},
	         {"amount", p.amount}};
    } else if (p.type() == "scientific") {
	auto derived_ref = dynamic_cast<const SciPubDescription &>(p);
	json courses(derived_ref.courses);
	j = json{{"title", p.title},   {"author", p.author},
	         {"code", p.code},     {"year", p.pub_year},
	         {"amount", p.amount}, {"publisher", p.publisher},
	         {"courses", courses}};
    } else if (p.type() == "study") {
	auto derived_ref = dynamic_cast<const StudyPubDescription &>(p);
	json courses(derived_ref.courses);
	json groups(derived_ref.group_indecies);
	j = json{{"title", p.title},   {"author", p.author},
	         {"code", p.code},     {"year", p.pub_year},
	         {"amount", p.amount}, {"publisher", p.publisher},
	         {"courses", courses}, {"group_indecies", groups}};
    } else {
	throw std::logic_error("Unknown pub type");
    }
}

void from_json(const json &j, PubDescription &p) {
    j.at("title").get_to(p.title);
    j.at("author").get_to(p.author);
    j.at("code").get_to(p.code);
    j.at("amount").get_to(p.amount);
    j.at("year").get_to(p.pub_year);
    j.at("publisher").get_to(p.publisher);
    if (j.at("type") == "fiction") {
	j.at("genre").get_to(dynamic_cast<FictionPubDescription &>(p).genre);
    }

    if (j.at("type") == "scientific") {
	j.at("courses").get_to(dynamic_cast<SciPubDescription &>(p).courses);
    }

    if (j.at("type") == "study") {
	j.at("courses").get_to(dynamic_cast<StudyPubDescription &>(p).courses);
	j.at("group_indecies")
	    .get_to(dynamic_cast<StudyPubDescription &>(p).group_indecies);
    }
}

void PubTable::load_json(std::string filename) {
    this->clean();
    std::ifstream t(filename);
    json table;
    t >> table;
    for (json::iterator it = table.begin(); it != table.end(); ++it) {
	if ((*it)["type"] == "fiction") {
	    FictionPubDescription pub = (*it).get<FictionPubDescription>();
	    std::shared_ptr<FictionPubDescription> pub_ptr(
	        new FictionPubDescription(pub));
	    this->add(pub_ptr->get_code(), pub_ptr);
	} else if ((*it)["type"] == "scientific") {
	    SciPubDescription pub = (*it).get<SciPubDescription>();
	    std::shared_ptr<SciPubDescription> pub_ptr(
	        new SciPubDescription(pub));
	    this->add(pub_ptr->get_code(), pub_ptr);
	} else if ((*it)["type"] == "study") {
	    StudyPubDescription pub = (*it).get<StudyPubDescription>();
	    std::shared_ptr<StudyPubDescription> pub_ptr(
	        new StudyPubDescription(pub));
	    this->add(pub_ptr->get_code(), pub_ptr);
	} else {
	    throw std::logic_error("Unknown pub type supplied");
	}
    }
}

void PubTable::clean() { this->_table.clear(); }
