/***************************************************************************
                          idlwriter.h  -  description
                             -------------------
    begin                : Sat Jan 4 2003
    copyright            : (C) 2003 by Oliver Kellogg
    email                : okellogg@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef IDLWRITER_H
#define IDLWRITER_H

#include "simplecodegenerator.h"
#include <qtextstream.h>

class UMLAssociation;
class UMLOperation;

/**
 * Class IDLWriter is a code generator for UMLClassifier objects.
 * Create an instance of this class, and feed it a UMLClassifier when
 * calling writeClass and it will generate an IDL interface for that
 * concept.
 */
class IDLWriter : public SimpleCodeGenerator {
public:

	IDLWriter (UMLDoc * parent, const char* name = 0);
	virtual ~IDLWriter ();

	/**
	 * call this method to generate IDL code for a UMLClassifier
	 * @param c the class to generate code for
	 */
	virtual void writeClass (UMLClassifier *c);

	virtual bool isType (QString & type);
        virtual QString getLanguage();

	void createDefaultDatatypes();

private:

	/**
	 * write one operation
	 * @param op the class for which we are generating code
	 * @param idl the stream associated with the output file
	 */
	void writeOperation(UMLOperation* op, QTextStream& idl, bool is_comment = false);

	void computeAssocTypeAndRole(UMLAssociation* a, UMLClassifier *c,
				     QString& typeName, QString& roleName);

	bool isOOClass(UMLClassifier* c);

	QString qualifiedName(UMLClassifier* c);

	QString spc();

	int indentlevel;
};

#endif // IDLWRITER_H
