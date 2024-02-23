#include <Core/Container/Container.hpp>
#include "stdio.h"

int main() {
    MatchEngine::Core::TArray<int> array { 1, 2, 3, 4 };
    array.add(1);
    array.add(2);
    printf("TArray Size: %ld\n", array.size());
    array.remove(1);
    for (auto num : array) {
        printf("TArray: %d\n", num);
    }
    MatchEngine::Core::TArray<int> array2 = array;
    MatchEngine::Core::TArray<int> array3 = std::move(array2);
    array.clear();
    printf("%ld %ld\n", array2.size(), array3.size());
    array3.clear();
    printf("%ld %ld\n", array2.size(), array3.size());

    MatchEngine::Core::TPair<int, float> pair { 1, 0.1f };
    printf("TPair: %d, %f\n", pair.first, pair.second);

    MatchEngine::Core::TSet<double> set;
    set.add(1.0);
    set.add(2.0);
    set.add(1.5);
    set.add(1.5);
    set.add(2.5);
    printf("TSet Size: %ld\n", array.size());
    set.remove(1.0);
    for (auto num : set) {
        printf("TSet: %lf\n", num);
    }
    set.clear();
    return 0;
}
