#include <iostream>

#include "manager.h"

void checkArgs(int argc);

using namespace std;

int main(int argc, const char **args) {
    checkArgs(argc);
    manager client(args[1]);
    client.mainLoop();
    return 0;
}

void checkArgs(int argc) {
    if (argc != 2) {
        cout << "argument error" << endl;
        exit(EXIT_FAILURE);
    }
}
