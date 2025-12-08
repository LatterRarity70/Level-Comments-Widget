#define GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE
#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

#include "Geode/modify/PauseLayer.hpp"//left-button-menu
class $modify(PauseLayerExt, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        if (auto menu = querySelector("left-button-menu")) {
            if (auto item = CCMenuItemExt::createSpriteExtraWithFrameName(
                "GJ_chatBtn_001.png", 0.6f, [](void*) {
                    if (auto a = CCScene::get()->getChildByType<InfoLayer>(0))
                        a->onComment(a);
                }
            )) {
                menu->addChild(item);
            }
            if (auto item = CCMenuItemExt::createSpriteExtraWithFrameName(
                "GJ_chatBtn_001.png", 0.6f, [](void*) {
                    openInfoPopup(getMod());
                }
            )) {
                auto sprite = item->getNormalImage();
                if (auto sub = CCSprite::createWithSpriteFrameName(
                    "geode.loader/settings.png"
                )) {
                    sub->setPosition({ 34.000f, 14.000f });
					sub->setScale(0.775f);
                    sprite->addChild(sub);
                }
                menu->addChild(item);
            }
            menu->updateLayout();
        }
    }
};

#include "Geode/modify/PlayLayer.hpp"
class $modify(PlayLayerExt, PlayLayer) {
    class InfoLayerUpdater : public InfoLayer {
	public:
        void reload(float) {
			if (!this or !typeinfo_cast<InfoLayer*>(this)) return;
            //dont reset when dragging
            if (auto a = m_list) if (auto b = a->m_list)
                if (auto c = b->m_tableView) if (c->m_touchDown) return;
            //reset
            this->onRefreshComments(0);
        }
        void setup(float) {
			if (!this or !typeinfo_cast<InfoLayer*>(this)) return;
            this->setPosition(
                getMod()->getSavedValue("window.pos.x", -202.000),
                getMod()->getSavedValue("window.pos.y", -49.000f)
            );
            this->setScale(getMod()->getSettingValue<float>("scale"));
            this->setOpacity(0);
            this->setZOrder(999);
            for (auto a : m_mainLayer->getChildrenExt()) a->setVisible(
                typeinfo_cast<GJCommentListLayer*>(a)
            );
            m_reportBtn->setVisible(0);
            if (auto a = m_timeBtn) if (auto p = a->getParent()) p->setVisible(
                getMod()->getSettingValue<bool>("show-left-side-menu")
            );
            if (auto a = m_buttonMenu) a->setVisible(
                getMod()->getSettingValue<bool>("show-menu")
            );
            if (m_list) {
                for (auto a : m_list->getChildrenExt()) a->setVisible(
                    typeinfo_cast<CustomListView*>(a)
                );
                findFirstChildRecursive<CCLayerColor>(
                    m_list, [](CCLayerColor* node) {
                        node->setColor(getMod()->getSettingValue<ccColor3B>("color"));
                        node->setOpacity(getMod()->getSettingValue<int>("opacity"));
                        return false;
                    }
                );
                if (auto a = m_list->m_list) if (auto scroll = a->m_tableView) {
                    scroll->setMouseEnabled(false);
                    scroll->m_disableVertical = false;
                    scroll->m_disableHorizontal = false;
                    auto content = scroll->m_contentLayer;
                    if (m_reportBtn->getPositionX() != 0) {
                        m_reportBtn->setPosition(content->getPosition());
                    }
                    if (scroll->m_touchDown) {
                        auto offset = this->getPosition() - ccpSub(
                            m_reportBtn->getPosition(),
                            content->getPosition()
                        );
                        getMod()->setSavedValue("window.pos.x", offset.x);
                        getMod()->setSavedValue("window.pos.y", offset.y);
                        content->setPosition(m_reportBtn->getPosition());
                    }
                }
            }
        }
    };
    void updateInfoLayer(float) { //meant to be called on scene
        if (auto info = getChildByType<InfoLayer>(0)) {
            info->scheduleOnce(schedule_selector(InfoLayerUpdater::setup), 0.f);
		}
	}
    void updateInfoLayerLive(float) { //meant to be called on scene
		if (auto info = getChildByType<InfoLayer>(0)) {
            info->scheduleOnce(schedule_selector(InfoLayerUpdater::reload), 0.f);
		}
	}
    virtual void onEnterTransitionDidFinish() {
		PlayLayer::onEnterTransitionDidFinish();
        if (!m_level) return;
        if (!getParent()) return;
        auto sc = getParent();
        auto info = InfoLayer::create(m_level, 0, 0);
        info->setMouseEnabled(false);
        info->setTouchEnabled(false);
        info->setKeyboardEnabled(false);
        handleTouchPriorityWith(info, 0, true);
        sc->addChild(info);
        sc->schedule(schedule_selector(PlayLayerExt::updateInfoLayer));
        sc->schedule(schedule_selector(PlayLayerExt::updateInfoLayerLive), 1.f);
    };
};