/*
  $Id: output.cc 4026 2008-10-01 00:23:25Z abehm $

  Copyright (C) 2007 by The Regents of the University of California

  Redistribution of this file is permitted under the terms of the
  BSD license

  Date: 01/30/2007
  Author: Rares Vernica <rares (at) ics.uci.edu>
*/

#include "output.h"

void writeString(const vector<string> &data, const string &filenameData)
{
  ofstream fileData(filenameData.c_str(), ios::out);
  if (!fileData) {
    cerr << "can't open output file \"" << filenameData << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  cerr << "writing \"" << filenameData << "\"...";
  cerr.flush();
  
  for (vector<string>::const_iterator it = data.begin();
       it !=  data.end(); ++it)
    fileData << *it << endl;
    
  fileData.close();

  cerr << "OK" << endl;
}
