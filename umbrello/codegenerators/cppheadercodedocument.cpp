
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
 *      Date   : Thu Aug 28 2003
 */

/**
  We carve the CPP document up into 2 documents, "source" and "header". 
  This one represents the header portion.
  The sections of each are as follows:

  * header
  * includes 
  * import statements
  * class declaration
  *   guts of the class (e.g. field decl, accessor methods, operations, dependant classes)
*/

#include <kdebug.h>
#include <kdebug.h>
#include <qregexp.h>

#include "cppheadercodedocument.h"
#include "cppcodegenerator.h"
#include "cppheadercodeaccessormethod.h"
#include "cppheadercodeoperation.h"
#include "cppheaderclassdeclarationblock.h"
#include "cppheadercodeclassfielddeclarationblock.h"

// Constructors/Destructors
//  

CPPHeaderCodeDocument::CPPHeaderCodeDocument ( UMLClassifier * concept, CPPCodeGenerator *parent) 
   : ClassifierCodeDocument (concept, (CodeGenerator *) parent ) { 
	init ( );
};

CPPHeaderCodeDocument::~CPPHeaderCodeDocument ( ) { 
 	kdDebug()<<"   CPPHeaderCodeGenerator "<<this<<" destroyed"<<endl;
};

//  
// Methods
//  

// Accessor methods
//

QString CPPHeaderCodeDocument::getCPPClassName (QString name) {
	CodeGenerator *g = getParentGenerator();
	return g->cleanName(name);
}

// Other methods
//

// we will put the class 'guts' inside a hierarchical code block
CPPHeaderClassDeclarationBlock * CPPHeaderCodeDocument::getClassDecl ( )
{

        // So we see if it already exists, IF it *does* then we wont create a 
	// new one.
        CPPHeaderClassDeclarationBlock * codeBlock = (CPPHeaderClassDeclarationBlock *) findTextBlockByTag("classBlock");
	if(!codeBlock) {
		codeBlock = new CPPHeaderClassDeclarationBlock (this);
		codeBlock->setTag("classBlock");
	} 

        return codeBlock;

}

// a little utility method 
bool CPPHeaderCodeDocument::forceDoc () {
        return getParentGenerator()->forceDoc();
}

// add declaration blocks for the passed classfields
void CPPHeaderCodeDocument::declareClassFields (QPtrList<CodeClassField> & list ,
       				 HierarchicalCodeBlock * classDeclBlock )
{
     	for (CodeClassField * field = list.first(); field ; field = list.next())
	{
		CodeClassFieldDeclarationBlock * declBlock = field->getDeclarationCodeBlock();
		classDeclBlock->addTextBlock(declBlock); // wont add it IF its already present
        }
}

/**
 * Save the XMI representation of this object
 * @return      bool    status of save
 */
bool CPPHeaderCodeDocument::saveToXMI ( QDomDocument & doc, QDomElement & root ) {
        bool status = true;

        QDomElement docElement = doc.createElement( "cppheadercodedocument" );

        setAttributesOnNode(doc, docElement);

        root.appendChild( docElement );

        return status;
}

/**
 * load params from the appropriate XMI element node.
 */
void CPPHeaderCodeDocument::loadFromXMI ( QDomElement & root ) {
        setAttributesFromNode(root);
}

/** set attributes of the node that represents this class
 * in the XMI document.
 */
void CPPHeaderCodeDocument::setAttributesOnNode ( QDomDocument & doc, QDomElement & docElement)
{

        // superclass call
        ClassifierCodeDocument::setAttributesOnNode(doc,docElement);

        // now set local attributes/fields
// FIX
}

/** set the class attributes of this object from
 * the passed element node.
 */
void CPPHeaderCodeDocument::setAttributesFromNode ( QDomElement & root)
{

        // superclass save
        ClassifierCodeDocument::setAttributesFromNode(root);

        // now set local attributes
// FIX

}

