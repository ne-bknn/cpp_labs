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
      editBox(Gtk::ORIENTATION_VERTICAL) {

    // global
    set_title("Nava");
    set_default_size(1200, 800);

    // menu
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
    menuEditNew.Gtk::Widget::add_accelerator("New Entry", get_accel_group(), 'n', Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
    menuEditNew.signal_activate().connect(
        [&]() { std::cout << "MainMenu/Edit/New Entry" << std::endl; });

    menuFileNew.set_label("New");
    menuFileNew.Gtk::Widget::add_accelerator(
        "New", get_accel_group(), 'n', Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
    menuFileNew.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::on_button_new_clicked), Gtk::FILE_CHOOSER_ACTION_SAVE));

    menuFileOpen.set_label("Open");
    menuFileOpen.Gtk::Widget::add_accelerator(
        "Open", get_accel_group(), 'n', Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
    menuFileOpen.signal_activate().connect(
        sigc::mem_fun(*this, &MainWindow::on_button_file_clicked));

    menuFileSave.set_label("Save");
    menuFileSave.Gtk::Widget::add_accelerator(
        "Save", get_accel_group(), 'n', Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
    menuFileSave.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_button_save_clicked));

    menuFileLoadDefault.set_label("Load Default");
    menuFileLoadDefault.Gtk::Widget::add_accelerator(
        "LoadDefault", get_accel_group(), 'n', Gdk::CONTROL_MASK,
        Gtk::ACCEL_VISIBLE);
    menuFileLoadDefault.signal_activate().connect([&]() { update_view(); });


    // editBox
    editCombo.append("scientific");
    editCombo.append("study");
    editCombo.append("fiction");

    editCombo.set_active(0);
    editCombo.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_editcombo_changed));
    editBox.pack_start(editCombo, Gtk::PACK_SHRINK);

    editBox.pack_start(editAuthor, Gtk::PACK_SHRINK);
    editAuthor.set_placeholder_text("Author");

    editBox.pack_start(editTitle, Gtk::PACK_SHRINK);
    editTitle.set_placeholder_text("Title");

    editBox.pack_start(editCode, Gtk::PACK_SHRINK);
    editCode.set_placeholder_text("Code");

    editBox.pack_start(editPublisher, Gtk::PACK_SHRINK);
    editPublisher.set_placeholder_text("Publisher");

    editBox.pack_start(editYear, Gtk::PACK_SHRINK);
    editYear.set_placeholder_text("Year");

    editBox.pack_start(editAmount, Gtk::PACK_SHRINK);
    editAmount.set_placeholder_text("Amount");

    editBox.pack_start(editGenre, Gtk::PACK_SHRINK);
    editGenre.set_placeholder_text("Genre");
    editGenre.set_editable(false);


    for (int index = 0; index < 3; ++index) {
	    std::stringstream ss;
	    ss << "Course " << index;
	    editBox.pack_start(editCourses[index], Gtk::PACK_SHRINK);
	    editCourses[index].set_placeholder_text(ss.str());
    }

    for (int index = 0; index < 8; ++index) {
	    std::stringstream ss;
	    ss << "Group " << index;
	    editBox.pack_start(editGroups[index], Gtk::PACK_SHRINK);
	    editGroups[index].set_placeholder_text(ss.str());
	    // since we are starting with scientific
	    editGroups[index].set_editable(false);
    }

    editBox.pack_start(editSave, Gtk::PACK_SHRINK);
    editSave.set_sensitive(false);
    editSave.set_label("Add");
    editSave.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_add_clicked));

    // mainGrid.attach(listBox, 1, 1);
    listBox.pack_start(tTreeView);
    scrollableWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

    scrollableWindow.add(listBox);
    // PositionType.RIGHT
    realMainBox.pack_start(menuBar, Gtk::PACK_SHRINK);
    realMainBox.pack_start(mainBox);
    mainBox.pack_start(scrollableWindow, Gtk::PACK_SHRINK);
    mainBox.pack_start(viewBox, Gtk::PACK_SHRINK);
    mainBox.pack_start(separator, Gtk::PACK_SHRINK, 5);
    mainBox.pack_start(editBox, Gtk::PACK_SHRINK);
    viewBox.pack_start(viewLabel);
    viewLabel.set_justify(Gtk::JUSTIFY_FILL);

    
    // PositionType.BOTTOM
    add(realMainBox);

    // treeview
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

    add_events(Gdk::KEY_PRESS_MASK);

    show_all();
}

bool MainWindow::on_key_press_event(GdkEventKey* key_event) {
  //GDK_MOD1_MASK -> the 'alt' key(mask)
  //GDK_KEY_1 -> the '1' key
  //GDK_KEY_2 -> the '2' key

  //select the first radio button, when we press alt + 1
  if (key_event->keyval == GDK_KEY_Escape) {
    hide();
    return true;
  }

  if (key_event->keyval == GDK_KEY_Delete) {
	std::cout << "in delete handler" << std::endl;
	if (this->current_activated != "") {
		std::cout << "Current code is " << this->current_activated << std::endl;
		this->table->remove(this->current_activated);
		this->update_view();
		return true;
	}
  }
  return Gtk::Window::on_key_press_event(key_event);
}

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
	this->current_activated = ss.str();
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
    tListStore->clear();
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
	    this->current_path = filename;
	    this->editSave.set_sensitive(true);
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

