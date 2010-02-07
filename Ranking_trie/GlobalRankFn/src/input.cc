/*
  $Id: input.cc 4026 2008-10-01 00:23:25Z abehm $

  Copyright (C) 2007 by The Regents of the University of California

  Redistribution of this file is permitted under the terms of the
  BSD license

  Date: 02/16/2007
  Author: Rares Vernica <rares (at) ics.uci.edu>
*/

#include "input.h"

void readString(vector<string> &data, const string &filenameData, unsigned count, 
                unsigned maxLineLen)
{
  ifstream fileData(filenameData.c_str());
  if (!fileData) {
    cerr << "can't open input file \"" << filenameData << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  cerr << "reading \"" << filenameData << "\"...";
  cerr.flush();

  char line[maxLineLen + 1];
  bool isIgnore = false;

  while (true) {
    fileData.getline(line, maxLineLen + 1);
    if (fileData.eof())
      break;
    if (fileData.rdstate() & ios::failbit) {
      isIgnore = true;
      while (fileData.rdstate() & ios::failbit) {      
        fileData.clear(fileData.rdstate() & ~ios::failbit);
        fileData.getline(line, maxLineLen);
      }
      cerr << "open reading input file \"" << filenameData << "\"" << endl
           << "line length might exceed " << maxLineLen << " characters" << endl;
      exit(EXIT_FAILURE);
    }
    else
      data.push_back(string(line));
    if (count != 0 && data.size() == count)
      break;
  }

  fileData.close();

  if (isIgnore)
    cerr << "WARNING" << endl 
         << "some lines in the file exceeded " << maxLineLen 
         << " characters and were ignored" << endl;
  else
    cerr << "OK" << endl;

}

bool existFileBin(const string &filename)  
{
  ifstream file(filename.c_str(), ios::in | ios::binary);
  if (!file) 
    return false;
  file.close();
  return true;
}

bool existFile(const string &filename) 
{
  ifstream file(filename.c_str(), ios::in);
  if (!file) 
    return false;
  file.close();
  return true;
}
