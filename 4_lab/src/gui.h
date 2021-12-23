#pragma once

#include <gtkmm.h>

#include "library.h"

class MainWindow : public Gtk::Window {
   private:
    std::shared_ptr<PubTable> table;
    Gtk::MenuBar menuBar;
    Gtk::MenuItem menuFile;
    Gtk::MenuItem menuEdit;
    Gtk::Menu subMenuEdit;
    Gtk::Menu subMenuFile;
    Gtk::MenuItem menuFileNew;
    Gtk::MenuItem menuFileOpen;
    Gtk::MenuItem menuFileSave;
    Gtk::MenuItem menuFileLoadDefault;
    Gtk::MenuItem menuEditNew;

   protected:
    Gtk::Box realMainBox;
    Gtk::Box mainBox;
    Gtk::ScrolledWindow scrollableWindow;
    Gtk::Box listBox;
    Gtk::Box viewBox;
    Gtk::TreeView tTreeView;
    Glib::RefPtr<Gtk::ListStore> tListStore;
    Gtk::Label viewLabel;
    Gtk::Separator separator;
    std::string current_activated = "";
    std::string current_path = "";
    std::string current_category = "scientific";

    Gtk::Box editBox;
    Gtk::ComboBoxText editCombo;
    Gtk::Entry editAuthor;
    Gtk::Entry editCode;
    Gtk::Entry editTitle;
    Gtk::Entry editPublisher;
    Gtk::Entry editGenre;
    Gtk::Button editSave;

    void on_button_add_clicked();

    class NumericEntry: public Gtk::Entry {
    	bool is_number(const std::string& s) {
	    std::string::const_iterator it = s.begin();
	    while (it != s.end() && std::isdigit(*it)) ++it;
	    return !s.empty() && it == s.end();
    	}


	void on_insert_text(const Glib::ustring& text, int* position) {
		if (is_number(text)) {
			Gtk::Entry::on_insert_text(text, position);
		}
	}
    };

    NumericEntry editYear;
    NumericEntry editAmount;
    Gtk::Entry editCourses[3];
    Gtk::Entry editGroups[8];

    void on_button_quit();
    void update_view();
    void on_button_file_clicked();
    bool on_key_press_event(GdkEventKey* key_event);
    void on_button_save_clicked();
    void on_button_new_clicked(Gtk::FileChooserAction action);
    void on_editcombo_changed();

    class tColumns : public Gtk::TreeModel::ColumnRecord {
       public:
	Gtk::TreeModelColumn<Glib::ustring> code;
	Gtk::TreeModelColumn<Glib::ustring> type;
	Gtk::TreeModelColumn<Glib::ustring> author;
	Gtk::TreeModelColumn<Glib::ustring> title;
	Gtk::TreeModelColumn<int> amount;
	Gtk::TreeModelColumn<int> year;
	Gtk::TreeModelColumn<Glib::ustring> publisher;

	tColumns() {
	    add(code);
	    add(type);
	    add(author);
	    add(title);
	    add(amount);
	    add(year);
	    add(publisher);
	}
    };

    tColumns columns;

   public:
    MainWindow();
    ~MainWindow();
    void on_treeview_row_activated(const Gtk::TreeModel::Path&,
                                   Gtk::TreeViewColumn*);
    void update_view_box(Gtk::Box&, std::string code);
    void set_table(std::shared_ptr<PubTable> table);
};
