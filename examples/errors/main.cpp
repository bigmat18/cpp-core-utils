#include <errors.hpp>
#include <print>
#include <string>

using namespace core::errors;

Result<int> parse_port(int raw_port) {
    REQUIRE(raw_port > 0 && raw_port <= 65535, 
            "Invalid port number", 1001, VAR(raw_port));
    return raw_port;
}

Result<std::string> setup_endpoint(const std::string& host, int port) {
    int valid_port = TRY(parse_port(port));
    return std::format("{}:{}", host, valid_port);
}

Result<std::string> initialize_service(const std::string& host, int port) {
    std::string endpoint = TRY(setup_endpoint(host, port));
    return std::format("Service online at -> {}", endpoint);
}

int main() {
    std::string primary_service = UNWRAP(initialize_service("127.0.0.1", 8080));
    std::println("{}", primary_service);

    std::string recovered_service = initialize_service("10.0.0.1", 70000)
        .or_else([](const Error& err) -> Result<std::string> {
            return initialize_service("127.0.0.1", 8080);
        })
        .value_or("Service offline (Emergency Fallback)");
    std::println("{}", recovered_service);

    auto fatal_service = UNWRAP(initialize_service("192.168.1.1", 99999));
    std::println("{}", fatal_service);
    return 0;
}
