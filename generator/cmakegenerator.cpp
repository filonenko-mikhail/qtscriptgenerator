/****************************************************************************
**
** Copyright (C) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Script Generator project on Qt Labs.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "cmakegenerator.h"
#include "reporthandler.h"
#include "fileout.h"

void CMakeGenerator::generate()
{
  QHashIterator<QString, Pri> cmake(priHash);
  while (cmake.hasNext()) {
      cmake.next();
      QStringList list = cmake.value().headers;
      if (list.isEmpty())
          continue;

      QString folder = cmake.key();
      FileOut file(m_out_dir + "/generated_cpp/" + folder + "/" + folder + ".cmake");
      file.stream << "get_filename_component("+ folder+"_PATH ${CMAKE_CURRENT_LIST_FILE} PATH)\n";
      file.stream << "list(APPEND QS_HEADERS \n";
      qSort(list.begin(), list.end());
      foreach (const QString &entry, list) {
        file.stream << "${" + folder+"_PATH}/" << entry << " \n";
      }
      file.stream << ")";

      file.stream << "\n";
      file.stream << "list(APPEND QS_SOURCES \n";
      list = cmake.value().sources;
      qSort(list.begin(), list.end());
      foreach (const QString &entry, list) {
        file.stream << "${" + folder+"_PATH}/" << entry << " \n";
      }
      file.stream << "${" + folder+"_PATH}/" << folder << "_init.cpp\n";
      file.stream << ")";

      if (file.done())
          ++m_num_generated_written;
      ++m_num_generated;
  }

  PriGenerator::generate();
}
