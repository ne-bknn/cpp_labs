#include <gtkmm.h>

#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

#include "gui.h"
#include "library.h"
using json = nlohmann::json;

int main() {
    auto app = Gtk::Application::create("org.gtkmm.example");
    MainWindow t;
    auto table = std::make_shared<PubTable>();

    table->load_json("library.json");

    t.set_table(table);

    app->run(t);
    return 0;
}
