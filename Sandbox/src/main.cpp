#include <MatchEngine/MatchEngine.hpp>

int main() {
    MatchEngine::MatchEngine engine;
    MatchEngine::MatchEngine engine2;

    // 引擎初始化入口
    engine.initialize();

    MCH_CLIENT_WARN("Hello World !")
    // 引擎运行入口
    engine.run();

    // 引擎销毁入口
    engine.destroy();

    engine2.initialize();
    engine2.run();
    engine2.destroy();
    return 0;
}
