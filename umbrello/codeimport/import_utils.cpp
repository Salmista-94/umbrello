/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2005-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

// own header
#include "import_utils.h"

// app includes
#include "association.h"
#include "artifact.h"
#include "attribute.h"
#include "classifier.h"
#include "debug_utils.h"
#include "folder.h"
#include "enum.h"
#include "enumliteral.h"
#include "messagewidget.h"
#include "model_utils.h"
#include "object_factory.h"
#include "objectwidget.h"
#include "operation.h"
#include "package.h"
#include "template.h"
#include "uml.h"
#include "umldoc.h"
#include "umllistview.h"
#include "umlobject.h"
#include "umbrellosettings.h"
#include "widget_factory.h"

// kde includes
#include <KMessageBox>
#include <KLocalizedString>

// qt includes
#include <QMap>
#include <QRegExp>

#ifdef Q_OS_WIN
#define PATH_SEPARATOR QLatin1Char(';')
#else
#define PATH_SEPARATOR QLatin1Char(':')
#endif

DEBUG_REGISTER_DISABLED(Import_Utils)
#undef DBG_SRC
#define DBG_SRC QLatin1String("Import_Utils")

namespace Import_Utils {

/**
 * Flag manipulated by createUMLObject().
 * Global state is generally bad, I know.
 * It would be cleaner to make this into a return value from
 * createUMLObject().
 */
bool bNewUMLObjectWasCreated = false;

/**
 * Related classifier for creation of dependencies on template
 * parameters in createUMLObject().
 */
UMLClassifier * gRelatedClassifier = NULL;

/**
 * On encountering a scoped typename string where the scopes
 * have not yet been seen, we synthesize UML objects for the
 * unknown scopes (using a question dialog to the user to decide
 * whether to treat a scope as a class or as a package.)
 * However, such an unknown scope is put at the global level.
 * I.e. before calling createUMLObject() we set this flag to true.
 */
bool bPutAtGlobalScope = false;

/**
 * The include path list (see addIncludePath() and includePathList())
 */
QStringList incPathList;

/**
 * Control whether an object which is newly created by createUMLObject()
 * is put at the global scope.
 *
 * @param yesno  When set to false, the object is created at the scope
 *               given by the parentPkg argument of createUMLObject().
 */
void putAtGlobalScope(bool yesno)
{
    bPutAtGlobalScope = yesno;
}

/**
 * Set a related classifier for creation of dependencies on template
 * parameters in createUMLObject().
 */
void setRelatedClassifier(UMLClassifier *c)
{
    gRelatedClassifier = c;
}

/**
 * Control whether the creation methods solicit a new unique ID for the
 * created object.
 * By default, unique ID generation is turned on.
 *
 * @param yesno   False turns UID generation off, true turns it on.
 */
void assignUniqueIdOnCreation(bool yesno)
{
    Object_Factory::assignUniqueIdOnCreation(yesno);
}

/**
 * Returns whether the last createUMLObject() actually created
 * a new object or just returned an existing one.
 */
bool newUMLObjectWasCreated()
{
    return bNewUMLObjectWasCreated;
}

/**
 * Strip comment lines of leading whitespace and stars.
 */
QString formatComment(const QString &comment)
{
    if (comment.isEmpty())
        return comment;

    QStringList lines = comment.split(QLatin1Char('\n'));
    QString& first = lines.first();
    QRegExp wordex(QLatin1String("\\w"));
    if (first.startsWith(QLatin1String(QLatin1String("/*")))) {
        int wordpos = wordex.indexIn(first);
        if (wordpos != -1)
            first = first.mid(wordpos);  // remove comment start
        else
            lines.pop_front();  // nothing interesting on this line
    }
    if (! lines.count())
        return QString();

    QString& last = lines.last();
    int endpos = last.indexOf(QLatin1String("*/"));
    if (endpos != -1) {
        if (last.contains(wordex))
            last = last.mid(0, endpos - 1);  // remove comment end
        else
            lines.pop_back();  // nothing interesting on this line
    }
    if (! lines.count())
        return QString();

    QStringList::Iterator end(lines.end());
    for (QStringList::Iterator lit(lines.begin()); lit != end; ++lit) {
        (*lit).remove(QRegExp(QLatin1String("^\\s+")));
        (*lit).remove(QRegExp(QLatin1String("^\\*+\\s?")));
    }
    return lines.join(QLatin1String("\n"));
}

/*
UMLObject* findUMLObject(QString name, UMLObject::ObjectType type)
{
    // Why an extra wrapper? See comment at addMethodParameter()
    UMLObject * o = umldoc->findUMLObject(name, type);
    return o;
}
 */

/**
 * Find or create a document object.
 */
UMLObject *createUMLObject(UMLObject::ObjectType type,
                           const QString& inName,
                           UMLPackage *parentPkg,
                           const QString& comment,
                           const QString& stereotype,
                           bool searchInParentPackageOnly)
{
    QString name = inName;
    UMLDoc *umldoc = UMLApp::app()->document();
    UMLFolder *logicalView = umldoc->rootFolder(Uml::ModelType::Logical);
    if (parentPkg == NULL) {
        // DEBUG(DBG_SRC) << "Import_Utils::createUMLObject(" << name
        //     << "): parentPkg is NULL, assuming Logical View";
        parentPkg = logicalView;
    } else if (parentPkg->baseType() == UMLObject::ot_Artifact) {
        DEBUG(DBG_SRC) << "Import_Utils::createUMLObject(" << name
                       << "): Artifact as parent package is not supported yet, using Logical View";
        parentPkg = logicalView;
    } else if (parentPkg->baseType() == UMLObject::ot_Association) {
        DEBUG(DBG_SRC) << "Import_Utils::createUMLObject(" << name
                       << "): Association as parent package is not supported yet, using Logical View";
        parentPkg = logicalView;
    } else if (name.startsWith(QLatin1String("::"))) {
        name = name.mid(2);
        parentPkg = logicalView;
    }
    UMLObject *o = umldoc->findUMLObject(name, type, parentPkg);
    bNewUMLObjectWasCreated = false;
    if (searchInParentPackageOnly) {
        if (o && o->umlPackage() != parentPkg) {
            o = Object_Factory::createNewUMLObject(type, name, parentPkg);
            bNewUMLObjectWasCreated = true;
            bPutAtGlobalScope = false;
        }
    }
    if (o == NULL) {
        // Strip possible adornments and look again.
        const bool isConst = name.contains(QRegExp(QLatin1String("^const ")));
        name.remove(QRegExp(QLatin1String("^const\\s+")));
        QString typeName(name);
        bool isAdorned = typeName.contains(QRegExp(QLatin1String("[^\\w:\\. ]")));
        const bool isPointer = typeName.contains(QLatin1Char('*'));
        const bool isRef = typeName.contains(QLatin1Char('&'));
        typeName.remove(QRegExp(QLatin1String("[^\\w:\\. ].*$")));
        typeName = typeName.simplified();
        UMLObject *origType = umldoc->findUMLObject(typeName, UMLObject::ot_UMLObject, parentPkg);
        if (origType == NULL) {
            // Still not found. Create the stripped down type.
            if (bPutAtGlobalScope)
                parentPkg = logicalView;
            // Find, or create, the scopes.
            QStringList components;
            if (typeName.contains(QLatin1String("::"))) {
                components = typeName.split(QLatin1String("::"), QString::SkipEmptyParts);
            } else if (typeName.contains(QLatin1String("..."))) {
                // Java variable length arguments
                type = UMLObject::ot_Datatype;
                parentPkg = umldoc->datatypeFolder();
                isAdorned = false;
            } else if (typeName.contains(QLatin1String("."))) {
                components = typeName.split(QLatin1Char('.'));
            }
            if (components.count() > 1) {
                typeName = components.back();
                components.pop_back();
                while (components.count()) {
                    QString scopeName = components.front();
                    components.pop_front();
                    o = umldoc->findUMLObject(scopeName, UMLObject::ot_UMLObject, parentPkg);
                    if (o) {
                        parentPkg = static_cast<UMLPackage*>(o);
                        continue;
                    }
                    o = Object_Factory::createUMLObject(UMLObject::ot_Class, scopeName, parentPkg);
                    o->setStereotypeCmd(QLatin1String("class-or-package"));
                    UMLListViewItem *item = UMLApp::app()->listView()->findUMLObject(o);
                    if (item)
                        item->updateObject();
                    parentPkg = static_cast<UMLPackage*>(o);
                    Model_Utils::treeViewSetCurrentItem(o);
                }
                // All scope qualified datatypes live in the global scope.
                bPutAtGlobalScope = true;
            }
            UMLObject::ObjectType t = type;
            if (type == UMLObject::ot_UMLObject || isAdorned)
                t = UMLObject::ot_Class;
            origType = Object_Factory::createUMLObject(t, typeName, parentPkg, false);
            bNewUMLObjectWasCreated = true;
            bPutAtGlobalScope = false;
        }
        if (isConst || isAdorned) {
            // Create the full given type (including adornments.)
            if (isConst)
                name.prepend(QLatin1String("const "));
            o = Object_Factory::createUMLObject(UMLObject::ot_Datatype, name,
                                                umldoc->datatypeFolder(),
                                                false); //solicitNewName
            UMLClassifier *dt = static_cast<UMLClassifier*>(o);
            UMLClassifier *c = dynamic_cast<UMLClassifier*>(origType);
            if (c)
                dt->setOriginType(c);
            else
                uError() << "createUMLObject(" << name << "): "
                    << "origType " << typeName << " is not a UMLClassifier";
            if (isRef || isPointer)
                dt->setIsReference();
            /*
            if (isPointer) {
                UMLObject *pointerDecl = Object_Factory::createUMLObject(UMLObject::ot_Datatype, type);
                UMLClassifier *dt = static_cast<UMLClassifier*>(pointerDecl);
                dt->setOriginType(classifier);
                dt->setIsReference();
                classifier = dt;
            }  */
        } else {
            o = origType;
        }
    } else if (parentPkg && !bPutAtGlobalScope) {
        UMLPackage *existingPkg = o->umlPackage();
        if (existingPkg != umldoc->datatypeFolder()) {
            if (existingPkg)
                existingPkg->removeObject(o);
            else
                uError() << "createUMLObject(" << name << "): "
                    << "o->getUMLPackage() was NULL";
            o->setUMLPackage(parentPkg);
            parentPkg->addObject(o);
        }
    }
    QString strippedComment = formatComment(comment);
    if (! strippedComment.isEmpty()) {
        o->setDoc(strippedComment);
    }
    if (!stereotype.isEmpty()) {
        o->setStereotype(stereotype);
    }
    if (gRelatedClassifier == NULL || gRelatedClassifier == o)
        return o;
    QRegExp templateInstantiation(QLatin1String("^[\\w:\\.]+\\s*<(.*)>"));
    int pos = templateInstantiation.indexIn(name);
    if (pos == -1)
        return o;
    // Create dependencies on template parameters.
    QString caption = templateInstantiation.cap(1);
    const QStringList params = caption.split(QRegExp(QLatin1String("[^\\w:\\.]+")));
    if (!params.count())
        return o;
    QStringList::ConstIterator end(params.end());
    for (QStringList::ConstIterator it(params.begin()); it != end; ++it) {
        UMLObject *p = umldoc->findUMLObject(*it, UMLObject::ot_UMLObject, parentPkg);
        if (p == NULL || p->baseType() == UMLObject::ot_Datatype)
            continue;
        const Uml::AssociationType::Enum at = Uml::AssociationType::Dependency;
        UMLAssociation *assoc = umldoc->findAssociation(at, gRelatedClassifier, p);
        if (assoc)
            continue;
        assoc = new UMLAssociation(at, gRelatedClassifier, p);
        assoc->setUMLPackage(umldoc->rootFolder(Uml::ModelType::Logical));
        umldoc->addAssociation(assoc);
    }
    if (o == NULL) {
        uError() << "is NULL!";
    }
    return o;
}

/**
 * Create a UMLOperation.
 * The reason for this method is to not generate any Qt signals.
 * Instead, these are generated by insertMethod().
 * (If we generated a creation signal prematurely, i.e. without
 * the method parameters being known yet, then that would lead to
 * a conflict with a pre-existing parameterless method of the same
 * name.)
 */
UMLOperation* makeOperation(UMLClassifier *parent, const QString &name)
{
    UMLOperation *op = Object_Factory::createOperation(parent, name);
    return op;
}

/**
 * Create a UMLAttribute and insert it into the document.
 * Use the specified existing attrType.
 */
UMLObject* insertAttribute(UMLClassifier *owner,
                           Uml::Visibility::Enum scope,
                           const QString& name,
                           UMLClassifier *attrType,
                           const QString& comment /* =QString() */,
                           bool isStatic /* =false */)
{
    UMLObject::ObjectType ot = owner->baseType();
    Uml::ProgrammingLanguage::Enum pl = UMLApp::app()->activeLanguage();
    if (! (ot == UMLObject::ot_Class ||
           (ot == UMLObject::ot_Interface && pl == Uml::ProgrammingLanguage::Java))) {
        DEBUG(DBG_SRC) << "insertAttribute: Don not know what to do with "
                 << owner->name() << " (object type " << UMLObject::toString(ot) << ")";
        return NULL;
    }
    UMLObject *o = owner->findChildObject(name, UMLObject::ot_Attribute);
    if (o) {
        return o;
    }

    UMLAttribute *attr = owner->addAttribute(name, attrType, scope);
    attr->setStatic(isStatic);
    QString strippedComment = formatComment(comment);
    if (! strippedComment.isEmpty()) {
        attr->setDoc(strippedComment);
    }

    UMLApp::app()->document()->setModified(true);
    return attr;
}

/**
 * Create a UMLAttribute and insert it into the document.
 */
UMLObject* insertAttribute(UMLClassifier *owner, Uml::Visibility::Enum scope,
                           const QString& name,
                           const QString& type,
                           const QString& comment /* =QString() */,
                           bool isStatic /* =false */)
{
    UMLObject *attrType = owner->findTemplate(type);
    if (attrType == NULL) {
        bPutAtGlobalScope = true;
        gRelatedClassifier = owner;
        attrType = createUMLObject(UMLObject::ot_UMLObject, type, owner);
        gRelatedClassifier = NULL;
        bPutAtGlobalScope = false;
    }
    return insertAttribute (owner, scope, name,
                            static_cast<UMLClassifier*>(attrType),
                            comment, isStatic);
}

/**
  * Insert the UMLOperation into the given classifier.
  *
  * @param klass          The classifier into which the operation shall be added.
  * @param op             Reference to pointer to the temporary UMLOperation
  *                       for insertion.  The caller relinquishes ownership of the
  *                       object pointed to.  If an UMLOperation of same signature
  *                       already exists  at the classifier then the incoming
  *                       UMLOperation is deleted and the pointer is set to the
  *                       existing UMLOperation.
  * @param scope          The Uml::Visibility of the method
  * @param type           The return type
  * @param isStatic       boolean switch to decide if method is static
  * @param isAbstract     boolean switch to decide if method is abstract
  * @param isFriend       true boolean switch to decide if methods is a friend function
  * @param isConstructor  boolean switch to decide if methods is a constructor
  * @param comment        The Documentation for this method
  */
void insertMethod(UMLClassifier *klass, UMLOperation* &op,
                  Uml::Visibility::Enum scope, const QString& type,
                  bool isStatic, bool isAbstract,
                  bool isFriend, bool isConstructor,
                  const QString& comment)
{
    op->setVisibilityCmd(scope);
    if (!type.isEmpty()     // return type may be missing (constructor/destructor)
        && type != QLatin1String("void")) {
        if (type == klass->name()) {
            op->setType(klass);
        } else {
            UMLObject *typeObj = klass->findTemplate(type);
            if (typeObj == NULL) {
                bPutAtGlobalScope = true;
                gRelatedClassifier = klass;
                typeObj = createUMLObject(UMLObject::ot_UMLObject, type, klass);
                gRelatedClassifier = NULL;
                bPutAtGlobalScope = false;
                op->setType(typeObj);
            }
        }
    }

    op->setStatic(isStatic);
    op->setAbstract(isAbstract);

    // if the operation is friend, add it as a stereotype
    if (isFriend)
        op->setStereotype(QLatin1String("friend"));
    // if the operation is a constructor, add it as a stereotype
    if (isConstructor)
        op->setStereotype(QLatin1String("constructor"));

    QString strippedComment = formatComment(comment);
    if (! strippedComment.isEmpty()) {
        op->setDoc(strippedComment);
    }

    UMLAttributeList params = op->getParmList();
    UMLOperation *exist = klass->checkOperationSignature(op->name(), params);
    if (exist) {
        // copy contents to existing operation
        exist->setVisibilityCmd(scope);
        exist->setStatic(isStatic);
        exist->setAbstract(isAbstract);
        if (! strippedComment.isEmpty())
            exist->setDoc(strippedComment);
        UMLAttributeList exParams = exist->getParmList();
        for (UMLAttributeListIt it(params), exIt(exParams) ; it.hasNext() ;) {
            UMLAttribute *param = it.next(), *exParam = exIt.next();
            exParam->setName(param->name());
            exParam->setVisibilityCmd(param->visibility());
            exParam->setStatic(param->isStatic());
            exParam->setAbstract(param->isAbstract());
            exParam->setDoc(param->doc());
            exParam->setInitialValue(param->getInitialValue());
            exParam->setParmKind(param->getParmKind());
        }
        // delete incoming UMLOperation and pass out the existing one
        delete op;
        op = exist;
    } else {
        klass->addOperation(op);
    }
}

/**
 * Add an argument to a UMLOperation.
 * The parentPkg arg is only used for resolving possible scope
 * prefixes in the `type'.
 */
UMLAttribute* addMethodParameter(UMLOperation *method,
                                 const QString& type,
                                 const QString& name)
{
    UMLClassifier *owner = static_cast<UMLClassifier*>(method->parent());
    UMLObject *typeObj = owner->findTemplate(type);
    if (typeObj == NULL) {
        bPutAtGlobalScope = true;
        gRelatedClassifier = owner;
        typeObj = createUMLObject(UMLObject::ot_UMLObject, type, owner);
        gRelatedClassifier = NULL;
        bPutAtGlobalScope = false;
    }
    UMLAttribute *attr = Object_Factory::createAttribute(method, name, typeObj);
    method->addParm(attr);
    return attr;
}

/**
 * Add an enum literal to an UMLEnum.
 */
void addEnumLiteral(UMLEnum *enumType, const QString &literal, const QString &comment, const QString &value)
{
    UMLObject *el = enumType->addEnumLiteral(literal, Uml::ID::None, value);
    el->setDoc(comment);
}

/**
 * Create a generalization from the given child classifier to the given
 * parent classifier.
 */
void createGeneralization(UMLClassifier *child, UMLClassifier *parent)
{
    // if the child is an interface, so is the parent.
    if (child->isInterface())
        parent->setBaseType(UMLObject::ot_Interface);
    Uml::AssociationType::Enum association = Uml::AssociationType::Generalization;

    if (parent->isInterface() && !child->isInterface()) {
        // if the parent is an interface, but the child is not, then
        // this is really realization.
        //
        association = Uml::AssociationType::Realization;
    }
    UMLAssociation *assoc = new UMLAssociation(association, child, parent);
    UMLDoc *umldoc = UMLApp::app()->document();
    assoc->setUMLPackage(umldoc->rootFolder(Uml::ModelType::Logical));
    umldoc->addAssociation(assoc);
}

/**
 * Create an artifact with the given name.
 */
UMLObject *createArtifact(const QString& name,
                          UMLPackage *parentPkg,
                          const QString &comment)
{
    Q_UNUSED(parentPkg);

    UMLObject::ObjectType type = UMLObject::ot_Artifact;
    if (!Settings::optionState().codeImportState.createArtifacts)
        return 0;
    UMLDoc *umldoc = UMLApp::app()->document();
    UMLFolder *componentView = umldoc->rootFolder(Uml::ModelType::Component);
    QFileInfo fi(name);
    UMLObject *o = umldoc->findUMLObjectRaw(componentView, fi.fileName(), type);
    if (o)
        return o;
    o = Object_Factory::createUMLObject(type, fi.fileName(), componentView, false);
    UMLArtifact *a = static_cast<UMLArtifact*>(o);
    a->setDrawAsType(UMLArtifact::file);
    a->setDoc(comment);
    DEBUG(DBG_SRC) << name << comment;
    return o;
}

/**
 * Create a generalization from the existing child UMLObject to the given
 * parent class name.
 * This method does not handle scopes well and is only a last resort.
 * The method
 *   createGeneralization(UMLClassifier *child, UMLClassifier *parent)
 * should be used instead.
 */
void createGeneralization(UMLClassifier *child, const QString &parentName)
{
    UMLObject *parentObj = createUMLObject(UMLObject::ot_Class, parentName);
    UMLClassifier *parent = static_cast<UMLClassifier*>(parentObj);
    createGeneralization(child, parent);
}

/**
 * Return the list of paths set by previous calls to addIncludePath()
 * and the environment variable UMBRELLO_INCPATH.
 * This list can be used for finding included (or Ada with'ed or...)
 * files.
 */
QStringList includePathList()
{
    QStringList includePathList(incPathList);
    QString umbrello_incpath = QString::fromLatin1(qgetenv("UMBRELLO_INCPATH"));
    if (!umbrello_incpath.isEmpty()) {
        includePathList += umbrello_incpath.split(PATH_SEPARATOR);
    }
    return includePathList;
}

/**
 * Add a path to the include path list.
 */
void addIncludePath(const QString& path)
{
    if (! incPathList.contains(path))
        incPathList.append(path);
}

/**
 * Returns true if a type is an actual Datatype
 */
bool isDatatype(const QString& name, UMLPackage *parentPkg)
{
    UMLDoc *umldoc = UMLApp::app()->document();
    UMLObject * o = umldoc->findUMLObject(name, UMLObject::ot_Datatype, parentPkg);
    return (o!=NULL);
}

/**
 * Helper function to parse stack trace line. The supported formats are:
 *
 * 1. gdb
 * 2. Qtcreator/gdb
 *
 * @param s string to parse
 * @param sequence return of sequence number
 * @param package return of package
 * @param method return of method
 * @return true line could be parsed and return variable has been filled
 * @return false line could not be parsed, no return variable has been filled
 */
bool parseStraceTraceLine(const QString &s, QString &sequence, QString &package, QString &method)
{
    QString identifier;
    QString module;
    QStringList cols = s.split(QRegExp(QLatin1String("\\s+")),QString::SkipEmptyParts);
    if (cols.size() < 2) {
        DEBUG(DBG_SRC) << "could not parse" << s;
        return false;
    }
    // skip comments
    if (cols[0] == QLatin1String("#")) {
        return false;
    }

    /*  gdb
     * #0  0x000000000050d0b0 in Import_Utils::importStackTrace(QString const&, UMLScene*) (fileName=..., scene=scene@entry=0x12bd0f0)
     * #25 0x00007fffefe1670c in g_main_context_iteration () from /usr/lib64/libglib-2.0.so.0
     * #0  Import_Utils::importStackTrace (fileName=..., scene=scene@entry=0x137c000) at /home/ralf.habacker/src/umbrello-3/umbrello/codeimport/import_utils.cpp:715
     */
    if (cols.size() > 2 && cols[0].startsWith(QLatin1String("#"))) {
        QString file;
        sequence = cols.takeFirst();
        if (cols[cols.size()-2] == QLatin1String("at")) {
            file = cols.takeLast();
            cols.takeLast();
        }
        else if (cols[cols.size()-2] == QLatin1String("from")) {
            module = cols.takeLast();
            cols.takeLast();
        }

        if (cols[1] == QLatin1String("in")) {
            cols.takeFirst(); // remove address
            cols.takeFirst(); // in
        }

        identifier = cols.join(QLatin1String(" "));

        if (identifier.contains(QLatin1String("::"))) {
            QStringList b = identifier.split( QLatin1String("::"));
            // TODO handle multiple '::'
            package = b.takeFirst();
            method = b.join(QLatin1String("::"));
        }
        else {
            method = identifier;
            package = module;
        }
        return true;
    }

    /**
     * Qtcreator/gdb
     *  6	Driver::ParseHelper::ParseHelper	driver.cpp	299	0x634c44
     * 31   g_main_context_dispatch /usr/lib64/libglib-2.0.so.0     0x7fffefe16316
     */
    else if (cols[cols.size()-1].startsWith(QLatin1String("0x"))) {
        if (cols[0] == QLatin1String("..."))
            return false;

        sequence = cols.takeFirst();
        cols.takeLast();  // remove address

        QString line, file;
        if (cols.size() == 2) {
            module = cols.takeLast();
            identifier = cols.join(QLatin1String(" "));
        } else {
            line = cols.takeLast();
            file = cols.takeLast();
            identifier = cols.join(QLatin1String(" "));
        }

        if (identifier.contains(QLatin1String("::"))) {
            QStringList b = identifier.split( QLatin1String("::"));
            method = b.takeLast();
            package = b.join(QLatin1String("::"));
        }
        else {
            method = identifier;
            package = module;
        }

        if (package.isEmpty() && !file.isEmpty())
            package = file;

        if (!method.endsWith(QLatin1String(")")))
            method.append(QLatin1String("()"));

        return true;
    } else
        return false;
}

/**
 * Import stack trace generated by gdb or qtcreator running gdb.
 *
 * The stack trace can be generated by
 *  1. copy and paste of the output of gdb 'bt' command into a file (single line for each stack frame)
 *  2. select frames in qtcreator stack window, right click and select "copy into clipboard", then paste into file
 *
 * @param fileName filename to import the stack trace from.
 * @param scene The diagram to import the stack trace into.
 * @return true Import successful.
 * @return false Import failed.
 */
bool importStackTrace(const QString &fileName, UMLScene *scene)
{
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly))
        return false;

    // reverse lines
    QStringList lines;
    QTextStream in(&file);
    in.setCodec("UTF-8");
    while (!in.atEnd()) {
        lines.insert(0, in.readLine());
    }

    // object widget cache map
    QMap<QString, ObjectWidget*> objectsMap;

    // create "client widget"
    UMLDoc *umldoc = UMLApp::app()->document();
    QString name(QLatin1String("client"));
    UMLObject *left = umldoc->findUMLObject(name, UMLObject::ot_Class);
    if (!left ) {
        left = new UMLObject(0, name);
        left->setBaseType(UMLObject::ot_Class);
    }

    ObjectWidget *leftWidget = (ObjectWidget *)Widget_Factory::createWidget(scene, left);
    leftWidget->activate();
    // required to be savable
    scene->widgetList().append(leftWidget);
    objectsMap[name] = leftWidget;

    ObjectWidget *rightWidget = 0;
    ObjectWidget *mostRightWidget = leftWidget;
    MessageWidget *messageWidget = 0;
    // for further processing
    MessageWidgetList messages;

    // for each line
    int index = 1;
    foreach(const QString &line, lines) {
        QString stackframe, package, method;

        if (!parseStraceTraceLine(line, stackframe, package, method))
            continue;

        bool createObject = false;
        if (package.contains(method))
            createObject = true;

        if (package.isEmpty())
            package = QLatin1String("unknown");

        // get or create right object widget
        if (objectsMap.contains(package)) {
            rightWidget = objectsMap[package];
        } else {
            UMLObject *right = umldoc->findUMLObject(package, UMLObject::ot_Class);
            if (!right)
                right = umldoc->findUMLObject(package, UMLObject::ot_Package);
            if (!right) {
                right = Object_Factory::createUMLObject(UMLObject::ot_Class, package);
            }

            rightWidget = (ObjectWidget *)Widget_Factory::createWidget(scene, right);
            rightWidget->setX(mostRightWidget->x() + mostRightWidget->width() + 10);
            rightWidget->activate();
            objectsMap[package] = rightWidget;
            scene->widgetList().append(rightWidget);
            mostRightWidget = rightWidget;
        }

        // create message
        int y = 10 + (messageWidget ? messageWidget->y() + messageWidget->height() : 20);
        messageWidget = new MessageWidget(scene, leftWidget, rightWidget, y,
                                          createObject ? Uml::SequenceMessage::Creation : Uml::SequenceMessage::Synchronous);
        messageWidget->setCustomOpText(method);
        messageWidget->setSequenceNumber(QString::number(index++));
        messageWidget->calculateWidget();
        messageWidget->activate();
        messageWidget->setY(y);
        // to make it savable
        scene->messageList().append(messageWidget);
        messages.insert(0, messageWidget);

        leftWidget = rightWidget;
    }

    if (messages.isEmpty())
        return false;

    // adjust vertical position
    foreach(MessageWidget *w, messages) {
        w->setY(w->y() + 20);
    }

    // adjust heights starting from the last message
    MessageWidget *previous = messages.takeFirst();
    foreach(MessageWidget *w, messages) {
        w->setSize(w->width(), previous->y() - w->y() + previous->height() + 5);
        // adjust vertical line length of object widgets
        w->objectWidget(Uml::RoleType::A)->slotMessageMoved();
        w->objectWidget(Uml::RoleType::B)->slotMessageMoved();
        previous = w;
    }
    return true;
}

}  // end namespace Import_Utils
