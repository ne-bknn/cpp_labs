#pragma once

#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

class PubDescription {
   protected:
    std::string code;
    std::string title;
    std::string author;
    std::string publisher;

    int pub_year;
    int amount = 0;

   public:
    int get_amount() const;
    void set_amount(int n);

    void set_code(const std::string& code);
    std::string get_code() const;

    void set_author(const std::string& author);
    std::string get_author() const;

    void set_title(const std::string& title);
    std::string get_title() const;

    void set_publisher(const std::string& code);
    std::string get_publisher() const;

    void set_pub_year(int year);
    int get_pub_year() const;

    virtual const std::string type() const { return "general"; }
    virtual const std::string get_description() const = 0;

    friend void to_json(json& j, const PubDescription& p);
    friend void from_json(const json& j, PubDescription& p);
};

class PubDescriptionWithCourses : public PubDescription {
   protected:
    std::vector<std::string> courses;

   public:
    void add_course(std::string course_name);
    bool edit_course(std::string before, std::string after);
    std::vector<std::string> get_courses() const;

    virtual const std::string type() const override {
	return "general_with_courses";
    }

    virtual const int max_courses() const {
	// -1 for unlimited
	return 0;
    }
};

class StudyPubDescription : public PubDescriptionWithCourses {
   protected:
    // RATIONALE: already implemented
    std::vector<std::string> group_indecies;

   public:
    void add_group(std::string group_name);
    virtual const std::string type() const override { return "study"; }

    virtual const int max_courses() const override { return 1; }
    virtual const std::string get_description() const override;

    friend void to_json(json& j, const PubDescription& p);
    friend void from_json(const json& j, PubDescription& p);
};

// RATIONALE: there's no explicit inheritance relation
// between Sci and Study; it's better to copypaste a bit
// I could create a common ancestor for both of them that inherits from
// PubDescription, but I think it's useless
//
// 20 minutes after: alright I created common ancestor
class SciPubDescription : public PubDescriptionWithCourses {
   public:
    virtual const std::string type() const override { return "scientific"; }
    virtual const int max_courses() const override { return 3; }
    virtual const std::string get_description() const override;

    friend void to_json(json& j, const PubDescription& p);
    friend void from_json(const json& j, PubDescription& p);
};

class FictionPubDescription : public PubDescription {
   protected:
    std::string genre;

   public:
    std::string get_genre() const;
    void set_genre(const std::string& new_genre);
    virtual const std::string type() const override { return "fiction"; }
    virtual const std::string get_description() const override;

    friend void to_json(json& j, const PubDescription& p);
    friend void from_json(const json& j, PubDescription& p);
};

class PubTable {
   private:
    // iterating over map is sorted with operator<, everything is fine
    std::map<std::string, std::shared_ptr<PubDescription>> _table;

   public:
    void add(const std::string& code, std::shared_ptr<PubDescription> desc);
    std::shared_ptr<PubDescription> get(const std::string& code) const;
    bool remove(const std::string& code);

    std::map<std::string, std::shared_ptr<PubDescription>>::iterator begin();
    std::map<std::string, std::shared_ptr<PubDescription>>::iterator end();
    void load_json(std::string filename);
    void save_json(std::string filename);
    void clean();
};

void to_json(json& j, const PubDescription& p);
void from_json(const json& j, PubDescription& p);
