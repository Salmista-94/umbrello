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
 *      Date   : Wed Jul 30 2003
 */

#include <kdebug.h>
#include <klocale.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include "javacodegenerationpolicypage.h"
#include "javacodegenerationformbase.h"

JavaCodeGenerationPolicyPage::JavaCodeGenerationPolicyPage( QWidget *parent, const char *name, JavaCodeGenerationPolicy * policy )
	:CodeGenerationPolicyPage(parent,name,(CodeGenerationPolicy*)policy) 
{
	form = new JavaCodeGenerationFormBase(this);
	form->m_SelectCommentStyle->setCurrentItem(commentTypeToInteger(policy->getCommentStyle()));
	form->m_generateConstructors->setChecked(policy->getAutoGenerateConstructors());
	form->m_generateAccessors->setChecked(policy->getAutoGenerateAccessors());
    	form->m_makeANTDocumentCheckBox->setChecked(policy->getBuildANTCodeDocument());

}

JavaCodeGenerationPolicyPage::~JavaCodeGenerationPolicyPage()
{
}

int JavaCodeGenerationPolicyPage::commentTypeToInteger(JavaCodeGenerationPolicy::JavaCommentStyle type) {
      switch (type) {
                case JavaCodeGenerationPolicy::DoubleSlash:
                        return 1;
                default:
                case JavaCodeGenerationPolicy::SlashStar:
                        return 0;
        }
}

void JavaCodeGenerationPolicyPage::apply()
{

	// now do our java-specific configs
	JavaCodeGenerationPolicy * parent = (JavaCodeGenerationPolicy*) m_parentPolicy;

kdDebug()<<"Apply in JavaCodeGenerationPage called (parent:"<<parent<<")"<<endl;

	parent->setCommentStyle((JavaCodeGenerationPolicy::JavaCommentStyle ) form->m_SelectCommentStyle->currentItem());
	parent->setAutoGenerateConstructors(form->m_generateConstructors->isChecked());
	parent->setAutoGenerateAccessors(form->m_generateAccessors->isChecked());
    	parent->setBuildANTCodeDocument(form->m_makeANTDocumentCheckBox->isChecked());

}

