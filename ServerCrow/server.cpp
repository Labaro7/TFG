#include "server.h"
#include "crow_all.h"
#include <sstream>


class ExampleLogHandler : public crow::ILogHandler
{
public:
    void log(std::string /*message*/, crow::LogLevel /*level*/) override
    {
        // cerr << "ExampleLogHandler -> " << message;
    }
};

int main(){
    crow::SimpleApp app;
    Server server;

    CROW_ROUTE(app, "/")([] {
        auto page = crow::mustache::load_text("index.html");
        return page;
        });

    CROW_ROUTE(app, "/table")([&server](const crow::request& req) {
        auto page = crow::mustache::load_text("table.html");
        std::string n_table = req.url_params.get("tableInput"); // This has to match the name of the input that is being sent to get its value correctly.
        //std::cout << n_table << std::endl;
        server.saveTable(stoi(n_table));
        Worker w("adrian", 2, "2023-10-04 15:30:45", "2023-10-04 15:30:45");
        server.saveWorker(&w);
        return page;
        });





    /*CROW_ROUTE(app, "/").methods("POST"_method)([&server](const crow::request& req) {
        const auto& json_data = crow::json::load(req.body);
        if (!json_data)
            return crow::response(400); // Bad request if unable to parse JSON

        std::string result = (std::string)json_data;
        std::cout << "Received: " << result << std::endl;

        std::string redirectScript = R"(
            <script type="text/javascript">
                window.location.href = 'table.html';
            </script>
        )";

        return crow::response(200, redirectScript);
        });

    CROW_ROUTE(app, "/about")
        ([]() {
        return "About Crow example.";
            });

    // simple json response
    CROW_ROUTE(app, "/json")
        ([] {
        crow::json::wvalue x({ {"message", "Hello, World!"} });
        x["message2"] = "Hello, World.. Again!";
        return x;
            });

    CROW_ROUTE(app, "/json-initializer-list-constructor")
        ([] {
        crow::json::wvalue x({
          {"first", "Hello world!"},                     // stores a char const* hence a json::type::String
          {"second", std::string("How are you today?")}, // stores a std::string hence a json::type::String.
          {"third", 54},                                 // stores an int (as 54 is an int literal) hence a std::int64_t.
          {"fourth", 54},                               //  stores a long (as 54l is a long literal) hence a std::int64_t.
          {"fifth", 54u},                                // stores an unsigned int (as 54u is a unsigned int literal) hence a std::uint64_t.
          {"sixth", 54u},                               //  stores an unsigned long (as 54ul is an unsigned long literal) hence a std::uint64_t.
          {"seventh", 2.f},                              // stores a float (as 2.f is a float literal) hence a double.
          {"eighth", 2.},                                // stores a double (as 2. is a double literal) hence a double.
          {"ninth", nullptr},                            // stores a std::nullptr hence json::type::Null .
          {"tenth", true}                                // stores a bool hence json::type::True .
            });
        return x;
            });

    // json list response
    CROW_ROUTE(app, "/json_list")
        ([] {
        crow::json::wvalue x(crow::json::wvalue::list({ 1, 2, 3 }));
        return x;
            });

    CROW_ROUTE(app, "/hello/<int>")
        ([](int count) {
        if (count > 100)
            return crow::response(400);
        std::ostringstream os;
        os << count << " bottles of beer!";
        return crow::response(os.str());
            });

    // example which uses only response as a paramter without
    // request being a parameter.
    CROW_ROUTE(app, "/add/<int>/<int>")
        ([](crow::response& res, int a, int b) {
        std::ostringstream os;
        os << a + b;
        res.write(os.str());
        res.end();
            });

    // more json example
    CROW_ROUTE(app, "/add_json")
        ([](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x)
            return crow::response(400);
        int sum = x["a"].i() + x["b"].i();
        std::ostringstream os;
        os << sum;
        return crow::response{ os.str() };
            });

    CROW_ROUTE(app, "/params")
        ([](const crow::request& req) {
        std::ostringstream os;
        os << "Params: " << req.url_params << "\n\n";
        os << "The key 'foo' was " << (req.url_params.get("foo") == nullptr ? "not " : "") << "found.\n";
        if (req.url_params.get("pew") != nullptr)
        {
            double countD = boost::lexical_cast<double>(req.url_params.get("pew"));
            os << "The value of 'pew' is " << countD << '\n';
        }
        auto count = req.url_params.get_list("count");
        os << "The key 'count' contains " << count.size() << " value(s).\n";
        for (const auto& countVal : count)
        {
            os << " - " << countVal << '\n';
        }
        return crow::response{ os.str() };
            });*/

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


void Server::saveTable(int n_table) {
    dataBase->saveTable(n_table);
}

void Server::saveWorker(Worker* worker) {
    dataBase->saveWorker(worker);
}
