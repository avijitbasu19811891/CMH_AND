//
//  GlobalConfig.hpp
//  SuzukiKasamiAlgo
//
//  Created by Avijit Basu on 04/11/18.
//

#ifndef GlobalConfig_hpp
#define GlobalConfig_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>

class Error {
public:
    enum ErrorId {
      _EOK,
        _EFAIL,
        _EINVAL,
        _ETEMRINATE
    };
};

///
/// @class responsible for gloabl config params.
///
class GlobalConfig {
public:
    int _NumNode;
    int _NumSites;
    int _Initiator;
    std::string _fileName;
private:
    GlobalConfig (void) {
        _NumNode = 6;
        _NumSites = 2;
        _Initiator = 0;
        _fileName = "WFG.txt";
    }
public:
    static GlobalConfig* instance (void) {
        static GlobalConfig globalConfig;
        return &globalConfig;
    }

    ///
    /// @brief configure the topology.
    ///
    void config (int numNode, int numSites,  int initiator) {
        if (numNode > numSites) {
            _NumNode = numNode;
            _NumSites = numSites;
            _Initiator = initiator;
        }
    }
    void setFileName (std::string name) {
        _fileName = name;
    }
    static std::string fileName (void) {
        GlobalConfig* config = GlobalConfig::instance();
        if (config != nullptr) {
            return config->_fileName;
        }
        return "temp.txt";
    }
    
    static int numNode (void) {
        GlobalConfig* config = GlobalConfig::instance();
        if (config != nullptr) {
            return config->_NumNode;
        }
        return 0;
    }
    static int initiator (void) {
        GlobalConfig* config = GlobalConfig::instance();
        if (config != nullptr) {
            return config->_Initiator;
        }
        return 0;
    }
    static int numSite (void) {
        GlobalConfig* config = GlobalConfig::instance();
        if (config != nullptr) {
            return config->_NumSites;
        }
        return 1;
    }
};

#endif /* GlobalConfig_hpp */
