#ifndef __ANALYZER_H__
#define __ANALYZER_H__

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

using namespace xercesc_3_1;

#include "Location.h"

#include <vector>
#include <string>

#include "WalkingAverage.h"

class Analyzer {
private:
    static const XMLCh* getValue(const DOMNode* n, std::wstring elementname);
    static double getDoubleValue(const DOMNode* n, std::wstring elementname);
    //static long   getLongValue(const DOMNode* n, std::wstring elementname);
    static long   getTime(const DOMNode* n, std::wstring elementname);

protected:
    XercesDOMParser*        parser;
    ErrorHandler*           errHandler;
    DOMDocument*            doc;
    std::vector<Location>   locations;
    WalkingAverage<double>* avgspeed;
public:
    Analyzer();
    ~Analyzer();

    int loadData(std::string filename);

    void updateSpeeds();
};

#endif // __ANALYZER_H__