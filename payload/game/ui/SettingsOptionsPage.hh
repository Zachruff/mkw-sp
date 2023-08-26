#pragma once

#include "game/ui/BlackBackControl.hh"
#include "game/ui/Page.hh"
#include "game/ui/ctrl/CtrlMenuBackButton.hh"
#include "game/ui/ctrl/CtrlMenuInstructionText.hh"
#include "game/ui/ctrl/CtrlMenuPageTitleText.hh"

namespace UI {

class SettingsOptionsPage : public Page {
public:
    SettingsOptionsPage();
    ~SettingsOptionsPage() override;

    void onInit() override;
    void onActivate() override;

private:
    void onBack(u32 localPlayerId);
    void onBackButtonFront(PushButton *button, u32 localPlayerId);

    MultiControlInputManager m_inputManager;

    BlackBackControl m_blackBackControl;

    // UI Elements
    PushButton m_optionButton;

    CtrlMenuBackButton m_backButton;

    template <typename T>
    using H = typename T::template Handler<SettingsOptionsPage>;
    H<MultiControlInputManager> m_onBack{this, &SettingsOptionsPage::onBack};
    H<PushButton> m_onBackButtonFront{this, &SettingsOptionsPage::onBackButtonFront};
};

} // namespace UI
