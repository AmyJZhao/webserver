#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <set>
#include <regex>
 

//REFERENCE: http://www.cplusplus.com/forum/general/182654/


namespace html_generator
{
    //USAGE: USE replace_all TO REPLACE ALL HYPERLINK AND SRC in a html text by appending prefix to the old_href
    
    std::string replace_all(std::string html_text, std::string prefix);

    std::string replace_old_src(std::string html_text, std::string prefix);

    std::string replace_old_href(std::string html_text, std::string prefix);

    //These functions are all helper functions
    std::string replace_single_href(std::string text, std::string old_href, std::string new_href);

    std::string file_to_string( std::string file_name );

    std::set<std::string> extract_hyperlinks( std::string text );


} // namespace html_generator