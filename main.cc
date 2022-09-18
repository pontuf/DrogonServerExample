#include <cstdlib>
#include <drogon/drogon.h>

int main() {
    std::cout<< "Server start..." << std::endl;
    drogon::app()
        .loadConfigFile("./config.json")
        .run();
}
