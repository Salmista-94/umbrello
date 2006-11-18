/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2004-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Wed Jun 18 2003
 */

#include <kdebug.h>
#include <qregexp.h>
#include <qdatetime.h>

#include "codedocument.h"
#include "codegenerator.h"
#include "package.h"
#include "umldoc.h"
#include "uml.h"

// Constructors/Destructors
//

CodeDocument::CodeDocument () : CodeGenObjectWithTextBlocks(this)
{
    initDoc();
}


CodeDocument::~CodeDocument ( ) {
    // delete all the text blocks we have
    TextBlock *tb;
    for (TextBlockListIt it(m_textblockVector); (tb = it.current()) != NULL; ++it)
        delete tb;
    m_textblockVector.clear();
    delete m_header;
}

//
// Methods
//


// Accessor methods
//


// Public attribute accessor methods
//


/**
 * Set the value of m_filename
 * @param new_var the new value of m_filename
 */
void CodeDocument::setFileName ( const QString &new_var ) {
    m_filename = new_var;
}

/**
 * Get the value of m_filename
 * @return the value of m_filename
 */
QString CodeDocument::getFileName ( ) const {
    return m_filename;
}

/**
 * Set the value of m_filename
 * @param new_var the new value of m_filename
 */
void CodeDocument::setFileExtension ( const QString &new_var ) {
    m_fileExtension = new_var;
    updateHeader(); // because we are using new heading file
}

/**
 * Get the value of m_filename
 * @return the value of m_filename
 */
QString CodeDocument::getFileExtension( ) const {
    return m_fileExtension;
}

/**
 * Set the value of the package.
 * @param new_var the new value of m_package
 */
void CodeDocument::setPackage ( UMLPackage *new_var ) {
    m_package = new_var;
}

/**
 * Get the value of path for this code document.
 * @return the value of the path
 */
QString CodeDocument::getPath ( ) {

    QString path = getPackage();

    // Replace all white spaces with blanks
    path.simplifyWhiteSpace();

    // Replace all blanks with underscore
    path.replace(QRegExp(" "), "_");

    // this allows multiple directory paths (ala Java, some other languages)
    // in from the package specification
    path.replace(QRegExp("\\."),"/"); // Simple hack!.. but this is more or less language
    // dependant and should probably be commented out.
    // Still, as a general default it may be useful -b.t.
    return path;
}

/**
 * Get the value of package name.
 * @return the value of m_package->getName()
 */
QString CodeDocument::getPackage ( ) const {
    if (m_package)
        return m_package->getName();
    return QString::null;
}

/**
 * Set the value of m_ID
 * @param new_var the new value of m_ID
 */
void CodeDocument::setID ( const QString &new_var ) {
    m_ID = new_var;
}

/**
 * Get the value of m_ID
 * @return the value of m_ID
 */
QString CodeDocument::getID ( ) const {
    return m_ID;
}

/**
 * Set the value of m_writeOutCode
 * Whether or not to write out this code document and any codeblocks, etc that it
 * owns.
 * @param new_var the new value of m_writeOutCode
 */
void CodeDocument::setWriteOutCode ( bool new_var ) {
    m_writeOutCode = new_var;
}

/**
 * Get the value of m_writeOutCode
 * Whether or not to write out this code document and any codeblocks, etc that it
 * owns.
 * @return the value of m_writeOutCode
 */
bool CodeDocument::getWriteOutCode ( ) {
    return m_writeOutCode;
}

/**
 * Set the Header comment
 */
void CodeDocument::setHeader ( CodeComment * header ) {
    m_header = header;
}

/**
 * Get the Header comment
 */
CodeComment * CodeDocument::getHeader ( ) {
    return m_header;
}

//
// Other methods
//

QString CodeDocument::getUniqueTag ( QString prefix )
{

    if(prefix.isEmpty())
        prefix = "tblock";

    QString tag = prefix + "_0";
    int number = lastTagIndex;
    for ( ; findTextBlockByTag(tag, true); number++) {
        tag = prefix + "_" + QString::number(number);
    }
    lastTagIndex = number;
    return tag;
}

/**
 * Insert a new text block before/after the existing text block. Returns
 * false if it cannot insert the textblock.
 */
