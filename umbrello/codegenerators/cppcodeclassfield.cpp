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
 *      Date   : Mon Sep 1 2003
 */


#include <kdebug.h>

#include "cppcodeclassfield.h"
#include "cppcodegenerationpolicy.h"

#include "../codegenerator.h"
#include "../classifiercodedocument.h"
#include "../attribute.h"
#include "../umlobject.h"
#include "../umlrole.h"
#include "../uml.h"

// #include "cppclassifiercodedocument.h"

// Constructors/Destructors
//

CPPCodeClassField::CPPCodeClassField (ClassifierCodeDocument * parentDoc, UMLRole * role)
        : CodeClassField(parentDoc, role)
{

}

CPPCodeClassField::CPPCodeClassField (ClassifierCodeDocument * parentDoc, UMLAttribute * attrib)
        : CodeClassField(parentDoc, attrib)
{

}

CPPCodeClassField::~CPPCodeClassField ( ) { }

//
// Methods
//

// Other methods
//

QString CPPCodeClassField::getFieldName() {
    if (parentIsAttribute())
    {
        UMLAttribute * at = (UMLAttribute*) getParentObject();
        return cleanName(at->getName());
    }
    else
    {
        UMLRole * role = (UMLRole*) getParentObject();
        QString roleName = role->getName();
        if (fieldIsSingleValue()) {
            return roleName.replace(0, 1, roleName.left(1).lower());
        } else {
            return roleName.lower() + "Vector";
        }
    }
}

QString CPPCodeClassField::getListFieldClassName () {
    CodeGenPolicyExt * p = UMLApp::app()->getPolicyExt();
    CPPCodeGenerationPolicy *policy = dynamic_cast<CPPCodeGenerationPolicy*>(p);
    return policy->getVectorClassName();
}

QString CPPCodeClassField::getInitialValue() {

    if (parentIsAttribute())
    {
        UMLAttribute * at = dynamic_cast<UMLAttribute*>( getParentObject() );
        if (at) {
            return fixInitialStringDeclValue(at->getInitialValue(), getTypeName());
        } else {
            kError() << "CPPCodeClassField::getInitialValue: parent object is not a UMLAttribute"
            << endl;
            return "";
        }
    }
    else
    {
        if (fieldIsSingleValue()) {
            // FIX : IF the multiplicity is "1" then we should init a new object here, if its 0 or 1,
            //       then we can just return 'empty' string (minor problem).
            return "";
        } else {
            return " new "+getListFieldClassName()+"( )";
        }
    }

}


#include "cppcodeclassfield.moc"
