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
 *      Date   : Mon Jun 23 2003
 */

// own header
#include "cppcodegenerationpolicy.h"
// qt/kde includes
#include <qregexp.h>
#include <kdebug.h>
#include <kconfig.h>
// app includes
#include "cppcodegenerationpolicypage.h"
#include "../uml.h"

const bool CPPCodeGenerationPolicy::DEFAULT_AUTO_GEN_ACCESSORS = true;
const bool CPPCodeGenerationPolicy::DEFAULT_INLINE_ACCESSORS = false;
const bool CPPCodeGenerationPolicy::DEFAULT_INLINE_OPERATIONS = false;
const bool CPPCodeGenerationPolicy::DEFAULT_VIRTUAL_DESTRUCTORS = true;
const bool CPPCodeGenerationPolicy::DEFAULT_PACKAGE_IS_NAMESPACE = false;
const char * CPPCodeGenerationPolicy::DEFAULT_STRING_CLASS_NAME = "string";
const char * CPPCodeGenerationPolicy::DEFAULT_STRING_CLASS_INCLUDE = "string";
const char * CPPCodeGenerationPolicy::DEFAULT_VECTOR_CLASS_NAME = "vector";
const char * CPPCodeGenerationPolicy::DEFAULT_VECTOR_CLASS_INCLUDE = "vector";
const char * CPPCodeGenerationPolicy::DEFAULT_VECTOR_METHOD_APPEND = "%VARNAME%.push_back(value);";
const char * CPPCodeGenerationPolicy::DEFAULT_VECTOR_METHOD_REMOVE = "int size = %VARNAME%.size();\nfor ( int i = 0; i < size; ++i) {\n\t%ITEMCLASS% item = %VARNAME%.at(i);\n\tif(item == value) {\n\t\tvector<%ITEMCLASS%>::iterator it = %VARNAME%.begin() + i;\n\t\t%VARNAME%.erase(it);\n\t\treturn;\n\t}\n }";
const char * CPPCodeGenerationPolicy::DEFAULT_VECTOR_METHOD_INIT = ""; // nothing to do in std::vector
const char * CPPCodeGenerationPolicy::DEFAULT_OBJECT_METHOD_INIT = "%VARNAME% = new %ITEMCLASS%( );";
const bool CPPCodeGenerationPolicy::DEFAULT_STRING_INCLUDE_GLOBAL = true;
const bool CPPCodeGenerationPolicy::DEFAULT_VECTOR_INCLUDE_GLOBAL = true;
const bool CPPCodeGenerationPolicy::DEFAULT_PUBLIC_ACCESSORS = false;


// Constructors/Destructors
//

CPPCodeGenerationPolicy::CPPCodeGenerationPolicy(KConfig *config)
{
    init();
    setDefaults(config,false);
}

CPPCodeGenerationPolicy::~CPPCodeGenerationPolicy ( ) { }

//
// Methods
//

// Accessor methods
//
/**
 * Set the value of m_publicAccessors
 * @param new_var the new value
 */
void CPPCodeGenerationPolicy::setAccessorsArePublic ( bool var )
{
    m_publicAccessors = var;
    // @todo we should probably use an own signal for this
    UMLApp::app()->getCommonPolicy()->emitModifiedCodeContentSig();
}

/**
 * Get the value of m_publicAccessors
 * @return value the boolean value of m_publicAccessors
 */
bool CPPCodeGenerationPolicy::getAccessorsArePublic( )
{
    return m_publicAccessors;
}

/**
 * Set the value of m_inlineAccessors
 * @param new_var the new value
 */
void CPPCodeGenerationPolicy::setAccessorsAreInline ( bool var )
{
    m_inlineAccessors = var;
    UMLApp::app()->getCommonPolicy()->emitModifiedCodeContentSig();
}

/**
 * Get the value of m_inlineAccessors
 * @return value the boolean value of m_inlineAccessors
 */
bool CPPCodeGenerationPolicy::getAccessorsAreInline( )
{
    return m_inlineAccessors;
}

/**
 * Set the value of m_inlineOperations
 * @param new_var the new value
 */
