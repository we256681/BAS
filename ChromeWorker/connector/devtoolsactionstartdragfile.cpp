#include "devtoolsactionstartdragfile.h"
#include "InputEventsEnumerations.h"
#include "JsonSerializer.h"

void DevToolsActionStartDragFile::Run()
{
    State = Running;

    std::string Filename = Params["filename"].String;
    Params.erase("filename");

    Params["type"] = Variant(std::string("dragEnter"));
    Params["x"] = Variant(GlobalState->CursorX);
    Params["y"] = Variant(GlobalState->CursorY);
    Params["modifiers"] = Variant(KeyboardModifiersNone);

    std::map<std::string, Variant> Data;
    std::vector<Variant> Files;
    Files.push_back(Variant(Filename));


    std::vector<Variant> Items;
    std::map<std::string, Variant> Item;
    Item["mimeType"] = Variant(std::string("image/png"));
    Item["data"] = Variant(std::string("file"));
    Items.push_back(Variant(Item));
    Data["dragOperationsMask"] = Variant(-1);
    Data["files"] = Variant(Files);
    Data["items"] = Variant(Items);

    Params["data"] = Variant(Data);

    JsonSerializer Serializer;
    GlobalState->DragAndDropIsEnabled = true;
    GlobalState->DragAndDropData = Serializer.SerializeObjectToString(Data);


    SendWebSocket("Input.dispatchDragEvent", Params);
}

void DevToolsActionStartDragFile::OnWebSocketMessage(const std::string& Message, const std::string& Error)
{
    Result->Success();
    State = Finished;
}
