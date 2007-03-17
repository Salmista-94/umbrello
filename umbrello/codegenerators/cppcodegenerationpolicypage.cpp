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
 *      Date   : Wed Jul 30 2003
 */

// own header
#include "cppcodegenerationpolicypage.h"
// qt/kde includes
#include <kdebug.h>
#include <klocale.h>
#include <qlabel.h>
#include <kcombobox.h>
#include <qcheckbox.h>
// app includes
#include "cppcodegenerationformbase.h"
#include "cppcodegenerator.h"
//#include "../codegenerationpolicy.h"
#include "../uml.h"

CPPCodeGenerationPolicyPage::CPPCodeGenerationPolicyPage( QWidget *parent, const char *name, CPPCodeGenerationPolicy * policy )
        : CodeGenerationPolicyPage(parent, name, UMLApp::app()->getCommonPolicy())
{
    CodeGenerationPolicy *common = UMLApp::app()->getCommonPolicy();
    form = new CPPCodeGenerationForm(this);
    form->m_SelectCommentStyle->setCurrentItem((int)(common->getCommentStyle()));
    form->setPackageIsANamespace(policy->getPackageIsNamespace());
    form->setVirtualDestructors(policy->getDestructorsAreVirtual());
    form->setGenerateAccessorMethods(policy->getAutoGenerateAccessors());
    form->setGenerateEmptyConstructors(common->getAutoGenerateConstructors());
    form->setOperationsAreInline(policy->getOperationsAreInline());
    form->setAccessorsAreInline(policy->getAccessorsAreInline());
    form->setAccessorsArePublic(policy->getAccessorsArePublic());

    form->m_stringClassHCombo->setCurrentItem(policy->getStringClassName(),true);
    form->m_listClassHCombo->setCurrentItem(policy->getVectorClassName(),true);

    form->m_stringIncludeFileHistoryCombo->setCurrentItem(policy->getStringClassNameInclude(),true);
    form->m_listIncludeFileHistoryCombo->setCurrentItem(policy->getVectorClassNameInclude(),true);

    form->m_globalStringCheckBox->setChecked(policy->stringIncludeIsGlobal());
    form->m_globalListCheckBox->setChecked(policy->vectorIncludeIsGlobal());
}

CPPCodeGenerationPolicyPage::~CPPCodeGenerationPolicyPage()
{
}

void CPPCodeGenerationPolicyPage::apply()
{
    CodeGenerationPolicy *common = UMLApp::app()->getCommonPolicy();

    // now do our cpp-specific configs
    CPPCodeGenerationPolicy * parent = (CPPCodeGenerationPolicy*) m_parentPolicy;

    // block signals so that we don't generate too many sync signals for child code
    // documents
    parent->blockSignals(true);

    common->setCommentStyle((CodeGenerationPolicy::CommentStyle ) form->m_SelectCommentStyle->currentItem());
    common->setAutoGenerateConstructors(form->getGenerateEmptyConstructors());
    parent->setAutoGenerateAccessors(form->getGenerateAccessorMethods());

    parent->setDestructorsAreVirtual(form->getVirtualDestructors());
    parent->setPackageIsNamespace(form->getPackageIsANamespace());
    parent->setAccessorsAreInline(form->getAccessorsAreInline());
    parent->setOperationsAreInline(form->getOperationsAreInline());
    parent->setAccessorsArePublic(form->getAccessorsArePublic());

    parent->setStringClassName(form->m_stringClassHCombo->currentText());
    parent->setStringClassNameInclude(form->m_stringIncludeFileHistoryCombo->currentText());
    parent->setStringIncludeIsGlobal(form->m_globalStringCheckBox->isChecked());

    parent->setVectorClassName(form->m_listClassHCombo->currentText());
    parent->setVectorClassNameInclude(form->m_listIncludeFileHistoryCombo->currentText());
    parent->setVectorIncludeIsGlobal(form->m_globalListCheckBox->isChecked());

    parent->blockSignals(false);

    // now send out modified code content signal
    common->emitModifiedCodeContentSig();

}


#include "cppcodegenerationpolicypage.moc"
