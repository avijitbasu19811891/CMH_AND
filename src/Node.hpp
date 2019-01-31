//
//  Node.hpp
//  CMH_AND
//
//  Created by Avijit Basu on 06/11/18.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>
#include <list>
#include "GlobalConfig.hpp"
#include "InputDataSet.hpp"
#include "CMHAndAlgo.hpp"

class SiteInfo {
    int _siteId;
    int _nodeId;
public:
    SiteInfo (void) {
        _siteId = -1;
        _nodeId = -1;
    }
    SiteInfo (int siteId, int nodeId) {
        _siteId = siteId;
        _nodeId = nodeId;
    }
    void setSite (int siteId, int nodeId) {
        if (_siteId < 0) {
            _siteId = siteId;
            _nodeId = nodeId;
        }
    }
    int sendProbetoSite (int destSite);
    
    void display (ostream& os) {
        os << "Site Info->\n";
        os << "\t id : "<< _siteId
        << "\t siteNode : " << _nodeId
        << std::endl;
    }
};
class Node {
    int _nodeId;
    std::shared_ptr<SiteInfo> _site;
    list<int> _waitForList;
public:
    Node (int i,std::shared_ptr<SiteInfo> site, list<int> dependentOnList);
    int addDependentNodes(list<int> dependentOnList);
   
    int startProbe (void);
    
    int recvProbe(ProbeMsg probe);
    void display (ostream& os) {
        os << "Node Info-> \n";
        os << "\t id : " << _nodeId << std::endl;
        if (_site != nullptr) {
            os << "Node part of site->\n";
            _site->display(os);
        }
        if (_waitForList.empty()) {
            os << "\t \t Not waiting for any node" << std::endl;
        } else {
            os << "\t wait list ->\n \t \t";
            for ( auto d : _waitForList) {
                os << d << " |  ";
            }
        }

        os << std::endl;
    }
};
#endif /* Node_hpp */
