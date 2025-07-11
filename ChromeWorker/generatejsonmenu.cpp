#include "generatejsonmenu.h"
#include "picojson.h"
#include "base64.h"

picojson::value GenerateItem(const std::string& Name, const std::string& Id, const std::string& Icon)
{
    picojson::value::object MenuItem;
    MenuItem["i"] = picojson::value(Id);
    MenuItem["n"] = picojson::value(Name);
    MenuItem["c"] = picojson::value(Icon);
    return picojson::value(MenuItem);
}

picojson::value GenerateItem(const std::string& Name, const std::string& Icon, const picojson::array& Children)
{
    picojson::value::object MenuItem;
    MenuItem["i"] = picojson::value(std::string());
    MenuItem["n"] = picojson::value(Name);
    MenuItem["c"] = picojson::value(Icon);
    MenuItem["l"] = picojson::value(Children);
    return picojson::value(MenuItem);
}

std::string GenerateJsonMenu(bool IsImageSelect, int MousePositionX, int MousePositionY, const std::vector<std::string>& Tabs, const ModulesDataList& Modules, const ModulesDataList& UnusedModules)
{
    std::string Position = std::string("(") + std::to_string(MousePositionX) + std::string(",") + std::to_string(MousePositionY) + std::string(")");

    picojson::array AllItems;

    AllItems.push_back(GenerateItem("Control browser directly", "IDManualControl", "wifi"));

    AllItems.push_back(GenerateItem("Move and click on element", "IDMoveAndClickElement", "mouse-pointer"));
    AllItems.push_back(GenerateItem("Click on element", "IDClickElement", "mouse-pointer"));
    AllItems.push_back(GenerateItem("Move on element", "IDMoveElement", "mouse-pointer"));

    if(!IsImageSelect)
    {
        AllItems.push_back(GenerateItem("tr(Move and click on coordinates)" + std::string(" ") + Position, "IDMoveAndClick", "mouse-pointer"));
        AllItems.push_back(GenerateItem("tr(Click on coordinates)" + std::string(" ") + Position, "IDClick", "mouse-pointer"));
        AllItems.push_back(GenerateItem("tr(Move on coordinates)" + std::string(" ") + Position, "IDMove", "mouse-pointer"));
    }

    AllItems.push_back(GenerateItem("Type text", "IDType", "keyboard-o"));
    AllItems.push_back(GenerateItem("Clear edit", "IDClear", "keyboard-o"));
    if(!IsImageSelect)
        AllItems.push_back(GenerateItem("Inspect element in devtools", "IDInspect", "search"));

    int Iterator = 0;
    picojson::array TabsSubmenu;
    TabsSubmenu.push_back(GenerateItem("Add new tab", std::string("IDTab") + std::to_string(Iterator++), "window-restore"));
    TabsSubmenu.push_back(GenerateItem("Get information about tabs", std::string("IDInfoTab"), "info-circle"));
    bool first = true;
    for(const std::string& Tab: Tabs)
    {
        TabsSubmenu.push_back(GenerateItem(std::string("tr(Select tab) (") + Tab + std::string(")"), std::string("IDTab") + std::to_string(Iterator++), "window-restore"));
        if(!first)
        {
            TabsSubmenu.push_back(GenerateItem(std::string("tr(Remove tab) (") + Tab + std::string(")"), std::string("IDTab") + std::to_string(Iterator++), "window-restore"));
        }
        first = false;
    }

    if(!IsImageSelect)
    {
        AllItems.push_back(GenerateItem("Start loop", "IDLoop", "bars"));
    }

    AllItems.push_back(GenerateItem("Tabs", "window-restore", TabsSubmenu));

    AllItems.push_back(GenerateItem("Is element exists", "IDExists", "question-circle-o"));

    if(!IsImageSelect)
    {
        AllItems.push_back(GenerateItem("Get element content(html)", "IDXml", "code"));
        AllItems.push_back(GenerateItem("Get element text", "IDText", "code"));
        AllItems.push_back(GenerateItem("Execute javascript on element", "IDScript", "code"));
        AllItems.push_back(GenerateItem("Screenshot", "IDScreenshot", "picture-o"));
        AllItems.push_back(GenerateItem("Solve captcha", "IDCaptcha", "picture-o"));
    }

    if(!IsImageSelect)
    {
        Iterator = 0;
        for(const ModulesData& Module:Modules)
        {
            for(const ActionData& Action:Module->Actions)
            {
                if(Action->IsElement)
                {
                    AllItems.push_back(GenerateItem(Action->DescriptionNoTranslation, std::string("IDModules") + std::to_string(Iterator++), "cube"));
                }
            }
        }
        for(const ModulesData& Module:UnusedModules)
        {
            for(const ActionData& Action:Module->Actions)
            {
                if(Action->IsElement)
                {
                    AllItems.push_back(GenerateItem(Action->DescriptionNoTranslation, std::string("IDModules") + std::to_string(Iterator++), "ban"));
                }
            }
        }
    }

    AllItems.push_back(GenerateItem("Wait while element exists", "IDWaitElement", "clock-o"));
    if(!IsImageSelect)
    {
        AllItems.push_back(GenerateItem("Scroll to element", "IDFocus", "bullseye"));
    }
    AllItems.push_back(GenerateItem("Get element coordinates", "IDCoordinates", "map-marker"));
    if(!IsImageSelect)
    {
        AllItems.push_back(GenerateItem("Get elements count", "IDLength", "list-ol"));
        AllItems.push_back(GenerateItem("Get element style", "IDStyle", "paint-brush"));
    }


    AllItems.push_back(GenerateItem("Drag from element", "IDDragElement", "arrows"));
    AllItems.push_back(GenerateItem("Drop on element", "IDDropElement", "arrows"));

    if(!IsImageSelect)
    {
        AllItems.push_back(GenerateItem("tr(Drag from coordinates)" + std::string(" ") + Position, "IDDrag", "arrows"));
        AllItems.push_back(GenerateItem("tr(Drop on coordinates)" + std::string(" ") + Position, "IDDrop", "arrows"));
    }

    if(!IsImageSelect)
    {
        AllItems.push_back(GenerateItem("Get link URL", "IDGetLinkURL", "link"));
    }

    if(!IsImageSelect)
    {
        AllItems.push_back(GenerateItem("Get element attribute", "IDGetAttr", "list-ul"));
        AllItems.push_back(GenerateItem("Set element attribute", "IDSetAttr", "list-ul"));


        AllItems.push_back(GenerateItem("Set combobox by value", "IDSet", "caret-square-o-down"));
        AllItems.push_back(GenerateItem("Set combobox by index", "IDSetInteger", "caret-square-o-down"));
        AllItems.push_back(GenerateItem("Set combobox to random", "IDSetRandom", "caret-square-o-down"));
    }



    /*if(!IsImageSelect)
    {
        picojson::array ForeachSubmenu;

        ForeachSubmenu.push_back(GenerateItem("Start loop", "IDLoop", "bars"));

        ForeachSubmenu.push_back(GenerateItem("Move and click on element", "IDMoveAndClickElementLoop", "mouse-pointer"));
        ForeachSubmenu.push_back(GenerateItem("Click on element", "IDClickElementLoop", "mouse-pointer"));
        ForeachSubmenu.push_back(GenerateItem("Move on element", "IDMoveElementLoop", "mouse-pointer"));
        ForeachSubmenu.push_back(GenerateItem("Type text", "IDTypeLoop", "keyboard-o"));
        ForeachSubmenu.push_back(GenerateItem("Clear edit", "IDClearLoop", "keyboard-o"));

        ForeachSubmenu.push_back(GenerateItem("Is element exists", "IDExistsLoop", "question-circle-o"));
        ForeachSubmenu.push_back(GenerateItem("Get element content(html)", "IDXmlLoop", "code"));
        ForeachSubmenu.push_back(GenerateItem("Get element text", "IDTextLoop", "code"));
        ForeachSubmenu.push_back(GenerateItem("Execute javascript on element", "IDScriptLoop", "code"));
        ForeachSubmenu.push_back(GenerateItem("Screenshot", "IDScreenshotLoop", "picture-o"));
        ForeachSubmenu.push_back(GenerateItem("Solve captcha", "IDCaptchaLoop", "picture-o"));

        Iterator = 0;
        for(const ModulesData& Module:Modules)
        {
            for(const ActionData& Action:Module->Actions)
            {
                if(Action->IsElement)
                {
                    ForeachSubmenu.push_back(GenerateItem(Action->DescriptionNoTranslation, std::string("IDModulesLoop") + std::to_string(Iterator++), "cube"));
                }
            }
        }

        ForeachSubmenu.push_back(GenerateItem("Scroll to element", "IDFocusLoop", "bullseye"));
        ForeachSubmenu.push_back(GenerateItem("Get element coordinates", "IDCoordinatesLoop", "map-marker"));
        ForeachSubmenu.push_back(GenerateItem("Get element style", "IDStyleLoop", "paint-brush"));


        ForeachSubmenu.push_back(GenerateItem("Get element attribute", "IDGetAttrLoop", "list-ul"));
        ForeachSubmenu.push_back(GenerateItem("Set element attribute", "IDSetAttrLoop", "list-ul"));

        ForeachSubmenu.push_back(GenerateItem("Set combobox by value", "IDSetLoop", "caret-square-o-down"));
        ForeachSubmenu.push_back(GenerateItem("Set combobox by index", "IDSetIntegerLoop", "caret-square-o-down"));
        ForeachSubmenu.push_back(GenerateItem("Set combobox to random", "IDSetRandomLoop", "caret-square-o-down"));

        AllItems.push_back(GenerateItem("For each element", "bars", ForeachSubmenu));
    }*/


    std::string ResRaw = picojson::value(AllItems).serialize();
    return base64_encode((unsigned char const *)ResRaw.data(),ResRaw.length());
}

