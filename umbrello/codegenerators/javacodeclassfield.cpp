
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
 *      Date   : Mon Jun 30 2003
 */


#include <kdebug.h>
#include <qregexp.h>

#include "javacodeclassfield.h"
#include "javacodecomment.h"

#include "../attribute.h"
#include "../umlobject.h"
#include "../umlrole.h"

#include "javagetaccessormethod.h"
#include "javasetaccessormethod.h"
#include "javaaddaccessormethod.h"
#include "javaremoveaccessormethod.h"
#include "javalistaccessormethod.h"
#include "javaclassifiercodedocument.h"

// Constructors/Destructors
//  

JavaCodeClassField::JavaCodeClassField (JavaClassifierCodeDocument * parentDoc, UMLRole * role) 
    : CodeClassField((ClassifierCodeDocument*)parentDoc, role) 
{

};

JavaCodeClassField::JavaCodeClassField (JavaClassifierCodeDocument * parentDoc, UMLAttribute * attrib) 
    : CodeClassField((ClassifierCodeDocument*)parentDoc, attrib) 
{

};

JavaCodeClassField::~JavaCodeClassField ( ) { };

//  
// Methods
//  

// Other methods
//  

// IF the type is "string" we need to declare it as
// the Java Object "String" (there is no string primative in Java).
// Same thing again for "bool" to "boolean"
QString JavaCodeClassField::fixTypeName(QString string)
{
        string.replace(QRegExp("^string$"),"String");
        string.replace(QRegExp("^bool$"),"boolean");
        return cleanName(string);
}

QString JavaCodeClassField::getFieldName() {
        if (parentIsAttribute()) 
	{
		UMLAttribute * at = (UMLAttribute*) getParentObject();
		return cleanName(at->getName());
	} 
	else
	{
		UMLRole * role = (UMLRole*) getParentObject();
		QString roleName = role->getName();
		if(fieldIsSingleValue()) {
			return roleName.replace(0, 1, roleName.left(1).lower());
		} else { 
			return roleName.lower() + "Vector";
		}
	}
}


QString JavaCodeClassField::getInitialValue() {

        if (parentIsAttribute()) 
	{
		UMLAttribute * at = (UMLAttribute*) getParentObject();
                return fixInitialStringDeclValue(at->getInitialValue(), getTypeName());
        } 
	else 
	{
		if(fieldIsSingleValue()) {
// FIX : IF the multiplicity is "1" then we should init a new object here, if its 0 or 1,
//       then we can just return 'empty' string (minor problem).
			return "";
		} else {
			return " new "+getListFieldClassName()+"( )";
		}
        }

}

