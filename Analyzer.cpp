#include "Analyzer.h"

#include <fstream>

Analyzer::Analyzer(){
    avgspeed = new WalkingAverage<double>(3);

    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        char *message = XMLString::transcode(toCatch.getMessage());
        std::cout << "Error during initialization! :\n"
                << message << "\n";
        XMLString::release(&message);
        return;
    }

    parser = new XercesDOMParser();
    parser->setValidationScheme(XercesDOMParser::Val_Always);
    parser->setDoNamespaces(true);    // optional

    errHandler = (ErrorHandler*) new HandlerBase();
    parser->setErrorHandler(errHandler);
}

Analyzer::~Analyzer() {
    delete avgspeed;
    if (parser) {
        delete parser;
        parser = nullptr;
    }
    if (errHandler) {
        delete errHandler;
        errHandler = nullptr;
    }
}

int Analyzer::loadData(std::string filename) {
    try {
        parser->parse(filename.c_str());
    }
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        std::cout << "Exception message is: \n"
                << message << "\n";
        XMLString::release(&message);
        return -1;
    }
    catch (const DOMException& toCatch) {
        char* message = XMLString::transcode(toCatch.msg);
        std::cout << "Exception message is: \n"
                << message << "\n";
        XMLString::release(&message);
        return -1;
    }
    catch (...) {
        std::cout << "Unexpected Exception \n" ;
        return -1;
    }

    doc = parser->getDocument();
    double lat, lon, ele;
    long t;

    DOMNodeList* nodeList = doc->getElementsByTagName(L"trkpt");
    if (nodeList) {
        if (nodeList->getLength()>0) {
            int l = nodeList->getLength();
            for (int i = 0; i<l; i++) {
                DOMNode* trkpt = nodeList->item((XMLSize_t) i);

                DOMNamedNodeMap* nnm = trkpt->getAttributes();
                DOMNode* latNode = nnm->getNamedItem(L"lat");
                DOMNode* lonNode = nnm->getNamedItem(L"lon");
                lat = wcstod(latNode->getNodeValue(),NULL);
                lon = wcstod(lonNode->getNodeValue(),NULL);

                ele = getDoubleValue(trkpt, L"ele");
                t = getTime(trkpt,L"time");
                Location loc(lat,lon,ele,t);
                locations.insert(locations.end(), loc);
            }
        }
    }
    return 0;
}



const XMLCh* Analyzer::getValue(const DOMNode* n, std::wstring elementname) {
    DOMNodeList* nodeList = n->getChildNodes();
    for (int i = 0; i<nodeList->getLength(); i++) {
        DOMNode* nn = nodeList->item((XMLSize_t)i);
        if (XMLString::compareString(nn->getNodeName(),elementname.c_str())==0) {
            DOMNode* first = nn->getFirstChild();
            return first->getNodeValue();
        }
    }
    return nullptr;
}

double Analyzer::getDoubleValue(const DOMNode* n, std::wstring elementname) {
    const XMLCh* value = getValue(n,elementname);
    if (value) {
        return wcstod(value,NULL);
    } else {
        return -1;
    }
}

/*
long   Analyzer::getLongValue(const DOMNode* n, std::wstring elementname){
    const XMLCh* value = getValue(n,elementname);
    if (value) {
        return wcstol(value,NULL,10);
    } else {
        return -1;
    }
}*/

long Analyzer::getTime(const DOMNode* n, std::wstring elementname) {
    const XMLCh* timestring = getValue(n,elementname);

    if (timestring) {
        int year, month, day, hour, min, sec, ms, dhour, dmin;
        //2015-02-23T16:23:17.770+01:00

        swscanf(timestring, L"%4d-%2d-%2dT%2d:%2d:%2d.%3d+%2d:%2d",
                &year,
                &month,
                &day,
                &hour,
                &min,
                &sec,
                &ms,
                &dhour,
                &dmin
        );

        year -= 1900;

        long t = sec + min * 60 + hour * 3600;// + day * 86400 +
//                (year - 70) * 31536000 + ((year - 69) / 4) * 86400 -
//                ((year - 1) / 100) * 86400 + ((year + 299) / 400) * 86400;

        long millit = t * 1000;
        millit += ms;
        return millit;
    } else {
        return -1;
    }
}

void Analyzer::updateSpeeds() {
    std::ofstream output("C:\\Users\\arno\\Documents\\git\\trackanalyzer\\speed.txt");
    double minSpeed = 1e99;
    double maxSpeed = 0;

    int iMax = -1;
    int iMin = -1;

    avgspeed->add(0.0);

    int l = locations.size();
    for(int i = 1; i<l; i++) {
        Location& l1 = locations[i-1];
        Location& l2 = locations[i];
        double s = l2.speed(l1);
        avgspeed->add(s);
        s = avgspeed->avg();
        l2.setSpeed(s);
        output << i << " ; " << s << std::endl;
        if (s > maxSpeed) { maxSpeed = s; iMax = i;}
        if (s < minSpeed) { minSpeed = s; iMin = i;}
    }

    output.close();

    std::cout << "Minimum speed: " << minSpeed << " at " << iMin << std::endl;
    std::cout << "Maximum speed: " << maxSpeed << " at " << iMax << std::endl;
}

