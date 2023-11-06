#include "SettingsLargeOptionsPage.hh"

#include "game/system/SaveManager.hh"
#include "game/ui/SettingsPage.hh"
#include "sp/settings/ClientSettings.hh"
#include <Common.hh>
#include <cstdio>

namespace UI {

SettingsLargeOptionsPage::SettingsLargeOptionsPage() = default;

SettingsLargeOptionsPage::~SettingsLargeOptionsPage() = default;

void SettingsLargeOptionsPage::onInit() {
    m_inputManager.init(0x1, false);
    setInputManager(&m_inputManager);
    m_inputManager.setWrappingMode(MultiControlInputManager::WrappingMode::Y);

    initChildren(16);
    for (u8 i = 0; i < 10; i++) {
        insertChild(i, &m_buttons[i], 0);
    }

    insertChild(10, &m_backButton, 0);
    insertChild(11, &m_blackBackControl, 0);
    insertChild(12, &m_arrowLeft, 0);
    insertChild(13, &m_arrowRight, 0);
    insertChild(14, &m_instructionText, 0);
    insertChild(15, &m_settingTitleText, 0);
    u8 buttonId = 0;
    for (u8 i = 0; i < 5; i++) {
        for (u8 j = 0; j < 2; j++) {
            char variant[18];
            snprintf(variant, sizeof(variant), "CategoryButton%hhu_%hhu", i, j);
            m_buttons[buttonId].load("button", "OptionsMenuLarge", variant, 0x1, false, false);
            buttonId++;
        }
    }
    m_backButton.load("button", "Back", "ButtonBackPopup", 0x1, false, true);

    m_blackBackControl.load("control", "RankingBlackBack", "RankingBlackBack");
    m_blackBackControl.m_zIndex = -150.0f;
    auto sectionId = SectionManager::Instance()->currentSection()->id();
    bool isRace = Section::GetSceneId(sectionId) == System::SceneId::Race;
    m_arrowLeft.load("button", "NumberMenuArrowLeft", "ButtonArrowLeft", 0x1, false, true);
    m_arrowRight.load("button", "NumberMenuArrowRight", "ButtonArrowRight", 0x1, false, true);
    if (isRace) {
        // m_instructionText.load("bg", "MenuObiInstructionText", "MenuObiInstructionText",
        // nullptr);
        m_instructionText.load("bg", "RaceObiInstructionText", "RaceObiInstructionText", nullptr);
    } else {
        m_instructionText.load("bg", "MenuObiInstructionText", "MenuObiInstructionText", nullptr);
    }
    m_settingTitleText.load("button", "SubMenuSettingTitle", "SettingTitleTop", nullptr);

    m_inputManager.setHandler(MenuInputManager::InputId::Back, &m_onBack, false, false);

    for (u8 i = 0; i < 10; i++) {
        m_buttons[i].setFrontHandler(&m_onButtonFront, false);
        m_buttons[i].m_index = i;
    }

    m_backButton.setFrontHandler(&m_onBackButtonFront, false);
    m_backButton.m_index = 10;

    m_arrowLeft.setFrontHandler(&m_onButtonFront, false);
    m_arrowRight.setFrontHandler(&m_onButtonFront, false);
    m_arrowLeft.m_index = 11;
    m_arrowRight.m_index = 12;

    m_buttons[0].selectDefault(0);
}

void SettingsLargeOptionsPage::onActivate() {
    auto *settingsPage = SectionManager::Instance()->currentSection()->page<PageId::MenuSettings>();
    auto categoryInfo = settingsPage->getCategoryInfo();
    u32 settingIndexLocal = settingsPage->getSelectedSetting();
    u32 settingIndex = categoryInfo.settingIndex + settingIndexLocal;
    const SP::ClientSettings::Entry &entry = SP::ClientSettings::entries[settingIndex];
    u32 chosen = System::SaveManager::Instance()->getSetting(settingIndex) - entry.valueOffset;
    m_settingTitleText.setMessageAll(entry.messageId);
    m_buttons[chosen].selectDefault(0);
    m_buttons[chosen].setPaneVisible("checkmark", true);

    if (entry.valueCount <= std::size(m_buttons)) {
        m_arrowLeft.setVisible(false);
        m_arrowRight.setVisible(false);
    }

    for (u8 j = 0; j < std::size(m_buttons); j++) {
        if (j >= entry.valueCount) {
            m_buttons[j].setVisible(false);
            continue;
        }
        m_buttons[j].setVisible(true);
        m_buttons[j].setMessageAll(entry.valueMessageIds[j]);
    }
}

void SettingsLargeOptionsPage::onBack(u32 /* localPlayerId */) {
    startReplace(Anim::Prev, 0.0f);
}

void SettingsLargeOptionsPage::onBackButtonFront(PushButton *button, u32 /*localPlayerId*/) {
    f32 delay = button->getDelay();
    startReplace(Anim::Prev, delay);
}

void SettingsLargeOptionsPage::onButtonFront(PushButton *button, u32 /*localPlayerId*/) {
    auto *settingsPage = SectionManager::Instance()->currentSection()->page<PageId::MenuSettings>();
    auto categoryInfo = settingsPage->getCategoryInfo();
    u32 settingIndexLocal = settingsPage->getSelectedSetting();

    auto *saveManager = System::SaveManager::Instance();
    u32 settingIndex = categoryInfo.settingIndex + settingIndexLocal;

    saveManager->setSetting(settingIndex, button->m_index);
    settingsPage->setMiddleButton(settingIndex);
}

} // namespace UI