void CPPCodeGenerationPolicy::setOperationsAreInline ( bool var )
{
    m_inlineOperations = var;
    UMLApp::app()->getCommonPolicy()->emitModifiedCodeContentSig();
}

/**
 * Get the value of m_inlineOperations
 * @return value the boolean value of m_inlineOperations
 */
bool CPPCodeGenerationPolicy::getOperationsAreInline( )
{
    return m_inlineOperations;
}

/**
 * Set the value of m_virtualDestructors
 * @param new_var the new value
 */
void CPPCodeGenerationPolicy::setDestructorsAreVirtual ( bool var )
{
    m_virtualDestructors = var;
    UMLApp::app()->getCommonPolicy()->emitModifiedCodeContentSig();
}

/**
 * Get the value of m_virtualDestructors
 * @return value the boolean value of m_virtualDestructors
 */
bool CPPCodeGenerationPolicy::getDestructorsAreVirtual( )
{
    return m_virtualDestructors;
}

/**
 * Set the value of m_packageIsNamespace
 * @param new_var the new value
 */
void CPPCodeGenerationPolicy::setPackageIsNamespace ( bool var ) {
    m_packageIsNamespace = var;
    UMLApp::app()->getCommonPolicy()->emitModifiedCodeContentSig();
}

/**
 * Get the value of m_packageIsNamespace
 * @return value the boolean value of m_packageIsNamespace
 */
bool CPPCodeGenerationPolicy::getPackageIsNamespace( ) {
    return m_packageIsNamespace;
}

/**
 * Set the value of m_autoGenerateAccessors
 * @param new_var the new value
 */
void CPPCodeGenerationPolicy::setAutoGenerateAccessors( bool var ) {
    m_autoGenerateAccessors = var;
    UMLApp::app()->getCommonPolicy()->emitModifiedCodeContentSig();
}

/**
 * Get the value of m_autoGenerateConstructors
 * @return the value of m_autoGenerateConstructors
 */
bool CPPCodeGenerationPolicy::getAutoGenerateAccessors( ){
    return m_autoGenerateAccessors;
}

QString CPPCodeGenerationPolicy::getStringClassName() {
    return m_stringClassName;
}

QString CPPCodeGenerationPolicy::getStringClassNameInclude() {
    return m_stringClassNameInclude;
}

QString CPPCodeGenerationPolicy::getVectorClassName() {
    return m_vectorClassName;
}

QString CPPCodeGenerationPolicy::getVectorClassNameInclude() {
    return m_vectorClassNameInclude;
}

void CPPCodeGenerationPolicy::setStringClassName(const QString &value) {
    m_stringClassName = value;
    UMLApp::app()->getCommonPolicy()->emitModifiedCodeContentSig();
}

void CPPCodeGenerationPolicy::setStringClassNameInclude(const QString &value) {
    m_stringClassNameInclude = value;
    UMLApp::app()->getCommonPolicy()->emitModifiedCodeContentSig();
}

void CPPCodeGenerationPolicy::setVectorClassName(const QString &value) {
    m_vectorClassName = value;
    UMLApp::app()->getCommonPolicy()->emitModifiedCodeContentSig();
}

void CPPCodeGenerationPolicy::setVectorClassNameInclude(const QString &value) {
    m_vectorClassNameInclude = value;
    UMLApp::app()->getCommonPolicy()->emitModifiedCodeContentSig();
}

/** determine if the string include is global one */
bool CPPCodeGenerationPolicy::stringIncludeIsGlobal () {
    return m_stringIncludeIsGlobal;
}

bool CPPCodeGenerationPolicy::vectorIncludeIsGlobal () {
    return m_vectorIncludeIsGlobal;
}

void CPPCodeGenerationPolicy::setStringIncludeIsGlobal(bool value) {
    m_stringIncludeIsGlobal = value;
    UMLApp::app()->getCommonPolicy()->emitModifiedCodeContentSig();
}

void CPPCodeGenerationPolicy::setVectorIncludeIsGlobal(bool value) {
    m_vectorIncludeIsGlobal = value;
    UMLApp::app()->getCommonPolicy()->emitModifiedCodeContentSig();
}

