#include "extract_labels.h"

#include "log.h"
#include <regex>
#include "picojson.h"
#include "startwith.h"

std::string extract_labels(const std::string& code)
{
    std::string res_string;
    try
    {
        std::regex pieces_regex("_set_goto_label\\(([^\\)]+)");
        std::smatch pieces_match;
        std::string::const_iterator SearchStart( code.cbegin() );

        std::vector<std::string> result_array;
        while(std::regex_search(SearchStart, code.cend(), pieces_match, pieces_regex))
        {
            std::ssub_match sub_match = pieces_match[1];
            std::string piece = sub_match.str();
            picojson::value val;
            picojson::parse(val,piece);
            result_array.push_back(val.get<std::string>());
            SearchStart += pieces_match.position() + pieces_match.length();
        }

        std::sort(result_array.begin(),result_array.end());
        result_array.erase(std::unique(result_array.begin(), result_array.end()), result_array.end());

        picojson::array json_array;
        for(std::string s:result_array)
        {
            if(!starts_with(s,"_internal_"))
            {
                picojson::value::object res;
                res["name"] = picojson::value(s);

                json_array.push_back(picojson::value(res));
            }
        }

        res_string = picojson::value(json_array).serialize();
        WORKER_LOG(std::string("extract_labels<<") + res_string);
    }catch(...)
    {

    }
    return res_string;
}
