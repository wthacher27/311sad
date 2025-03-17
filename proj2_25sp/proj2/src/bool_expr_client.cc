// Copyright Thacher 2025

#include <bool_expr_client.h>


void BoolExprClient::Run() {

}


// Main function
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] <<
        " <socket_path> <truth_values...>" << std::endl;
        return 1;
    }

    // Extract the socket path and the truth values from command-line arguments
    std::string socket_path = argv[1];
    std::vector<std::string> truth_values;
    for (int i = 2; i < argc; ++i) {
        truth_values.push_back(argv[i]);
    }

    // Create and run the client
    BoolExprClient client(socket_path, truth_values);
    client.Run();

    return 0;
}