QString CPPCodeGenerationPolicy::getVectorMethodAppend(const QString & variableName, const QString & itemClassName) {
    QString value = m_vectorMethodAppendBase;
    if (!variableName.isEmpty())
        value.replace(QRegExp("%VARNAME%"),variableName);
    value.replace(QRegExp("%VECTORTYPENAME%"), m_vectorClassName);
    if (!itemClassName.isEmpty())
        value.replace(QRegExp("%ITEMCLASS%"),itemClassName);
    return value;
}

QString CPPCodeGenerationPolicy::getVectorMethodRemove(const QString & variableName, const QString & itemClassName) {
    QString value = m_vectorMethodRemoveBase;
    if (!variableName.isEmpty())
        value.replace(QRegExp("%VARNAME%"),variableName);
    value.replace(QRegExp("%VECTORTYPENAME%"), m_vectorClassName);
    if (!itemClassName.isEmpty())
        value.replace(QRegExp("%ITEMCLASS%"),itemClassName);
    return value;
}

QString CPPCodeGenerationPolicy::getVectorMethodInit(const QString & variableName, const QString & itemClassName) {
    QString value = m_vectorMethodInitBase;
    if (!variableName.isEmpty())
        value.replace(QRegExp("%VARNAME%"),variableName);
    value.replace(QRegExp("%VECTORTYPENAME%"), m_vectorClassName);
    if (!itemClassName.isEmpty())
        value.replace(QRegExp("%ITEMCLASS%"),itemClassName);
    return value;
}

QString CPPCodeGenerationPolicy::getObjectMethodInit(const QString & variableName, const QString & itemClassName) {
    QString value = m_objectMethodInitBase;
    if (!variableName.isEmpty())
        value.replace(QRegExp("%VARNAME%"),variableName);
    value.replace(QRegExp("%VECTORTYPENAME%"), m_vectorClassName);
    if (!itemClassName.isEmpty())
        value.replace(QRegExp("%ITEMCLASS%"),itemClassName);
    return value;
}

// Other methods
//

void CPPCodeGenerationPolicy::writeConfig ( KConfig * config )
{

    // write ONLY the CPP specific stuff
    config->setGroup("CPP Code Generation");

    config->writeEntry("autoGenAccessors",getAutoGenerateAccessors());

    config->writeEntry("inlineAccessors",getAccessorsAreInline());
    config->writeEntry("publicAccessors",getAccessorsArePublic());
    config->writeEntry("inlineOps",getOperationsAreInline());
    config->writeEntry("virtualDestructors",getDestructorsAreVirtual());
    config->writeEntry("packageIsNamespace",getPackageIsNamespace());

    config->writeEntry("stringClassName",getStringClassName());
    config->writeEntry("stringClassNameInclude",getStringClassNameInclude());
    config->writeEntry("stringIncludeIsGlobal",stringIncludeIsGlobal());

    config->writeEntry("vectorClassName",getVectorClassName());
    config->writeEntry("vectorClassNameInclude",getVectorClassNameInclude());
    config->writeEntry("vectorIncludeIsGlobal",vectorIncludeIsGlobal());

}

void CPPCodeGenerationPolicy::setDefaults ( CPPCodeGenerationPolicy * cppclone, bool emitUpdateSignal )
{
    blockSignals(true); // we need to do this because otherwise most of these
    // settors below will each send the modifiedCodeContent() signal
    // needlessly (we can just make one call at the end).

    {
        setAutoGenerateAccessors(cppclone->getAutoGenerateAccessors());

        setAccessorsAreInline(cppclone->getAccessorsAreInline());
        setOperationsAreInline(cppclone->getOperationsAreInline());
        setDestructorsAreVirtual(cppclone->getDestructorsAreVirtual());
        setPackageIsNamespace(cppclone->getPackageIsNamespace());

        setStringClassName(cppclone->getStringClassName() );
        setStringClassNameInclude(cppclone->getStringClassNameInclude());
        setStringIncludeIsGlobal(cppclone->stringIncludeIsGlobal());

        setVectorClassName(cppclone->getVectorClassName());
        setVectorClassNameInclude(cppclone->getVectorClassNameInclude());
        setVectorIncludeIsGlobal(cppclone->vectorIncludeIsGlobal());

    }

    blockSignals(false); // "as you were citizen"

    if (emitUpdateSignal)
        UMLApp::app()->getCommonPolicy()->emitModifiedCodeContentSig();

}

