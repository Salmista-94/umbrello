/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2006                                                    *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// kde includes
#include <kcursor.h>
#include <kdebug.h>

// app includes
#include "messagewidgetcontroller.h"
#include "messagewidget.h"
#include "floatingtextwidget.h"
#include "objectwidget.h"
#include "listpopupmenu.h"

MessageWidgetController::MessageWidgetController(MessageWidget* messageWidget):
        UMLWidgetController(messageWidget) {
    m_messageWidget = messageWidget;
}

MessageWidgetController::~MessageWidgetController() {
}

void MessageWidgetController::saveWidgetValues(QMouseEvent *me) {
    UMLWidgetController::saveWidgetValues(me);

    m_unconstrainedPositionY = m_widget->getY();
}

QCursor MessageWidgetController::getResizeCursor() {
    return KCursor::sizeVerCursor();
}

void MessageWidgetController::resizeWidget(int newW, int newH) {
    if (m_messageWidget->getSequenceMessageType() == Uml::sequence_message_creation)
        m_messageWidget->setSize(m_messageWidget->width(), newH);
    else {
        int x1 = m_messageWidget->m_pOw[Uml::A]->getX();
        int x2 = m_messageWidget->getxclicked();
        int diffX = 0;
        if (x1 < x2) {
            diffX = x2 + (newW - m_messageWidget->width());
        }
        else {
            diffX = x2 - (newW - m_messageWidget->width());
        }
        if (diffX <= 0 )
            diffX = 10;
        m_messageWidget->setxclicked (diffX);
        m_messageWidget->setSize(newW, newH);
        m_messageWidget->calculateWidget();

    }
    emit m_messageWidget->sigMessageMoved();
}

void MessageWidgetController::moveWidgetBy(int diffX, int diffY) {
    m_unconstrainedPositionY += diffY;
    int newY = constrainPositionY(diffY);

    if (m_unconstrainedPositionY != newY) {
        if (m_unconstrainedPositionY > m_messageWidget->getY()) {
            newY = m_unconstrainedPositionY;
        } else {
            return;
        }
    }

    m_messageWidget->setY(newY);

    if (m_messageWidget->m_sequenceMessageType == Uml::sequence_message_creation) {
        const int objWidgetHalfHeight = m_messageWidget->m_pOw[Uml::B]->getHeight() / 2;
        m_messageWidget->m_pOw[Uml::B]->UMLWidget::setY(newY - objWidgetHalfHeight);
    }

    m_messageWidget->moveEvent(0);
}

void MessageWidgetController::constrainMovementForAllWidgets(int &diffX, int &diffY) {
    diffX = 0;
    diffY = constrainPositionY(diffY) - m_widget->getY();
}

void MessageWidgetController::doMouseDoubleClick(QMouseEvent *me) {
    if (m_messageWidget->m_pFText != NULL) {
        m_messageWidget->m_pFText->slotMenuSelection(ListPopupMenu::mt_Select_Operation);
    }
}

int MessageWidgetController::constrainPositionY(int diffY) {
    int newY = m_widget->getY() + diffY;

    int minY = m_messageWidget->getMinY();
    if (m_messageWidget->m_pFText && !m_messageWidget->m_pFText->getDisplayText().isEmpty()) {
        minY += m_messageWidget->m_pFText->getHeight();
    }

    if (newY < minY) {
        newY = minY;
    }

    return newY;
}

