
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
 *      Date   : Tue Jul 1 2003
 */



#ifndef CODEACCESSORMETHOD_H
#define CODEACCESSORMETHOD_H

#include <qstring.h>
#include <qptrlist.h>
#include "codemethodblock.h"

class CodeClassField;
class UMLAttribute;
class UMLObject;
class UMLRole;

class CodeAccessorMethod : public CodeMethodBlock
{
	Q_OBJECT
public:

	// some types of accessor methods that are possible:
	// "GET" is to retrieve single-valued (primative or Object) fields
	// "SET" is to set single-valued (primative or Object) fields
	// "ADD" is to add a value to a multiple-valued field of either primative or Object items
	// "REMOVE" is to remove a value to a multiple-valued field of either primative or Object items
	// "LIST" is to retrive the entire list of items in a  multiple-valued field 
	enum AccessorType {GET=0, SET, ADD, REMOVE, LIST};

	// Constructors/Destructors
	//  


	/**
	 * Constructors 
	 */
	CodeAccessorMethod ( CodeClassField * field );

	/**
	 * Empty Destructor
	 */
	virtual ~CodeAccessorMethod ( );

	/**
	 * Get the value of m_parentclassfield
	 * @return the value of m_parentclassfield
	 */
	CodeClassField * getParentClassField ( );

	/** return the type of accessor method this is
 	 */
	AccessorType getType( );

	/** Set the type of accessor method this is
	 */
	void setType ( AccessorType type);

 	/** Utility method to get the value of the parent object of the parent classifield.
	 */
	// virtual UMLObject * getParentObject();

	bool parentIsAttribute();

        /**
         * Save the XMI representation of this object
         * @return      bool    status of save
         */
        virtual bool saveToXMI ( QDomDocument & doc, QDomElement & root ) = 0;

        /**
         * load params from the appropriate XMI element node.
         */
        virtual void loadFromXMI ( QDomElement & root ) = 0;

        /** set the class attributes from a passed object
         */
        virtual void setAttributesFromObject (TextBlock * obj);

protected:

	virtual void release ();

       /** set attributes of the node that represents this class
         * in the XMI document.
         */
        virtual void setAttributesOnNode ( QDomDocument & doc, QDomElement & blockElement);

        /** set the class attributes of this object from
         * the passed element node.
         */
        virtual void setAttributesFromNode ( QDomElement & element);

        virtual void updateMethodDeclaration() = 0;

        virtual void updateContent() = 0;

private:

	CodeClassField * m_parentclassfield;
	AccessorType m_accessorType;

	void initFields(CodeClassField * parentCF );

};

#endif // CODEACCESSORMETHOD_H
