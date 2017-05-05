#ifndef EASYCC_BASE64_H
#define EASYCC_BASE64_H

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

namespace ecc{
    /**
     * Code reference:
     * http://stackoverflow.com/a/28471421/3435918
     */
    std::string decode64(const std::string &val) {
        using namespace boost::archive::iterators;
        using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
        return boost::algorithm::trim_right_copy_if(std::string(It(std::begin(val)), It(std::end(val))), [](char c) {
            return c == '\0';
        });
    }

    std::string encode64(const std::string &val) {
        using namespace boost::archive::iterators;
        using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
        auto tmp = std::string(It(std::begin(val)), It(std::end(val)));
        return tmp.append((3 - val.size() % 3) % 3, '=');
    }
}

#endif
