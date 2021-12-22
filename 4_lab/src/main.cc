#include "gui.h"
#include "library.h"
#include <gtkmm.h>
#include <iostream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main() {
    auto app = Gtk::Application::create("org.gtkmm.example");
    MainWindow t;
    auto table = std::make_shared<PubTable>();
    auto desc1 = std::make_shared<StudyPubDescription>();

    desc1->set_amount(5);
    desc1->set_code("HELLO");
    desc1->set_author("Demidovich");
    desc1->set_title("Матанализ");
    desc1->set_publisher("Printing Machine");
    desc1->set_pub_year(1850);

    auto desc2 = std::make_shared<FictionPubDescription>();

    desc2->set_amount(6);
    desc2->set_code("LOTR");
    desc2->set_author("Tolkien");
    desc2->set_title("Hobbit");
    desc2->set_publisher("London Pub");
    desc2->set_genre("Fantasy");
    desc2->set_pub_year(1943);

    json p = *desc2;
    std::cout << p << std::endl;

    table->add(desc1->get_code(), desc1);
    table->add(desc2->get_code(), desc2);

    t.set_table(table);

    app->run(t);
    return 0;
}
