#include <print>
#include <string>
#include <logging.hpp>

int main() {
    using namespace core::logging;

    info("Service initialization started");
    debug("Allocated internal cache buffers");
    warn("High memory consumption detected");
    error("Failed to bind socket to network interface");

    const int user_id = 4289;
    const std::string username = "alex_doe";
    const double latency_ms = 4.82;
    const int retry_count = 3;
    const bool ssl_enabled = true;
    const std::string host = "192.168.1.100";
    const int port = 8080;

    info("Client connection accepted", VAR(host), VAR(port), VAR(ssl_enabled));

    debug("Database query completed", VAR(user_id), VAR(latency_ms));

    warn("Upstream service slow response", VAR(retry_count), VAR(latency_ms));

    error("Authentication token rejected", VAR(user_id), VAR(username));

    std::println("Normal print");

    return 0;
}
