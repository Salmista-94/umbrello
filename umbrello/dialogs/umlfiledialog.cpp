/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2012-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

// own header
#include "umlfiledialog.h"
#include "umlviewimageexportermodel.h"

// kde includes
#if QT_VERSION < 0x050000
#include <KMimeType>
#endif
#include <KLocalizedString>

#if QT_VERSION < 0x050000
static QStringList mime2KdeFilter(const QStringList &mimeTypes, QString *allExtensions = 0)
{
    const KUrl emptyUrl;
    QStringList kdeFilter;
    QStringList allExt;
    foreach(const QString& mimeType, mimeTypes) {
        KMimeType::Ptr mime(KMimeType::mimeType(mimeType));
        if (mime) {
            allExt += mime->patterns();
            kdeFilter.append(mime->patterns().join(QLatin1String(" ")) + QLatin1Char('|') +
            mime->comment(emptyUrl));
        }
        else if (mimeType == QString::fromLatin1("image/x-dot")) {
            allExt += QString::fromLatin1("*.dot");
            kdeFilter.append(QLatin1String("*.dot|Dot Files"));
        }
    }
    if (allExtensions) {
        allExt.sort();
        *allExtensions = allExt.join(QLatin1String(" "));
    }
    return kdeFilter;
}
#endif

#if QT_VERSION >= 0x050000
UMLFileDialog::UMLFileDialog(const QUrl &startDir, const QString &filter, QWidget *parent, QWidget *widget)
  : m_dialog(new QFileDialog(parent, QString(), startDir.toLocalFile(), filter))
#else
UMLFileDialog::UMLFileDialog(const KUrl &startDir, const QString &filter, QWidget *parent, QWidget *widget)
  : m_dialog(new KFileDialog(startDir, filter, parent, widget))
#endif
{
    Q_UNUSED(widget);
}

UMLFileDialog::~UMLFileDialog()
{
    delete m_dialog;
}

int UMLFileDialog::exec()
{
    return m_dialog->exec();
}

#if QT_VERSION >= 0x050000
void UMLFileDialog::setCaption(const QString &caption)
{
    m_dialog->setWindowTitle(caption);
}

void UMLFileDialog::setAcceptMode(QFileDialog::AcceptMode mode)
{
    m_dialog->setAcceptMode(mode);
}

void UMLFileDialog::setMimeTypeFilters(const QStringList &filters)
{
    m_dialog->setMimeTypeFilters(filters);
}

void UMLFileDialog::selectUrl(const QUrl &url)
{
    m_dialog->selectUrl(url);
}

void UMLFileDialog::setUrl(const QUrl &url)
{
    m_dialog->selectUrl(url);
}

QUrl UMLFileDialog::selectedUrl()
{
    QList<QUrl> urls = m_dialog->selectedUrls();
    if (urls.size() > 0)
        return urls.first();
    else
        return QUrl();
}

void UMLFileDialog::setSelection(const QString &name)
{
    m_dialog->selectFile(name);
}

#else
void UMLFileDialog::setCaption(const QString &caption)
{
    m_dialog->setCaption(caption);
}

void UMLFileDialog::setOperationMode(KFileDialog::OperationMode mode)
{
    m_dialog->setOperationMode(mode);
}

void UMLFileDialog::setUrl(const KUrl &url, bool clearforward)
{
    m_dialog->setUrl(url, clearforward);
}

KUrl UMLFileDialog::selectedUrl()
{
    return m_dialog->selectedUrl();
}

void UMLFileDialog::setSelection(const QString &name)
{
    m_dialog->setSelection(name);
}

void UMLFileDialog::setMimeFilter(const QStringList &types, const QString &defaultType)
{
    QString allExtensions;
    QStringList filters = mime2KdeFilter(types, &allExtensions);
    if (defaultType.isEmpty() && (types.count() > 1)) {
        filters.prepend(allExtensions + QLatin1Char('|') + i18n("All Supported Files"));
    }
    m_dialog->setFilter(filters.join(QLatin1String("\n")));
}

/**
 * @brief return the mime type of the selected file
 * @return mime type string
 */
QString UMLFileDialog::currentMimeFilter()
{
    KUrl url = m_dialog->selectedUrl();
    QFileInfo fi(url.toLocalFile());
    return UMLViewImageExporterModel::imageTypeToMimeType(fi.suffix());
}
#endif
