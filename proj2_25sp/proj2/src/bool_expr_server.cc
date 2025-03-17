// Copyright Thacher 2025

#include <bool_expr_server.h>

//obj
BoolExprServer::BoolExprServer(const std::string& socket_path,
    const std::string& expr_file, const std::string& us, const std::string& eot)
        : socket_path_(socket_path),
        expr_file_(expr_file),us_(us), eot_(eot) {}

// Converts expressions from text file into string
std::string BoolExprServer::ReadExpressionFromFile() {
    std::ifstream file(expr_file_);
    if (!file.is_open()) {
        std::cerr << "Failed to open expression file." << std::endl;
        exit(4);
    }
    return std::string((std::istreambuf_iterator<char>(file)),
    std::istreambuf_iterator<char>());
}

// Splits string of expressions by each new line
std::vector<std::string> BoolExprServer::SplitExpressionByLines
(const std::string& expression) {
    std::vector<std::string> result;
    std::string current_line;
    for (size_t i = 0; i < expression.size(); ++i) {
        if (expression[i] == '\n') {
            if (!current_line.empty()) {
                result.push_back(current_line);
                current_line.clear();
            }
        } else {
            current_line += expression[i];
        }
    }
    if (!current_line.empty()) result.push_back(current_line);
    return result;
}

// ripped from bool_xpr_parser
std::unordered_map<char, bool> BoolExprServer::BuildTruthMap
(const std::string& truth_values) {
    std::unordered_map<char, bool> values;
    std::size_t varIndex = 0;
    for (char b_val : truth_values) {
      char var = 'a' + varIndex; // Variables are a, b, c, ...
      values[var] = (b_val == 'T');
      varIndex++;
    }
    return values;
}

// Removes blank spaces from string
std::string BoolExprServer::RemoveSpaces(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (c != ' ') result += c;
    }
    return result;
}

void BoolExprServer::Run() {
    if (!Init() || !Bind() || !Listen()) {
        exit(1);
    }

    std::cout <<
    "Server is running and waiting for client connections..."
    << std::endl;

    while (true) {
        //accept connection
        int client_fd = accept(server_fd_, nullptr, nullptr);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        std::cout << "Client connected" << std::endl;
        ssize_t bytes_received = 0;
        ssize_t bytes_sent = 0;
        char buffer[BUFFER_SIZE];

        ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            bytes_received += bytes_read;
            buffer[bytes_read] = '\0';

            std::string values(buffer);
            values = RemoveSpaces(values);
            std::unordered_map<char, bool> truth_map = BuildTruthMap(values);

            int true_count = 0, false_count = 0, error_count = 0;
            std::vector<std::string> expressions =
            SplitExpressionByLines(ReadExpressionFromFile());

            for (const auto& expr : expressions) {
                std::string str_no_space = RemoveSpaces(expr);
                std::set<char> unique_vars;
                for (char c : str_no_space) {
                    if (std::isalpha(c)) unique_vars.insert(c);
                }

                bool missing_value = false;
                for (char var : unique_vars) {
                    if (truth_map.count(var) == 0) {
                        missing_value = true;
                        break;
                    }
                }

                if (missing_value) {
                    error_count++;
                    continue;
                }

                BooleanExpressionParser parser(str_no_space, truth_map);
                if (!parser.HasError()) {
                    if (parser.Parse()) {
                        true_count++;
                    } else {
                        false_count++;
                    }
                } else {
                    error_count++;
                }
            }

            std::string result = std::to_string(true_count) + us_ +
                                 std::to_string(false_count) + us_ +
                                 std::to_string(error_count) + eot_;

            // Retruns string to client
            ssize_t sent_bytes = send(client_fd,
                result.c_str(), result.size(), 0);
            if (sent_bytes != -1) {
                bytes_sent += sent_bytes;
            } else {
                perror("Send failed");
            }
        }

        std::cout << bytes_sent << "B sent, " <<
        bytes_received << "B received" << std::endl;

        std::cout << "Client disconnected" << std::endl;
        close(client_fd);  // Close client connection
    }

    close(server_fd_);
}



// Main function
int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] <<
        " <expression_file> <socket_path> US EOT" << std::endl;
        return 1;
    }

    BoolExprServer server(argv[2], argv[1], argv[3], argv[4]);
    server.Run();
    return 0;
}
