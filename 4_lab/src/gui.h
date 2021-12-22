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
    Gtk::Button m_Button_Quit;
    Gtk::Button m_Button_Update;
    Gtk::Label viewLabel;

    void on_button_quit();
    void update_view();

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
    void on_treeview_row_activated(const Gtk::TreeModel::Path&, Gtk::TreeViewColumn*);
    void update_view_box(Gtk::Box&, std::string code);
    void set_table(std::shared_ptr<PubTable> table);
};
