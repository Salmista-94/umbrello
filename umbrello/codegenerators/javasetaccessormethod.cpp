
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
 *      Date   : Fri Jun 20 2003
 */

#include <kdebug.h>
#include "javasetaccessormethod.h"

#include "../attribute.h"
#include "../classifiercodedocument.h"
#include "../umlobject.h"
#include "../umlrole.h"

#include "javaclassifiercodedocument.h"
#include "javacodeclassfield.h"
#include "javacodedocumentation.h"

// Constructors/Destructors
//  

JavaSetAccessorMethod::JavaSetAccessorMethod ( JavaCodeClassField * field )
   : CodeAccessorMethod ( (CodeClassField*) field ) 
{
        updateMethodDeclaration();
        updateContent();
};

JavaSetAccessorMethod::~JavaSetAccessorMethod ( ) { };

// Other methods
//

/** Save the XMI representation of this object
 * @return      bool    status of save
 */
bool JavaSetAccessorMethod::saveToXMI ( QDomDocument & doc, QDomElement & root ) {
        bool status = true;

        QDomElement blockElement = doc.createElement( "codeaccessormethod" );

        // set attributes
        setAttributesOnNode(doc, blockElement);

        root.appendChild( blockElement );

        return status;
}

void JavaSetAccessorMethod::setAttributesOnNode ( QDomDocument & doc, QDomElement & blockElement)
{

        // set super-class attributes
        CodeAccessorMethod::setAttributesOnNode(doc, blockElement);

        // set local attributes now

}

/**
 * load params from the appropriate XMI element node.
 */
void JavaSetAccessorMethod::loadFromXMI ( QDomElement & root )
{
        setAttributesFromNode(root);
}

void JavaSetAccessorMethod::setAttributesFromNode( QDomElement & root)
{

        // set attributes from superclass method the XMI
        CodeAccessorMethod::setAttributesFromNode(root);

        // load local stuff

}

void JavaSetAccessorMethod::updateContent( )
{

	CodeClassField * parentField = getParentClassField();
	JavaCodeClassField * javafield = (JavaCodeClassField*)parentField;
        QString fieldName = javafield->getFieldName();

	setText(fieldName+" = value;");
}

// we basically want to update the doc and start text of this method
void JavaSetAccessorMethod::updateMethodDeclaration()
{

	CodeClassField * parentField = getParentClassField();
	ClassifierCodeDocument * doc = parentField->getParentDocument();

	// JavaClassifierCodeDocument * javadoc = dynamic_cast<JavaClassifierCodeDocument*>(doc);
	JavaClassifierCodeDocument * javadoc = (JavaClassifierCodeDocument*) doc;
	JavaCodeClassField * javafield = (JavaCodeClassField*)parentField;
	
        QString strVis = javadoc->scopeToJavaDecl(getParentClassField()->getVisibility());
        QString fieldName = javafield->getFieldName();
        QString fieldType = javafield->getTypeName();
        QString methodName = "set"+javadoc->capitalizeFirstLetter(fieldName);
	QString endLine = javadoc->getNewLineEndingChars();

        // set method
	JavaCodeDocumentation * header = new JavaCodeDocumentation(javadoc);
        if(!getParentObject()->getDoc().isEmpty())
                header->setText("Set the value of "+fieldName+endLine+getParentObject()->getDoc()+endLine);
        setComment(header);

	setStartMethodText(strVis+" void "+methodName+" ( "+fieldType+" value ) {");
        setEndMethodText("}");

	setOverallIndentationLevel(1);

}
