
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
  The sections of each are as follows:

  * header
  * includes 
  * constructor methods
  * all other methods

*/

#include <kdebug.h>
#include <qregexp.h>

#include "cppsourcecodedocument.h"
#include "cppcodegenerator.h"
#include "cppcodedocumentation.h"
#include "cppsourcecodeaccessormethod.h"
#include "cppsourcecodeoperation.h"
#include "cppsourcecodeclassfielddeclarationblock.h"

// Constructors/Destructors
//  

CPPSourceCodeDocument::CPPSourceCodeDocument ( UMLClassifier * concept, CPPCodeGenerator *parent) 
   : ClassifierCodeDocument (concept, (CodeGenerator *) parent ) { 
	init ( );
}

CPPSourceCodeDocument::~CPPSourceCodeDocument ( ) { }

//  
// Methods
//  

// Accessor methods
//

// Other methods
//

QString CPPSourceCodeDocument::getCPPClassName (QString name) {
        CPPCodeGenerator *g = (CPPCodeGenerator*) getParentGenerator();
        return g->getCPPClassName(name);
}

// a little utility method 
bool CPPSourceCodeDocument::forceDoc () {
        return getParentGenerator()->forceDoc();
}

// IF the classifier object is modified, this will get called.
// Possible mods include changing the filename and package
// the classifier has.
void CPPSourceCodeDocument::syncNamesToParent( ) 
{

        setFileName(getParentGenerator()->cleanName(getParentClassifier()->getName().lower()));
        setPackage(getParentGenerator()->cleanName(getParentClassifier()->getPackage().lower()));
}

// Initialize this cpp classifier code document
void CPPSourceCodeDocument::init ( ) {

	setFileExtension(".cpp");

	methodsBlock = 0;
	constructorBlock = 0;

	initCodeClassFields(); // we have to call here as .newCodeClassField is pure virtual in parent class 

	// this will call updateContent() as well as other things that sync our document.
        synchronize();
}

/**
 * @param       op
 */
// in the vannilla version, we just tack all operations on the end
// of the document
bool CPPSourceCodeDocument::addCodeOperation (CodeOperation * op ) {

	if(!op->getParentOperation()->isConstructorOperation())
	{
        	return methodsBlock->addTextBlock(op);
	} else
        	return constructorBlock->addTextBlock(op);
}

/**
 * create a new CodeAccesorMethod object belonging to this CodeDocument.
 * @return      CodeAccessorMethod
 */
CodeAccessorMethod * CPPSourceCodeDocument::newCodeAccessorMethod( CodeClassField *cf, CodeAccessorMethod::AccessorType type ) {
        return new CPPSourceCodeAccessorMethod((CPPCodeClassField*)cf, type);
}


CodeClassField * CPPSourceCodeDocument::newCodeClassField ( UMLAttribute * at) {
        return new CPPCodeClassField(this,at);
}

CodeClassField * CPPSourceCodeDocument::newCodeClassField ( UMLRole * role) {
        return new CPPCodeClassField(this,role);
}

/**
 * create a new CodeBlockWithComments object belonging to this CodeDocument.
 * @return      CodeBlockWithComments
 */
CodeComment * CPPSourceCodeDocument::newCodeComment ( ) {
        return new CPPCodeDocumentation(this);
}

/**
 * create a new CodeOperation object belonging to this CodeDocument.
 * @return      CodeOperation
 */
CodeOperation * CPPSourceCodeDocument::newCodeOperation( UMLOperation * op) {
        return new CPPSourceCodeOperation(this, op);
}

CodeClassFieldDeclarationBlock * CPPSourceCodeDocument::newDeclarationCodeBlock (CodeClassField * cf ) {
        return new CPPSourceCodeClassFieldDeclarationBlock((CPPCodeClassField*)cf);
}

void CPPSourceCodeDocument::resetTextBlocks()
{

        // all special pointers need to be zero'd out.
	methodsBlock = 0;
	constructorBlock = 0;

        // now do the traditional release of child text blocks
        ClassifierCodeDocument::resetTextBlocks();

}

// This method will cause the class to rebuild its text representation.
// based on the parent classifier object.
// For any situation in which this is called, we are either building the code
// document up, or replacing/regenerating the existing auto-generated parts. As
// such, we will want to insert everything we resonablely will want
// during creation. We can set various parts of the document (esp. the
// comments) to appear or not, as needed.
void CPPSourceCodeDocument::updateContent( ) 
{

      	// Gather info on the various fields and parent objects of this class...
	UMLClassifier * c = getParentClassifier();
	CPPCodeGenerator * gen = (CPPCodeGenerator*) getParentGenerator();
	CPPCodeGenerationPolicy * policy = (CPPCodeGenerationPolicy*) getParentGenerator()->getPolicy();
        QString endLine = gen->getNewLineEndingChars(); // a shortcut..so we dont have to call this all the time

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

	// START GENERATING CODE/TEXT BLOCKS and COMMENTS FOR THE DOCUMENT
	//

       // INCLUDE CODEBLOCK
        //
        // Q: Why all utils? Isnt just List and Vector the only classes we are using?
        // A: doesn't matter at all; its more readable to just include '*' and cpp compilers
        //    don't slow down or anything. (TZ)
        QString includeStatement = "";
        includeStatement.append("#include \""+policy->getStringClassNameInclude()+"\""+endLine);
        if ( hasObjectVectorClassFields() )
                includeStatement.append("#include "+policy->getVectorClassNameInclude()+"\""+endLine);

        //only include classes in a different package from this class
        UMLClassifierList includes;
        QMap<UMLClassifier *,QString> *packageMap = new QMap<UMLClassifier*,QString>; // so we dont repeat packages

        gen->findObjectsRelated(c,includes);
        for(UMLClassifier *con = includes.first(); con ; con = includes.next())
        if (!(packageMap->contains(con)))
        {
                packageMap->insert(con,con->getPackage());
                includeStatement.append("#include \""+gen->cleanName(con->getName().lower())+".h\""+endLine);
        }
        // now, add/update the includes codeblock
        CodeBlockWithComments * iblock = addOrUpdateTaggedCodeBlockWithComments("includes", includeStatement, QString::null, 0, false);
	if(includeStatement.isEmpty() && iblock->getContentType() == CodeBlock::AutoGenerated)
		iblock->setWriteOutText(false);
	else
		iblock->setWriteOutText(true);

	// After the includes we have just 2 big blocks basically, the "constructor" block and the
	// block for the rest of our methods (operations + accessors)

        constructorBlock = getHierarchicalCodeBlock("constructionMethodsBlock", "Constructors/Destructors", 0);
        methodsBlock = getHierarchicalCodeBlock("otherMethodsBlock", "Methods", 0);
        
	// add accessors to the methods block
        methodsBlock->addCodeClassFieldMethods(staticAttribClassFields);
        methodsBlock->addCodeClassFieldMethods(attribClassFields);
        methodsBlock->addCodeClassFieldMethods(plainAssocClassFields);
        methodsBlock->addCodeClassFieldMethods(aggregationClassFields);
        methodsBlock->addCodeClassFieldMethods(compositionClassFields);

	// constuctors and other operations are handled by the "addCodeOperation" method above.

}


#include "cppsourcecodedocument.moc"
