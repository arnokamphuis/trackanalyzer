#include <iostream>

#include "Analyzer.h"

int main() {
    char xmlFile[] = "C:\\Users\\arno\\Documents\\git\\trackanalyzer\\data\\day10.xml";

    Analyzer analyzer;
    analyzer.loadData(xmlFile);
    analyzer.updateSpeeds();
    return 0;
}