function _get_clipboard(is_base64, mime)
{
    return ScriptWorker.GetClipboard(is_base64, mime)
}

function _set_clipboard(is_base64, mime, data)
{
    ScriptWorker.SetClipboard(is_base64, mime, data)
}