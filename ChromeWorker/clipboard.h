#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <string>
#include "base64.h"
#include "converter.h"
#include "startwith.h"

std::string read_clipboard();
void write_clipboard(const std::string data, bool use_prefix = true, bool is_base64 = false);


#endif // CLIPBOARD_H