bool CodeDocument::insertTextBlock(TextBlock * newBlock, TextBlock * existingBlock, bool after)
{

    if(!newBlock || !existingBlock)
        return false;

    QString tag = existingBlock->getTag();
    if(!findTextBlockByTag(tag, true))
        return false;

    int index = m_textblockVector.findRef(existingBlock);
    if(index < 0)
    {
        // may be hiding in child hierarchical codeblock
        for(TextBlock * tb = m_textblockVector.first(); tb ; tb = m_textblockVector.next())
        {
            HierarchicalCodeBlock * hb = dynamic_cast<HierarchicalCodeBlock*>(tb);
            if(hb && hb->insertTextBlock(newBlock, existingBlock, after))
                return true; // found, and inserted, otherwise keep going
        }
        // ugh. where is the child block?
        kWarning()<<" Warning: couldnt insert text block (tag:"<<newBlock->getTag()<<"). Reference text block (tag:"<<existingBlock->getTag()<<") not found."<<endl;
        return false;
    }

    // if we get here.. it was in this object so insert

    // check for tag FIRST
    QString new_tag = newBlock->getTag();

    // assign a tag if one doesn't already exist
    if(new_tag.isEmpty())
    {
        new_tag = getUniqueTag();
        newBlock->setTag(new_tag);
    }

    if(m_textBlockTagMap.contains(new_tag))
        return false; // return false, we already have some object with this tag in the list
    else
        m_textBlockTagMap.insert(new_tag, newBlock);

    if(after)
        index++;

    m_textblockVector.insert(index,newBlock);

    return true;
}

/**
 * Get the value of m_dialog
 * @return the value of m_dialog
 */
/*
CodeDocumentDialog * CodeDocument::getDialog ( ) {
    return m_dialog;
}
*/

// Other methods
//

QString CodeDocument::cleanName ( const QString &name ) {
    return CodeGenerator::cleanName(name);
}

// update the text and status of the head comment
void CodeDocument::updateHeader () {

    //try to find a heading file (license, coments, etc) then extract its text
    QString headingText = UMLApp::app()->getCommonPolicy()->getHeadingFile(getFileExtension());

    headingText.replace(QRegExp("%filename%"),getFileName()+getFileExtension());
    headingText.replace(QRegExp("%filepath%"),getPath());
    headingText.replace( QRegExp("%time%"), QTime::currentTime().toString());
    headingText.replace( QRegExp("%date%"), QDate::currentDate().toString());

    getHeader()->setText(headingText);

    // update the write out status of the header
    if(UMLApp::app()->getCommonPolicy()->getIncludeHeadings())
        getHeader()->setWriteOutText(true);
    else
        getHeader()->setWriteOutText(false);

}

/**
 * create the string representation of this object.
 * @return      QString
 */
QString CodeDocument::toString ( ) {

    // IF the whole document is turned "Off" then don't bother
    // checking individual code blocks, just send back empty string
    if(!getWriteOutCode())
        return QString("");

    QString content = getHeader()->toString();

    // update the time/date

    // comments, import, package codeblocks go next
    TextBlockList * items = getTextBlockList();
    for (TextBlock *c = items->first(); c; c = items->next())
    {
        if(c->getWriteOutText()) {
            QString str = c->toString();
            if(!str.isEmpty())
                content.append(str);
        }
    }
    return content;
}

void CodeDocument::synchronize() {
    updateContent();
}

// need to overload method to beable to clear the childTextBlockMap
void CodeDocument::resetTextBlocks() {
    CodeGenObjectWithTextBlocks::resetTextBlocks();
    m_childTextBlockTagMap.clear();
}

/**
 * load params from the appropriate XMI element node.
 */
void CodeDocument::loadFromXMI ( QDomElement & root ) {
    setAttributesFromNode(root);
}

/** set attributes of the node that represents this class
 * in the XMI document.
 */
void CodeDocument::setAttributesOnNode ( QDomDocument & doc, QDomElement & docElement)
{

    // superclass call
    CodeGenObjectWithTextBlocks::setAttributesOnNode(doc,docElement);

    // now set local attributes/fields
    docElement.setAttribute("fileName",getFileName());
    docElement.setAttribute("fileExt",getFileExtension());
    Uml::IDType pkgId = Uml::id_None;
    if (m_package)
        pkgId = m_package->getID();
    docElement.setAttribute("package", ID2STR(pkgId));
    docElement.setAttribute("writeOutCode",getWriteOutCode()?"true":"false");
    docElement.setAttribute("id",getID());

    // set the a header
    // which we will store in its own separate child node block
    QDomElement commElement = doc.createElement( "header" );
    getHeader()->saveToXMI(doc, commElement); // comment
    docElement.appendChild( commElement);

    // doc codePolicy?
    // FIX: store ONLY if different from the parent generator
    // policy.. something which is not possible right now. -b.t.

}

