
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
 *      Date   : Mon Sep 1 2003
 */

#include "cppsourcecodeoperation.h"

#include "cppcodegenerator.h"
#include "cppsourcecodedocument.h"
#include "cppcodedocumentation.h"

// Constructors/Destructors
//

CPPSourceCodeOperation::CPPSourceCodeOperation ( CPPSourceCodeDocument * doc, UMLOperation *parent, const QString & body, const QString & comment )
    : CodeOperation ((ClassifierCodeDocument*)doc, parent, body, comment)
{
	init(doc);
}

CPPSourceCodeOperation::~CPPSourceCodeOperation ( ) { }

// Other methods
//

// we basically just want to know whether or not to print out
// the body of the operation.
// In C++ if the operations are inline, then we DONT print out
// the body text.
void CPPSourceCodeOperation::updateContent( )
{
        CPPCodeGenerationPolicy * policy = (CPPCodeGenerationPolicy*) getParentDocument()->getParentGenerator()->getPolicy();
        bool isInlineMethod = policy->getOperationsAreInline();

        if(!isInlineMethod)
                setText(""); // change whatever it is to "";

}

// we basically want to update the doc and start text of this method
void CPPSourceCodeOperation::updateMethodDeclaration()
{

	CPPSourceCodeDocument * doc = (CPPSourceCodeDocument*) getParentDocument();
	CPPCodeGenerator * gen = (CPPCodeGenerator*) getParentDocument()->getParentGenerator();
        CPPCodeGenerationPolicy * policy = (CPPCodeGenerationPolicy*) gen->getPolicy();
	UMLClassifier * c = doc->getParentClassifier();
	UMLOperation * o = getParentOperation();
	bool isInterface = doc->parentIsInterface();
        bool isInlineMethod = policy->getAccessorsAreInline( );

	// first, the comment on the operation
	QString comment = o->getDoc();
	getComment()->setText(comment);

	// no return type for constructors
	QString returnType = o->isConstructorOperation() ? QString("") : o->getTypeName();
	QString methodName = o->getName();
	QString paramStr = QString("");
	QString className = gen->getCPPClassName(c->getName());

	// assemble parameters
        UMLAttributeList * list = getParentOperation()->getParmList();
	int nrofParam = list->count();
	int paramNum = 0;
	for(UMLAttribute* parm = list->first(); parm; parm=list->next())
	{
		QString rType = parm->getTypeName();
		QString paramName = parm->getName();
		paramStr += rType + " " + paramName;
		paramNum++;

		if (paramNum != nrofParam )
			paramStr  += ", ";
	}

	QString startText = returnType + " " + className + "::" + methodName + " ("+paramStr+") {";
	setStartMethodText(startText);

	// Only write this out if its a child of an interface OR is abstract.
	// and its not inline
	if(isInterface || o->getAbstract() || isInlineMethod)
	{
		setWriteOutText(false);
	} else {
		setWriteOutText(true);
	}

}

void CPPSourceCodeOperation::init (CPPSourceCodeDocument * doc )
{

	// lets not go with the default comment and instead use
	// full-blown cpp documentation object instead
        setComment(new CPPCodeDocumentation(doc));

	// these things never change..
        setOverallIndentationLevel(0);
	setEndMethodText("}");

        updateMethodDeclaration();
        updateContent();

}

#include "cppsourcecodeoperation.moc"