void MainWindow::on_button_save_clicked() {
	if (this->current_path == "") {
		throw std::logic_error("No current path is specified");
	}
	this->table->save_json(this->current_path);
}

void MainWindow::on_button_new_clicked(Gtk::FileChooserAction action) {
  const bool open = (action == Gtk::FILE_CHOOSER_ACTION_OPEN);
  auto dialog = Gtk::FileChooserNative::create("Please choose a file", *this, action);

  // Show the dialog and wait for a user response:
  const int result = dialog->run();

  // Handle the response:
  switch (result)
  {
  case Gtk::RESPONSE_ACCEPT:
  {
    // Notice that this is a std::string, not a Glib::ustring.
    auto filename = dialog->get_filename();
    std::cout << "File selected: " <<  filename << std::endl;
    this->current_path = filename;
    this->editSave.set_sensitive(true);
    break;
  }

  case Gtk::RESPONSE_CANCEL:
    std::cout << "Cancel clicked." << std::endl;
    break;

  case Gtk::RESPONSE_DELETE_EVENT:
    std::cout << "Dialog closed." << std::endl;
    break;

  default:
    std::cout << "Unexpected button clicked." << std::endl;
    break;
  }
}

void MainWindow::on_editcombo_changed() {
	std::string text = this->editCombo.get_active_text();
	if (text != "scientific" && text != "fiction" && text != "study") {
		throw std::logic_error("Got unkown pub type");
	}

	if (text == "scientific") {
		std::cout << "Creating scientific" << std::endl;
		for (int index = 0; index < 3; ++index) {
			editCourses[index].set_editable(true);
		}
		for (int index = 0; index < 8; ++index) {
		    editGroups[index].set_editable(false);
		    editGroups[index].set_text("");
    		}
		editGenre.set_editable(false);
		editGenre.set_text("");
	} else if (text == "study") {
		std::cout << "Creating study" << std::endl;
		editCourses[0].set_editable(true);
		for (int index = 1; index < 3; ++index) {
			editCourses[index].set_editable(false);
			editCourses[index].set_text("");
		}

		for (int index = 0; index < 8; ++index) {
		    editGroups[index].set_editable(true);
    		}

		editGenre.set_editable(false);
		editGenre.set_text("");
	} else if (text == "fiction") {
		std::cout << "Creating fiction" << std::endl;
		for (int index = 0; index < 8; ++index) {
		    editGroups[index].set_editable(false);
		    editGroups[index].set_text("");
    		}
		for (int index = 0; index < 3; ++index) {
		    editCourses[index].set_editable(false);
		    editCourses[index].set_text("");
    		}
		editGenre.set_editable(true);
	}
	this->current_category = text;
}

void MainWindow::on_button_add_clicked() {
    // construct an object
    std::string author = editAuthor.get_text();
    std::string title = editTitle.get_text();
    std::string code = editCode.get_text();
    std::string publisher = editPublisher.get_text();
    int year = std::stoi(editYear.get_text());
    int amount = std::stoi(editAmount.get_text());

    if (this->current_category == "scientific") {
	auto pub = std::make_shared<SciPubDescription>();
	pub->set_author(author);
	pub->set_title(title);
	pub->set_code(code);
	pub->set_publisher(publisher);
	pub->set_pub_year(year);
	pub->set_amount(amount);

	for (int i = 0; i < 3; ++i) {
		if (editCourses[i].get_text() != "") {
			pub->add_course(editCourses[i].get_text());
		}
	}

	this->table->add(code, pub);
	this->update_view();
    } else if (this->current_category == "study") {
	auto pub = std::make_shared<StudyPubDescription>();
	pub->set_author(author);
	pub->set_title(title);
	pub->set_code(code);
	pub->set_publisher(publisher);
	pub->set_pub_year(year);
	pub->set_amount(amount);

	if (editCourses[0].get_text() != "") {
		pub->add_course(editCourses[0].get_text());
	}

	for (int i = 0; i < 8; ++i) {
		if (editGroups[i].get_text() != "") {
			pub->add_course(editGroups[i].get_text());
		}
	}

	this->table->add(code, pub);
	this->update_view();
    } else if (this->current_category == "fiction") {
	auto pub = std::make_shared<FictionPubDescription>();
	pub->set_author(author);
	pub->set_title(title);
	pub->set_code(code);
	pub->set_publisher(publisher);
	pub->set_pub_year(year);
	pub->set_amount(amount);
	std::string genre = editGenre.get_text();
	pub->set_genre(genre);

	this->table->add(code, pub);
	this->update_view();
    } else {
	    throw std::logic_error("Got unknown pub type");
    }

    // clean
    editAuthor.set_text("");
    editTitle.set_text("");
    editCode.set_text("");
    editPublisher.set_text("");
    editYear.set_text("");
    editAmount.set_text("");
    editGenre.set_text("");

    for (int index = 0; index < 3; ++index) {
	    editCourses[index].set_text("");
    }

    for (int index = 0; index < 8; ++index) {
	    editGroups[index].set_text("");
    }
}

MainWindow::~MainWindow() {}
