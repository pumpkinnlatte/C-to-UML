//
// Created by jitomate on 19/08/24.
//

#ifndef UML_STRINGF_H
#define UML_STRINGF_H

class stringF {
    public:
        static void ltrim(std::string &s){
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
        };
        static void rtrim(std::string &s){
            s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), s.end());
        };
        static void trim(std::string &s){
            ltrim(s);
            rtrim(s);
        };
};

#endif //UML_STRINGF_H
