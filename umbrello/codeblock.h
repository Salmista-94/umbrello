
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
 *      Date   : Wed Jun 18 2003
 */



#ifndef CODEBLOCK_H
#define CODEBLOCK_H

#include <qstring.h>
#include <qptrlist.h>

#include "textblock.h"
// #include "codeblockdialog.h"

class HierarchicalCodeBlock;


/**
  * class CodeBlock
  * A "chunk" of code within the code document. 
  */

class CodeBlock : public TextBlock
{
	Q_OBJECT
public:

	enum ContentType {AutoGenerated=0, UserGenerated };

	// Constructors/Destructors
	//  

	/** constructor with QString so we can create & populate it in
	 * one step.
	 */
	CodeBlock ( CodeDocument * parent, const QString & body = "");
	CodeBlock ( HierarchicalCodeBlock * parent, const QString & body = "" );

	/**
	 * Empty Destructor
	 */
	virtual ~CodeBlock ( );

	// Public attributes
	//  


	// Public attribute accessor methods
	//  

	/**
	 * Set the value of m_contentType
	 * specifies whether the content (text) of this object was generated by the code
	 * generator or was supplied by the user.
	 * @param new_var the new value of m_contentType
	 */
	void setContentType ( ContentType new_var );

	/**
	 * Get the value of m_contentType
	 * specifies whether the content (text) of this object was generated by the code
	 * generator or was supplied by the user.
	 * @return the value of m_contentType
	 */
	ContentType getContentType ( );

	/**
	 * Get the value of m_dialog
	 * @return the value of m_dialog
	 */
//	CodeBlockDialog getDialog ( );

      /**
         * Save the XMI representation of this object
         * @return      bool    status of save
         */
        virtual bool saveToXMI ( QDomDocument & doc, QDomElement & root );

        /**
         * load params from the appropriate XMI element node.
         */
        virtual void loadFromXMI ( QDomElement & root );


protected:

       /** set attributes of the node that represents this class
         * in the XMI document.
         */
        virtual void setAttributesOnNode ( QDomDocument & doc, QDomElement & blockElement);

        /** set the class attributes of this object from
         * the passed element node.
         */
        virtual void setAttributesFromNode ( QDomElement & element);


private:

//	CodeBlockDialog * m_dialog;

	// specifies whether the content (text) of this object 
	// was generated by the code generator or was supplied by the user (or some other way).
	ContentType m_contentType; 

	void initFields ( );

};

#endif // CODEBLOCK_H
