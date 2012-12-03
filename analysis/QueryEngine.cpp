//
//  QueryEngine.cpp
//  
//
//  Created by Thomas Scott on 11/18/12.
//
//

#include "QueryEngine.h"
#include <iostream>
#include <stdio.h>
#include <CLucene/StdHeader.h>
#include "CLucene/clucene-config.h"
#include <CLucene.h>
#include <CLucene/config/repl_tchar.h>
#include <CLucene/config/repl_wchar.h>
#include <CLucene/util/Misc.h>

#include "HardCoded.h"

#include "stdafx.h"
using namespace std;
using namespace lucene::analysis;
using namespace lucene::index;
using namespace lucene::util;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::search;

void QueryEngine::loadIndex(const char* index){
    reader = IndexReader::open(index);
    return;
}

void QueryEngine::verifyReader(){
    /*IndexReader* newreader = reader->reopen();
    if ( newreader != reader ){
        _CLLDELETE(reader);
        reader = newreader;
    }*/
    return;
}
void QueryEngine::setUpQuery(char*tmp){
    //TODO: take input from UI and set as query term
    tmp[strlen(tmp)-1]=0;
    STRCPY_AtoT(tline,tmp,80);
    return;
}

void QueryEngine::runFuzzyQuery(char* tmp){
    /*setUpQuery(tmp);
    verifyReader();
    IndexSearcher s(reader);
    if ( tmp[0] == '\0' ) return;
    FuzzyQuery* q = FuzzyQuery(term,float_t minimumSimilarity=0.9,&analyzer);
    cerr<<"Not fully implemented.";
     */
    return;
}

void QueryEngine::runLuceneQuery(char*tmp){
    setUpQuery(tmp);
    IndexSearcher s(reader);
    Query* q = QueryParser::parse(tline,_T("contents"),&analyzer);
    buffer = q->toString(_T("contents"));
    _CLDELETE_CARRAY(buffer);
    uint64_t start = lucene::util::Misc::currentTimeMillis();
    hits=s.search(q);
    endSearch = lucene::util::Misc::currentTimeMillis() - start;
    _CLDELETE(q);
    s.close();
    return;
}

void QueryEngine::getResults(){
    Document* doc;
    for ( int32_t i=0;i<hits->length();i++ ){
        doc = &hits->doc(i);
        //const TCHAR* buf = doc.get(_T("contents"));
        _tprintf(_T("%d. %s - %f\n"), i, doc->get(_T("path")), hits->score(i));
    }
    return;
}

void QueryEngine::clean(){
    _CLDELETE(hits);
    return;
}

void QueryEngine::execute(){
    loadIndex(INDEX_LOC);
    return;
}