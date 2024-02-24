#include <Core/Container/Container.hpp>
#include <Core/Delegate/Delegate.hpp>
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
    MatchEngine::Core::TArray<int> array3;
    array3 = MatchEngine::Core::Move(array2);
    array.clear();
    printf("%ld %ld\n", array2.size(), array3.size());
    array3.clear();
    printf("%ld %ld\n", array2.size(), array3.size());

    MatchEngine::Core::TPair<int, float> pair { 1, 0.1f }, pair2, pair3;
    pair2 = pair;
    pair3 = MatchEngine::Core::Move(pair2);
    printf("TPair: %d, %f\n", pair.first, pair.second);

    MatchEngine::Core::TSet<double> set, set1, set2;
    set.add(1.0);
    set.add(2.0);
    set.add(1.5);
    set.add(1.5);
    set.add(2.5);
    set1 = set;
    set2 = MatchEngine::Core::Move(set1);
    printf("TSet Size: %ld\n", array.size());
    set.remove(1.0);
    for (auto num : set) {
        printf("TSet: %lf\n", num);
    }
    set.clear();

    MatchEngine::Core::TMap<float, char> map {
        { 0.1f, 'a' },
        { 0.2f, 'a' },
        { 0.3f, 'a' }
    };

    printf("%c\n", map.at(0.2f));
    map.remove(0.2f);
    for (auto &[key, value] : map) {
        printf("%f: %c\n", key, value);
    }

    MatchEngine::Core::TMap<float, char> map1, map2;
    map1 = MatchEngine::Core::Copy(map);
    map2 = MatchEngine::Core::Move(map1);
    auto result_pair = map2.find(0.1f);
    printf("%f, %c\n", result_pair.first, result_pair.second);

    MatchEngine::Core::IDelegate<void, int, int, int> *delegate;
    delegate = new MatchEngine::Core::SingleDelegate<void(int, int, int)>();
    delegate->broadcast(1, 2, 3);
    delegate->bind([](int a, int b, int c) {
        printf("Delegate1: %d, %d, %d\n", a, b, c);
    });
    delegate->broadcast(1, 2, 2);
    delegate->bind([](int a, int b, int c) {
        printf("Delegate2: %d, %d, %d\n", a, b, c);
    });
    delegate->broadcast(1, 4, 2);

    return 0;
}
