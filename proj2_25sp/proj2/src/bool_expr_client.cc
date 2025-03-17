// Copyright Thacher 2025

#include <bool_expr_client.h>


void BoolExprClient::Run() {

}


// Main function
int main(int argc, char* argv[]) {
    TestClient dsc(argv[1], argv[2], argv[3][0], argv[4][0]);
    dsc.Run();
    return 0;
}
