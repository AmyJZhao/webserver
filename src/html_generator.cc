#include "html_generator.h"

//This is for debugging purposes
//AGAIN THIS IS FROM A WEB SOURCE
//REFERENCE: http://www.cplusplus.com/forum/general/182654/
namespace html_generator
{
    struct compare {
    	inline bool operator()(const std::string& first,
			const std::string& second) const
	{
	    return first.size() < second.size();
	}
    };

    std::string file_to_string(std::string file_name)
    {
        std::ifstream file(file_name);
        return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>{}};
    }

    //AGAIN THIS IS FROM A WEB SOURCE
    //REFERENCE: http://www.cplusplus.com/forum/general/182654/
    std::set<std::string> extract_hyperlinks(std::string text)
    {
        static const std::regex hl_regex("href=\"(.*?)\"", std::regex_constants::icase);

        return {std::sregex_token_iterator(text.begin(), text.end(), hl_regex, 1),
                std::sregex_token_iterator{}};
    }

    std::set<std::string> extract_src(std::string text)
    {
        static const std::regex hl_regex("src=\"(.*?)\"", std::regex_constants::icase);

        return {std::sregex_token_iterator(text.begin(), text.end(), hl_regex, 1),
                std::sregex_token_iterator{}};
    }


    //AGAIN THIS IS FROM A WEB SOURCE
    //FOR replacing logic, reference: https://stackoverflow.com/questions/9053687/trying-to-replace-words-in-a-string
    //HIGHLY SUGGEST TO REPLACE THIS WITH YOUR OWN REPlACING FUNCTION
    std::string replace_single_href(std::string text, std::string old_href, std::string new_href)
    {
	size_t index = 0;
        while ((index = text.find(old_href, index)) != std::string::npos)
        {
            text.replace(index, old_href.length(), new_href);
            index += new_href.length();
        }
	return text;
    }

    std::string replace_old_href(std::string html_text, std::string prefix)
    {
        std::string new_text = html_text;
	    std::set<std::string> temp = extract_hyperlinks(html_text);
     	compare comp;
	    std::vector<std::string> hyperlinks;
	    for(std::string hlink : temp) 
	    {
		    hyperlinks.push_back(hlink);
	    }
	    std::sort(hyperlinks.begin(), hyperlinks.end(), comp);
	
	    for (std::string hlink : hyperlinks)
        {
            char hlink_array[hlink.size() + 1];
		    strcpy(hlink_array, hlink.c_str());
            if (hlink.find("http://") != std::string::npos || hlink.find("https://") != std::string::npos || hlink_array[0] != '/')
            {
		//skipping all hyperlink with "http://"" or "https://" prefix
                continue;
            }
            std::string new_href = "\"";
            std::string old_href = "\"";
            std::string hyper_link_hdr = "href=\"";
            new_href.insert(0, hlink);
            new_href.insert(0, prefix);
            new_href.insert(0, hyper_link_hdr);
            old_href.insert(0, hlink);
            old_href.insert(0, hyper_link_hdr);
            new_text = replace_single_href(new_text, old_href, new_href);
        }
        return new_text;
    }
    std::string replace_old_src(std::string html_text, std::string prefix)
    {
        std::string new_text = html_text;
	    std::set<std::string> srcs = extract_src(html_text);
	
	    for (std::string src : srcs)
        {
            char src_array[src.size() + 1];
		    strcpy(src_array, src.c_str());
            if (src.find("http://") != std::string::npos || src.find("https://") != std::string::npos || src_array[0] != '/')
            {
		//skipping all hyperlink with "http://"" or "https://" prefix
                continue;
            }
            std::string new_src = "\"";
            std::string old_src = "\"";
            std::string src_link_hdr = "src=\"";
            new_src.insert(0, src);
            new_src.insert(0, prefix);
            new_src.insert(0, src_link_hdr);
            old_src.insert(0, src);
            old_src.insert(0, src_link_hdr);
            new_text = replace_single_href(new_text, old_src, new_src);
        }
        return new_text;
    }

    std::string replace_all(std::string html_text, std::string prefix){
        std::string result = replace_old_src(html_text, prefix);
        std::string result2 = replace_old_href(result, prefix);
        return result2;
    }



} // namespace html_generator
