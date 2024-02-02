#include <MatchEngine/MatchEngine.hpp>
#include <Match/Match.hpp>

int main() {
    MatchEngine::Initialize();

    MCH_WARN("Hello World !")

    while (Match::window->is_alive()) {
        Match::window->poll_events();
    }

    MatchEngine::Destroy();
    return 0;
}
