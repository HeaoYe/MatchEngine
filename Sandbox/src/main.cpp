#include <Core/Container/Container.hpp>
#include "stdio.h"

int main() {
    MatchEngine::Core::TArray<int> array { 1, 2, 3, 4 };
    for (auto num : array) {
        printf("TArray: %d\n", num);
    }

    MatchEngine::Core::TPair<int, float> pair { 1, 0.1f };
    printf("TPair: %d, %f\n", pair.first, pair.second);
    return 0;
}