void CPPCodeGenerationPolicy::setDefaults( KConfig * config, bool emitUpdateSignal )
{

    if (!config)
        return;

    blockSignals(true); // we need to do this because otherwise most of these
    // settors below will each send the modifiedCodeContent() signal
    // needlessly (we can just make one call at the end).

    // now do cpp specific stuff
    config -> setGroup("CPP Code Generation");

    setAutoGenerateAccessors(config->readEntry("autoGenAccessors",DEFAULT_AUTO_GEN_ACCESSORS));

    setAccessorsAreInline(config->readEntry("inlineAccessors",DEFAULT_INLINE_ACCESSORS));
    setAccessorsArePublic(config->readEntry("publicAccessors",DEFAULT_PUBLIC_ACCESSORS));
    setOperationsAreInline(config->readEntry("inlineOps",DEFAULT_INLINE_OPERATIONS));
    setDestructorsAreVirtual(config->readEntry("virtualDestructors",DEFAULT_VIRTUAL_DESTRUCTORS));
    setPackageIsNamespace(config->readEntry("packageIsNamespace",DEFAULT_PACKAGE_IS_NAMESPACE));

    setStringClassName(config->readEntry("stringClassName",DEFAULT_STRING_CLASS_NAME) );
    setStringClassNameInclude(config->readEntry("stringClassNameInclude",DEFAULT_STRING_CLASS_INCLUDE ) );
    setStringIncludeIsGlobal(config->readEntry("stringIncludeIsGlobal",DEFAULT_STRING_INCLUDE_GLOBAL) );

    setVectorClassName(config->readEntry("vectorClassName",DEFAULT_VECTOR_CLASS_NAME) );
    setVectorClassNameInclude(config->readEntry("vectorClassNameInclude",DEFAULT_VECTOR_CLASS_INCLUDE) );
    setVectorIncludeIsGlobal(config->readEntry("vectorIncludeIsGlobal",DEFAULT_VECTOR_INCLUDE_GLOBAL) );

    blockSignals(false); // "as you were citizen"

    if (emitUpdateSignal)
        UMLApp::app()->getCommonPolicy()->emitModifiedCodeContentSig();
}


/**
 * Create a new dialog interface for this object.
 * @return dialog object
 */
CodeGenerationPolicyPage * CPPCodeGenerationPolicy::createPage ( QWidget *parent, const char *name ) {
    return new CPPCodeGenerationPolicyPage ( parent, name, this );
}

void CPPCodeGenerationPolicy::init() {

    m_inlineAccessors = DEFAULT_INLINE_ACCESSORS;
    m_publicAccessors = DEFAULT_PUBLIC_ACCESSORS;
    m_inlineOperations = DEFAULT_INLINE_OPERATIONS;
    m_virtualDestructors = DEFAULT_VIRTUAL_DESTRUCTORS;
    m_packageIsNamespace = DEFAULT_PACKAGE_IS_NAMESPACE;

    m_stringClassName = DEFAULT_STRING_CLASS_NAME;
    m_stringClassNameInclude = DEFAULT_STRING_CLASS_INCLUDE;
    m_stringIncludeIsGlobal = DEFAULT_STRING_INCLUDE_GLOBAL;

    m_vectorClassName = DEFAULT_VECTOR_CLASS_NAME;
    m_vectorClassNameInclude = DEFAULT_VECTOR_CLASS_INCLUDE;
    m_vectorIncludeIsGlobal = DEFAULT_VECTOR_INCLUDE_GLOBAL;

    m_vectorMethodAppendBase = DEFAULT_VECTOR_METHOD_APPEND;
    m_vectorMethodRemoveBase = DEFAULT_VECTOR_METHOD_REMOVE;
    m_vectorMethodInitBase = DEFAULT_VECTOR_METHOD_INIT;
    m_objectMethodInitBase = DEFAULT_OBJECT_METHOD_INIT;

}


#include "cppcodegenerationpolicy.moc"
