
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


#include "codegeneratorfactory.h"
#include "umldoc.h"

// Constructors/Destructors
//  

CodeGeneratorFactory::CodeGeneratorFactory ( ) {
};

CodeGeneratorFactory::~CodeGeneratorFactory ( ) { };

//  
// Methods
//  


// Accessor methods
//  


/**
 * Get the value of m_defaultcodegenerationpolicy
 * @return the value of m_defaultcodegenerationpolicy
 */
CodeGenerationPolicy * CodeGeneratorFactory::getDefaultCodeGenerationPolicy ( ) {
	return m_defaultcodegenerationpolicy;
}

/**
 * Get the value of m_dialog
 * @return the value of m_dialog
 */
/*
CodeGeneratorFactoryDialog * CodeGeneratorFactory::getDialog ( ) {
	return m_dialog;
}
*/

// Other methods
//  


/**
 * Create a new code document for a given UMLDoc.
 * @return	CodeGenerator
 * @param	document 
 */
CodeGenerator CodeGeneratorFactory::newCodeGenerator (UMLDoc * document ) {

}


/**
 * Allows you to create a new code generator from a UML document AND the XMI which
 * stores saved parameters.
 * @return	CodeGenerator
 * @param	doc 
 * @param	element 
 */
CodeGenerator CodeGeneratorFactory::newCodeGenerator (UMLDoc * doc, QDomElement & element ) {

}


