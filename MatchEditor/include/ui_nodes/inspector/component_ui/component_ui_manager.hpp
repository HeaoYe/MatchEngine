#pragma once

#include <ui_nodes/inspector/component_ui/component_ui.hpp>
#include <MatchEngine/game_framework/component/component.hpp>
#include <MatchEngine/game_framework/game_object.hpp>

namespace MatchEditor {
    // 绘制ComponentUI的管理器, 根据反射信息自动绘制Component的编辑UI
    // 也可以注册某一类Component的UI
    class ComponentUIManager {
    private:
        struct ComponentUIWrapper {
            std::function<void()> delete_callback;
            std::function<void(MatchEngine::Game::Component *)> render_callback;
        };
        struct ComponentViewWrapper {
            void *real_data;
            std::function<void()> delete_callback;
        };
    public:
        ComponentUIManager();
        ~ComponentUIManager();
        void renderComponent(MatchEngine::Game::Component *component);
        void clearCache();
    private:
        void defaultRenderComponent(MatchEngine::Game::Component *component);
        
        template <class ComponentCls>
        void registerComponentUI() {
            auto *ui = new ComponentUI<ComponentCls>();
            ui_wrappers.insert(std::make_pair(
                ComponentCls::GetClassName(),
                ComponentUIWrapper {
                    .delete_callback = [ui]() {
                        delete ui;
                    },
                    .render_callback = [this, ui](MatchEngine::Game::Component *component) {
                        auto *view = getComponentView<ComponentCls>(component);
                        ui->render(*view);
                    }
                }
            ));
        }

        template <class ComponentCls>
        ComponentView<ComponentCls> *getComponentView(MatchEngine::Game::Component *component) {
            auto uuid = component->cast("Component").getValue<MatchEngine::Game::GameObject *>("master")->getUUID();
            auto cache_iter = component_view_cache.find(uuid);
            if (cache_iter == component_view_cache.end()) {
                cache_iter = component_view_cache.insert(std::make_pair(uuid, std::map<std::string, ComponentViewWrapper> {})).first;
            }
            auto component_iter = cache_iter->second.find(component->getClassName());
            if (component_iter == cache_iter->second.end()) {
                auto *view = new ComponentView<ComponentCls>(*static_cast<ComponentCls *>(component));
                component_iter = cache_iter->second.insert(std::make_pair(
                    component->getClassName(),
                    ComponentViewWrapper {
                        .real_data = view,
                        .delete_callback = [view]() {
                            delete view;
                        }
                    }
                )).first;
            }
            return static_cast<ComponentView<ComponentCls> *>(component_iter->second.real_data);
        }
    private:
        std::map<std::string, ComponentUIWrapper> ui_wrappers;
        std::map<MatchEngine::Game::GameObjectUUID, std::map<std::string, ComponentViewWrapper>> component_view_cache;
    };
}
