#include "..\headers\server.h"

int main() {
    crow::SimpleApp app;
    Server server;

    CROW_ROUTE(app, "/")([] {
        auto page = crow::mustache::load_text("index.html");
        return page;
        });

    CROW_ROUTE(app, "/table")([&server](const crow::request& req) {
        auto page = crow::mustache::load_text("table.html");

        server.database->initialize();

        std::string n_table = req.url_params.get("tableInput"); // This has to match the name of the input that is being sent to get its value correctly.
        server.saveTable((new Table(stoi(n_table))));

        Employee e("adrian", 2, "2023-10-04 15:30:45", "2023-10-04 15:30:45");
        server.saveEmployee(&e);

        Product p;
        p.setName("Carrot pie");
        p.setPrice(12.0);
        server.saveProduct(&p);

        Order o;
        o.setTime("13:04");
        o.setMessage("Hey");
        server.saveOrder(&o);

        Ingredient i;
        i.setName("Carrot");
        server.saveIngredient(&i);

        Allergen a;
        a.setName("Sulfite");
        server.saveAllergen(&a);

        std::vector<Employee> employees = server.getEmployees();
        std::cout << "EMPLOYEES: " << std::endl;
        for (auto e : employees) {
            std::cout << e.getName() << std::endl;
        }
        //std::cout << server.getEmployeeByName("adrian").getName() << std::endl;

        return page;
        });


    CROW_CATCHALL_ROUTE(app)
        ([]() {
        return "Wrong Route";
            });

    // ignore all log
    crow::logger::setLogLevel(crow::LogLevel::Debug);
    //crow::logger::setHandler(std::make_shared<ExampleLogHandler>());

    app.bindaddr("192.168.1.66")
        .port(18080)
        .server_name("CrowCpp")
        .multithreaded()
        .run();
}