// Initialize this cpp classifier code document
void CPPHeaderCodeDocument::init ( ) {

	setFileExtension(".cpp");

	initCodeClassFields(); // we have to call here as .newCodeClassField is pure virtual in parent class 

	// this will call updateContent() as well as other things that sync our document.
        syncronize();
}

/**
 * @param       op
 */
// in the vannilla version, we just tack all operations on the end
// of the document
bool CPPHeaderCodeDocument::addCodeOperation (CodeOperation * op ) {

	if(!op->getParentOperation()->isConstructorOperation())
        	return operationsBlock->addTextBlock(op);
	else
        	return constructorBlock->addTextBlock(op);
}

/**
 * create a new CodeAccesorMethod object belonging to this CodeDocument.
 * @return      CodeAccessorMethod
 */
CodeAccessorMethod * CPPHeaderCodeDocument::newCodeAccessorMethod( CodeClassField *cf, CodeAccessorMethod::AccessorType type ) {
        return new CPPHeaderCodeAccessorMethod((CPPCodeClassField*)cf, type);
}


CodeClassField * CPPHeaderCodeDocument::newCodeClassField ( UMLAttribute * at) {
        return new CPPCodeClassField(this,at);
}

CodeClassField * CPPHeaderCodeDocument::newCodeClassField ( UMLRole * role) {
        return new CPPCodeClassField(this,role);
}

/**
 * create a new CodeOperation object belonging to this CodeDocument.
 * @return      CodeOperation
 */
CodeOperation * CPPHeaderCodeDocument::newCodeOperation( UMLOperation * op) {
        return new CPPHeaderCodeOperation(this, op);
}

CodeClassFieldDeclarationBlock * CPPHeaderCodeDocument::newDeclarationCodeBlock (CodeClassField * cf ) {
        return new CPPHeaderCodeClassFieldDeclarationBlock((CPPCodeClassField*)cf);
}

