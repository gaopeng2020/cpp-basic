#include "MapTest.h"

int main(int argc, char const *argv[]) {
    pairConstructTest();

    constructTest();

    std::map<std::string, uint8_t> _map = addElementsTest();

    iteratorTest(_map);

    removeElementTest(_map);
    return 0;
}
