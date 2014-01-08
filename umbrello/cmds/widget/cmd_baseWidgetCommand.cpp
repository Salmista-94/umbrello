/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2007-2013                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#include "cmd_baseWidgetCommand.h"

// app includes
#include "uml.h"
#include "umldoc.h"
#include "umlscene.h"
#include "umlview.h"
#include "umlwidget.h"

// kde includes
#include <klocale.h>

namespace Uml
{
    CmdBaseWidgetCommand::CmdBaseWidgetCommand(UMLWidget* widget)
    {
        setWidget(widget);
    }

    CmdBaseWidgetCommand::~CmdBaseWidgetCommand()
    {
    }

    void CmdBaseWidgetCommand::setWidget(UMLWidget* widget)
    {
        Q_ASSERT(widget);

        m_widgetId = widget->localID();
        m_sceneId = widget->umlScene()->ID();
    }

    UMLScene* CmdBaseWidgetCommand::scene()
    {
        UMLView* umlView = UMLApp::app()->document()->findView(m_sceneId);

        Q_ASSERT(umlView);

        return umlView->umlScene();
    }

    UMLWidget* CmdBaseWidgetCommand::widget()
    {
        UMLWidget* umlWidget = scene()->findWidgetByLocalId(m_widgetId);

        Q_ASSERT(umlWidget);

        return umlWidget;
    }
}