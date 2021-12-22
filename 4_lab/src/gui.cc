#include "gui.h"

#include <gtkmm.h>
#include <sigc++/sigc++.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

#include "library.h"

MainWindow::MainWindow()
    : realMainBox(Gtk::ORIENTATION_VERTICAL),
      mainBox(Gtk::ORIENTATION_HORIZONTAL),
      listBox(Gtk::ORIENTATION_VERTICAL),
      viewBox(Gtk::ORIENTATION_VERTICAL),
      m_Button_Quit("Quit"),
      m_Button_Update("Update") {
    set_title("Nava");
    set_default_size(800, 600);

    menuFile.set_label("File");
    menuEdit.set_label("Edit");
    menuBar.append(menuFile);
    menuBar.append(menuEdit);
    menuFile.set_submenu(subMenuFile);
    menuEdit.set_submenu(subMenuEdit);
    subMenuEdit.append(menuEditNew);
    subMenuFile.append(menuFileNew);
    subMenuFile.append(menuFileOpen);
    subMenuFile.append(menuFileSave);
    subMenuFile.append(menuFileLoadDefault);

    menuEditNew.set_label("New Entry");
    menuEditNew.Gtk::Widget::add_accelerator("New Entry", get_accel_group(),
                                             'n', Gdk::CONTROL_MASK,
                                             Gtk::ACCEL_VISIBLE);
    menuEditNew.signal_activate().connect(
        [&]() { std::cout << "MainMenu/Edit/New Entry" << std::endl; });

    menuFileNew.set_label("New");
    menuFileNew.Gtk::Widget::add_accelerator(
        "New", get_accel_group(), 'n', Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
    menuFileNew.signal_activate().connect(
        [&]() { std::cout << "MainMenu/File/New" << std::endl; });

    menuFileOpen.set_label("Open");
    menuFileOpen.Gtk::Widget::add_accelerator(
        "Open", get_accel_group(), 'n', Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
    menuFileOpen.signal_activate().connect(
        sigc::mem_fun(*this, &MainWindow::on_button_file_clicked));

    menuFileSave.set_label("Save");
    menuFileSave.Gtk::Widget::add_accelerator(
        "Save", get_accel_group(), 'n', Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
    menuFileSave.signal_activate().connect(
        [&]() { std::cout << "MainMenu/File/Save" << std::endl; });

    menuFileLoadDefault.set_label("Load Default");
    menuFileLoadDefault.Gtk::Widget::add_accelerator(
        "LoadDefault", get_accel_group(), 'n', Gdk::CONTROL_MASK,
        Gtk::ACCEL_VISIBLE);
    menuFileLoadDefault.signal_activate().connect([&]() { update_view(); });

    // mainGrid.attach(listBox, 1, 1);
    listBox.pack_start(tTreeView);
    scrollableWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

    scrollableWindow.add(listBox);
    // PositionType.RIGHT
    realMainBox.pack_start(menuBar, Gtk::PACK_SHRINK);
    realMainBox.pack_start(mainBox);
    mainBox.pack_start(scrollableWindow, Gtk::PACK_SHRINK);
    mainBox.pack_start(viewBox, Gtk::PACK_SHRINK);
    viewBox.pack_start(viewLabel);
    viewLabel.set_justify(Gtk::JUSTIFY_FILL);
    // PositionType.BOTTOM
    add(realMainBox);

    tListStore = Gtk::ListStore::create(columns);
    tTreeView.set_model(tListStore);
    tTreeView.append_column("Code", columns.code);
    tTreeView.append_column("Type", columns.type);
    tTreeView.append_column("Author", columns.author);
    tTreeView.append_column("Title", columns.title);
    tTreeView.append_column("Amount", columns.amount);
    tTreeView.append_column("Year", columns.year);
    tTreeView.append_column("Publisher", columns.publisher);

    tTreeView.signal_row_activated().connect(
        sigc::mem_fun(*this, &MainWindow::on_treeview_row_activated));

    show_all();
}

void MainWindow::on_button_quit() { hide(); }

void MainWindow::set_table(std::shared_ptr<PubTable> table) {
    this->table = table;
}

void MainWindow::on_treeview_row_activated(const Gtk::TreeModel::Path& path,
                                           Gtk::TreeViewColumn* /* column */) {
    Gtk::TreeModel::iterator iter = tListStore->get_iter(path);
    if (iter) {
	Gtk::TreeModel::Row row = *iter;
	std::cout << "Row activated: Code=" << row[columns.code] << std::endl;
	std::stringstream ss;
	ss << row[columns.code];
	update_view_box(viewBox, ss.str());
    }
}

void MainWindow::update_view_box(Gtk::Box& box, std::string code) {
    std::cout << "Called update_view_box" << std::endl;
    // clean box
    auto children = box.get_children();

    for (auto v : children) {
	box.remove(*v);
    }

    std::shared_ptr<PubDescription> desc = table->get(code);
    if (desc->type() == "study") {
	auto desc1 = std::dynamic_pointer_cast<StudyPubDescription>(desc);
	viewLabel.set_text(desc1->get_description());
    } else if (desc->type() == "scientific") {
	auto desc1 = std::dynamic_pointer_cast<SciPubDescription>(desc);
	viewLabel.set_text(desc1->get_description());
    } else if (desc->type() == "fiction") {
	auto desc1 = std::dynamic_pointer_cast<FictionPubDescription>(desc);
	viewLabel.set_text(desc1->get_description());
    } else {
	throw std::logic_error("Got description of unknown type");
    }
    box.pack_start(viewLabel);
}

void MainWindow::update_view() {
    for (auto [key, value] : *this->table) {
	Gtk::TreeModel::Row row = *(tListStore->append());
	row[columns.code] = value->get_code();
	row[columns.type] = value->type();
	row[columns.author] = value->get_author();
	row[columns.title] = value->get_title();
	row[columns.amount] = value->get_amount();
	row[columns.year] = value->get_pub_year();
	row[columns.publisher] = value->get_publisher();
    }

    show_all_children();
}

void MainWindow::on_button_file_clicked() {
    Gtk::FileChooserDialog dialog("Choose library file",
                                  Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    // Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    // Add filters, so that only certain file types can be selected:

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("JSON");
    filter_text->add_mime_type("text/plain");
    filter_text->add_mime_type("application/json");
    filter_text->add_mime_type("text/json");
    dialog.add_filter(filter_text);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog.add_filter(filter_any);

    // Show the dialog and wait for a user response:
    int result = dialog.run();

    // Handle the response:
    switch (result) {
	case (Gtk::RESPONSE_OK): {
	    std::cout << "Open clicked." << std::endl;

	    // Notice that this is a std::string, not a Glib::ustring.
	    std::string filename = dialog.get_filename();
	    std::cout << "File selected: " << filename << std::endl;
	    this->table->load_json(filename);
	    this->update_view();
	    break;
	}
	case (Gtk::RESPONSE_CANCEL): {
	    std::cout << "Cancel clicked." << std::endl;
	    break;
	}
	default: {
	    std::cout << "Unexpected button clicked." << std::endl;
	    break;
	}
    }
}

MainWindow::~MainWindow() {}