/** set the class attributes of this object from
 * the passed element node.
 */
void CodeDocument::setAttributesFromNode ( QDomElement & root) {

    // now set local attributes
    setFileName(root.attribute("fileName",""));
    setFileExtension(root.attribute("fileExt",""));
    QString pkgStr = root.attribute("package","");
    if (!pkgStr.isEmpty() && pkgStr != "-1") {
        UMLDoc *umldoc = UMLApp::app()->getDocument();
        if (pkgStr.contains( QRegExp("\\D") )) {
            // suspecting pre-1.5.3 file format where the package name was
            // saved instead of the package ID.
            UMLObject *o = umldoc->findUMLObject(pkgStr);
            m_package = dynamic_cast<UMLPackage*>(o);
        }
        if (m_package == NULL) {
            UMLObject *o = umldoc->findObjectById(STR2ID(pkgStr));
            m_package = dynamic_cast<UMLPackage*>(o);
        }
    }
    setWriteOutCode(root.attribute("writeOutCode","true") == "true" ? true : false);
    setID(root.attribute("id",""));

    // load comment now
    // by looking for our particular child element
    QDomNode node = root.firstChild();
    QDomElement element = node.toElement();
    while( !element.isNull() ) {
        QString tag = element.tagName();
        if( tag == "header" ) {
            QDomNode cnode = element.firstChild();
            QDomElement celem = cnode.toElement();
            getHeader()->loadFromXMI(celem);
            break;
        }
        node = element.nextSibling();
        element = node.toElement();
    }

    // a rare case where the super-class load is AFTER local attributes
    CodeGenObjectWithTextBlocks::setAttributesFromNode(root);
}

/**
 * Save the XMI representation of this object
 */
void CodeDocument::saveToXMI ( QDomDocument & doc, QDomElement & root ) {
    QDomElement docElement = doc.createElement( "codedocument" );

    setAttributesOnNode(doc, docElement);

    root.appendChild( docElement );
}

// vanilla code documents don't have much
// to do.. override this with a different
// version for your own documents
void CodeDocument::updateContent() {
    updateHeader(); // doing this insures time/date stamp is at the time of this call
}

/**
 * create a new CodeBlock object belonging to this CodeDocument.
 * @return      CodeBlock
 */
CodeBlock * CodeDocument::newCodeBlock ( ) {
    return new CodeBlock(this);
}

/**
 * create a new CodeBlockWithComments object belonging to this CodeDocument.
 * @return      CodeBlockWithComments
 */
CodeBlockWithComments * CodeDocument::newCodeBlockWithComments ( ) {
    return new CodeBlockWithComments(this);
}

HierarchicalCodeBlock * CodeDocument::newHierarchicalCodeBlock ( ) {
    HierarchicalCodeBlock *hb = new HierarchicalCodeBlock(this);
    //hb->update();
    return hb;
}

void CodeDocument::removeChildTagFromMap ( const QString &tag )
{
    m_childTextBlockTagMap.erase(tag);
}

void CodeDocument::addChildTagToMap ( const QString &tag, TextBlock * tb)
{
    m_childTextBlockTagMap.insert(tag, tb);
}

TextBlock * CodeDocument::findTextBlockByTag( const QString &tag , bool descendIntoChildren)
{
    //if we already know to which file this class was written/should be written, just return it.
    if(m_textBlockTagMap.contains(tag))
        return m_textBlockTagMap[tag];

    if (descendIntoChildren)
        if(m_childTextBlockTagMap.contains(tag))
            return m_childTextBlockTagMap[tag];

    return (TextBlock*) NULL;
}

void CodeDocument::initDoc () {

    m_writeOutCode = true;
    m_package = NULL;
    m_fileExtension = QString("");
    m_ID = QString(""); // leave with NO ID as a default

    //m_textblockVector.setAutoDelete(false);

    setHeader(new CodeComment(this));

    lastTagIndex = 0;

    //  m_dialog = new CodeDocumentDialog( );

}

TextBlock * CodeDocument::findCodeClassFieldTextBlockByTag ( const QString &tag ) {
    kWarning()<<"Called findCodeClassFieldMethodByTag("<<tag<<") for a regular CodeDocument"<<endl;
    return (TextBlock *) NULL;
}

#include "codedocument.moc"
