/*
  $Id: input.h 4100 2008-11-07 21:50:39Z abehm $

  Copyright (C) 2007 by The Regents of the University of California

  Redistribution of this file is permitted under the terms of the
  BSD license

  Date: 02/16/2007
  Author: Rares Vernica <rares (at) ics.uci.edu>
*/

#ifndef _input_h_
#define _input_h_

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void readString(vector<string> &data, const string &filenameData, unsigned count = 0,
                unsigned maxLineLen = 255);
// ignores lines with length over maxLineLen, those lines will not affect the count

template <typename T> 
void readBin(vector<T> &data, 
             const string &filenameData)
{
  ifstream fileData(filenameData.c_str(), ios::in | ios::binary);
  if (!fileData) {
    cerr << "can't open input file \"" << filenameData << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  cerr << "reading \"" << filenameData << "\"...";
  cerr.flush();

  T e;
  while (true) {
    fileData.read(reinterpret_cast<char*>(&e), sizeof(T));
    if (fileData.eof())
      break;
    data.push_back(e);
  }
    
  fileData.close();

  cerr << "OK" << endl;
}

bool existFile(const string &filename);
bool existFileBin(const string &filename);

#endif
