//
//  InputDataSet.hpp
//  SuzukiKasamiAlgo
//
//  Created by Avijit Basu on 05/11/18.
//  Copyright © 2018 Juniper. All rights reserved.
//

#ifndef InputDataSet_hpp
#define InputDataSet_hpp

#include <stdio.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>
#include <list>
#include "GlobalConfig.hpp"
using namespace std;

class WaitForList {
public:
    string str;
    int    _siteId;
    list<int> _waitForList;
    
    enum Type :int {
        SITE_INFO,
        WAIT_LIST,
        MAX
    };
    WaitForList::Type _type;

    WaitForList (std::string line) {
         _siteId = -1;
        _type = WaitForList::Type::MAX;
        _waitForList.clear();
        stringstream s(line);
        bool parsedCmd = std::false_type::value;
        if (s >> str) {
            parsedCmd = std::true_type::value;
        }
        if (!parsedCmd) {
            return;
        }
        std::cout << "Parsed Cmd:" << str << std::endl;
        if (str == "WAIT_LIST") {
            if (s >> _siteId) {
                std::cout << "Parsed SiteId" << _siteId << std::endl;
                if ((_siteId > GlobalConfig::numNode()) ||
                    (_siteId < 0)) {
                    str = "End";
                } else {
                    int i;
                    while (s >> i) {
                        std::cout << "Dependent Node :" << i << std::endl;
                        _waitForList.push_back(i);
                    }
                }
            }
            _type = WaitForList::Type::WAIT_LIST;
        } else {
            if (str == "SITE_INFO") {
                if (s >> _siteId) {
                    std::cout << "Parsed SiteId" << _siteId << std::endl;
                    if ((_siteId > GlobalConfig::numNode()) ||
                        (_siteId < 0)) {
                        str = "End";
                    } else {
                        int i;
                        while (s >> i) {
                            _waitForList.push_back(i);
                        }
                    }
                }
            }
            _type = WaitForList::Type::SITE_INFO;
        }
        display(std::cout);
    }

    bool isValid (void) {
        return (!str.empty());
    }

    std::string decode (void) {
        if (isValid()) {
            return str;
        }
        return "END";
    }
    int siteId (void) {
        if (isValid()) {
            return _siteId;
        }
        return -1;
    }
    bool isWaitForGraph (void) {
        return (_type == WaitForList::Type::WAIT_LIST);
    }
    
    bool isSiteInfo (void) {
        return (_type == WaitForList::Type::SITE_INFO);
    }
    void display (ostream& os) {
        os << "User Data -> \n \t type : " << _type << std::endl;
        if (_type == WaitForList::Type::WAIT_LIST) {
            os << "NodeId : " << _siteId << std::endl;
            if (_waitForList.empty()) {
                os << "Not waiting for any resource" << std::endl;
            } else {
                os << "wait list ->\n";
                for ( auto d : _waitForList) {
                    os << d << " |  ";
                }
            }
            os << std::endl << std::endl;
        }
    }
};

class DataSet {
    DataSet (void);
    std::list<string> _eventList;
public:
    std::shared_ptr<WaitForList> getNextEvent (void);
    
    static DataSet* instance (void) {
        static DataSet dataSet;
        return &dataSet;
    }
    void readConfigFile( void);
    void description (std::ostream& os);
};
#endif /* InputDataSet_hpp */