// This method will cause the class to rebuild its text representation.
// based on the parent classifier object.
// For any situation in which this is called, we are either building the code
// document up, or replacing/regenerating the existing auto-generated parts. As
// such, we will want to insert everything we resonablely will want
// during creation. We can set various parts of the document (esp. the
// comments) to appear or not, as needed.
void CPPHeaderCodeDocument::updateContent( ) 
{

	// temp document construction
        constructorBlock = getHierarchicalCodeBlock("constructionMethods", "Constructors", 1);
        operationsBlock = getHierarchicalCodeBlock("operationMethods", "Operations", 1);

/*
      	// Gather info on the various fields and parent objects of this class...
	UMLClassifier * c = getParentClassifier();
	CodeGenerator * g = getParentGenerator();
	// CPPCodeGenerator * gen = dynamic_cast<CPPCodeGenerator*>(g);
	CPPCodeGenerator * gen = (CPPCodeGenerator*)g;

	// first, set the global flag on whether or not to show classfield info 
	QPtrList<CodeClassField> * cfList = getCodeClassFieldList();
	for(CodeClassField * field = cfList->first(); field; field = cfList->next())
		field->setWriteOutMethods(gen->getAutoGenerateAccessors());

      	// attribute-based ClassFields
      	// we do it this way to have the static fields sorted out from regular ones
        QPtrList<CodeClassField> staticAttribClassFields = getSpecificClassFields (CodeClassField::Attribute, true);
        QPtrList<CodeClassField> attribClassFields = getSpecificClassFields (CodeClassField::Attribute, false);
      	// association-based ClassFields 
      	// dont care if they are static or not..all are lumped together
        QPtrList<CodeClassField> plainAssocClassFields = getSpecificClassFields ( CodeClassField::PlainAssociation );
        QPtrList<CodeClassField> aggregationClassFields = getSpecificClassFields ( CodeClassField::Aggregation );
        QPtrList<CodeClassField> compositionClassFields = getSpecificClassFields ( CodeClassField::Composition );

        bool hasOperationMethods = c->getFilteredOperationsList()->last() ? true : false;
        QString endLine = gen->getNewLineEndingChars(); // a shortcut..so we dont have to call this all the time


	// START GENERATING CODE/TEXT BLOCKS and COMMENTS FOR THE DOCUMENT
	//

	//
        // PACKAGE CODE BLOCK
        //
	QString packageText = getPackage().isEmpty() ? "" : "package "+getPackage()+";";
	addOrUpdateTaggedCodeBlockWithComments("packages", packageText, "", 0, false);

       	// IMPORT CODEBLOCK 
       	//
       	// Q: Why all utils? Isnt just List and Vector the only classes we are using?
       	// A: doesn't matter at all; its more readable to just include '*' and cpp compilers
       	//    don't slow down or anything. (TZ)
       	QString importStatement = "";
       	if ( hasObjectVectorClassFields() )
       		importStatement.append("import cpp.util.*;"+endLine);

   	//only import classes in a different package from this class
       	UMLClassifierList imports;
       	QMap<UMLClassifier *,QString> *packageMap = new QMap<UMLClassifier*,QString>; // so we dont repeat packages

	gen->findObjectsRelated(c,imports);
       	for(UMLClassifier *con = imports.first(); con ; con = imports.next())
       	if ((con->getPackage() != c->getPackage())
       	        && !(packageMap->contains(con)))
	{
       		packageMap->insert(con,con->getPackage());
               	importStatement.append("import "+con->getPackage()+"."+gen->cleanName(con->getName())+";"+endLine);
	}
        // now, add/update the imports codeblock
	addOrUpdateTaggedCodeBlockWithComments("imports", importStatement, "", 0, false);

	// CLASS DECLARATION BLOCK
	//

	// get the declaration block. If its not already present, add it too
        CPPHeaderClassDeclarationBlock * classDeclBlock = getClassDecl ( );
	addTextBlock(classDeclBlock); // note: wont add if already present

	// NOW create document in sections..
	// now we want to populate the body of our class
	// our layout is the following general groupings of code blocks:

	// start cpp classifier document

	// header comment

	// package code block

	// import code block

	// class declaration 

	//   section:
	//   - class field declaration section comment
	//   - class field declarations (0+ codeblocks)
 
	//   section:
	//   - methods section comment

	//     sub-section: constructor ops
	//     - constructor method section comment
	//     - constructor methods (0+ codeblocks)

	//     sub-section: accessors
	//     - accessor method section comment
	//     - static accessor methods (0+ codeblocks)
	//     - non-static accessor methods (0+ codeblocks)

	//     sub-section: non-constructor ops 
	//     - operation method section comment
	//     - operations (0+ codeblocks)

	// end class declaration 

	// end cpp classifier document


	// Q: Why use the more complicated scheme of arranging code blocks within codeblocks?
	// A: This will allow us later to preserve the format of our document so that if 
	//    codeblocks are added, they may be easily added in the correct place, rather than at
	//    the end of the document, or by using a difficult algorithm to find the location of
	//    the last appropriate code block sibling (which may not exist.. for example user adds
	//    a constructor operation, but there currently are no constructor code blocks 
	//    within the document). 

	//
	// * CLASS FIELD declaration section
	//

	// get/create the field declaration code block
        HierarchicalCodeBlock * fieldDeclBlock = classDeclBlock->getHierarchicalCodeBlock("fieldsDecl", "Fields", 1);

        // Update the comment: we only set comment to appear under the following conditions
        CodeComment * fcomment = fieldDeclBlock->getComment();
        if (!forceDoc() && !hasClassFields() )
		fcomment->setWriteOutText(false);
	else
		fcomment->setWriteOutText(true);

	// now actually declare the fields within the appropriate HCodeBlock
	declareClassFields(staticAttribClassFields, fieldDeclBlock);
	declareClassFields(attribClassFields, fieldDeclBlock);
	declareClassFields(plainAssocClassFields, fieldDeclBlock);
	declareClassFields(aggregationClassFields, fieldDeclBlock);
	declareClassFields(compositionClassFields, fieldDeclBlock);

	//
        // METHODS section
        //

        // get/create the method codeblock
        HierarchicalCodeBlock * methodsBlock = classDeclBlock->getHierarchicalCodeBlock("methodsBlock", "Methods", 1);

        // Update the section comment
        CodeComment * methodsComment = methodsBlock->getComment();
        // set conditions for showing this comment
        if (!forceDoc() && !hasClassFields() && !hasOperationMethods)
                methodsComment->setWriteOutText(false);
        else
                methodsComment->setWriteOutText(true);

        // METHODS sub-section : constructor methods
        //

	// get/create the constructor codeblock
        HierarchicalCodeBlock * constBlock = methodsBlock->getHierarchicalCodeBlock("constructorMethods", "Constructors", 1);
	constructorBlock = constBlock; // record this codeblock for later, when operations are updated

	// special condiions for showing comment: only when autogenerateding empty constructors
	// Although, we *should* check for other constructor methods too
	CodeComment * constComment = constBlock->getComment();
	if (!forceDoc() && (parentIsInterface() || !gen->getAutoGenerateConstructors()))
		constComment->setWriteOutText(false);
	else
		constComment->setWriteOutText(true);

	// add/get the empty constructor 
	QString CPPClassName = getCPPClassName(c->getName()); 
	QString emptyConstStatement = "public "+CPPClassName+" ( ) { }";
	CodeBlockWithComments * emptyConstBlock = 
		constBlock->addOrUpdateTaggedCodeBlockWithComments("emptyconstructor", emptyConstStatement, "Empty Constructor", 1, false);
	// Now, as an additional condition we only show the empty constructor block 
	// IF it was desired to be shown
	if(!parentIsInterface() && gen->getAutoGenerateConstructors())
		emptyConstBlock->setWriteOutText(true);
	else
		emptyConstBlock->setWriteOutText(false);

        // METHODS subsection : ACCESSOR METHODS 
	//

        // get/create the accessor codeblock
        HierarchicalCodeBlock * accessorBlock = methodsBlock->getHierarchicalCodeBlock("accessorMethods", "Accessor Methods", 1);

	// set conditions for showing section comment
        CodeComment * accessComment = accessorBlock->getComment();
        if (!forceDoc() && !hasClassFields())
		accessComment->setWriteOutText(false);
	else
		accessComment->setWriteOutText(true);

	// now, 2 sub-sub sections in accessor block
        // add/update accessor methods for attributes
        HierarchicalCodeBlock * staticAccessors = accessorBlock->getHierarchicalCodeBlock("staticAccessorMethods", "", 1);
	staticAccessors->getComment()->setWriteOutText(false); // never write block comment
	staticAccessors->addCodeClassFieldMethods(staticAttribClassFields); 
	staticAccessors->addCodeClassFieldMethods(attribClassFields);

        // add/update accessor methods for associations
        HierarchicalCodeBlock * regularAccessors = accessorBlock->getHierarchicalCodeBlock("regularAccessorMethods", "", 1);
	regularAccessors->getComment()->setWriteOutText(false); // never write block comment
	regularAccessors->addCodeClassFieldMethods(plainAssocClassFields);
	regularAccessors->addCodeClassFieldMethods(aggregationClassFields);
	regularAccessors->addCodeClassFieldMethods(compositionClassFields);

        // METHODS subsection : Operation methods (which arent constructors) 
	//

        // get/create the operations codeblock
        HierarchicalCodeBlock * opsBlock = methodsBlock->getHierarchicalCodeBlock("operationMethods", "Operations", 1);
	operationsBlock = opsBlock; // record this so that later operations go in right place

	// set conditions for showing section comment
	CodeComment * ocomment = opsBlock->getComment();
        if (!forceDoc() && !hasOperationMethods )
		ocomment->setWriteOutText(false);
	else
		ocomment->setWriteOutText(true);

*/
}

