
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Mon Sep 26 2003
 */

#include <kdebug.h>
#include "xmlcodecomment.h"

// Constructors/Destructors
//

XMLCodeComment::XMLCodeComment ( CodeDocument * doc, const QString & text )
        : CodeComment (doc, text)
{

}

XMLCodeComment::~XMLCodeComment ( ) { }

//
// Methods
//


// Accessor methods
//

// Other methods
//

/**
 * @return      QString
 */
QString XMLCodeComment::toString ( )
{

    QString output = "";

    // simple output method
    if (getWriteOutText())
    {
        QString indent = getIndentationString();
        QString endLine = getNewLineEndingChars();
        QString body = getText();
        output.append(indent+"<!-- ");
        if (!body.isEmpty())
            output.append(formatMultiLineText (body, indent, endLine));
        output.append(indent+"-->"+endLine);
    }

    return output;
}


#include "xmlcodecomment.moc"
