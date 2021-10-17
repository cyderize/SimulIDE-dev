/***************************************************************************
 *   Copyright (C) 2020 by santiago González                               *
 *   santigoro@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/

#ifndef PLOTBASE_H
#define PLOTBASE_H

#include <QScriptEngine>
#include <QScriptProgram>

#include "component.h"
#include "e-element.h"
#include "datachannel.h"

class PlotDisplay;
class QGraphicsProxyWidget;

class MAINMODULE_EXPORT PlotBase : public Component, public eElement
{
    public:
        PlotBase( QObject* parent, QString type, QString id );
        ~PlotBase();

        int baSizeX() { return m_baSizeX; }
        void setBaSizeX( int size );

        int baSizeY() { return m_baSizeY; }
        void setBaSizeY( int size );

        double dataSize() { return m_dataSize/1e6; }
        void setDataSize( double ds ) { m_dataSize = ds*1e6; }

        int hTick() { return m_timeDiv/1e3; }
        virtual void sethTick( int td ){ setTimeDiv( (uint64_t)td*1e3 );}

        double timeDiv() { return m_timeDiv; }
        virtual void setTimeDiv( double td );

        int trigger() { return m_trigger; }

        virtual QString tunnels();
        virtual void setTunnels( QString tunnels )=0;

        virtual void expand( bool e ){;}
        void toggleExpand() { expand( !m_expand ); }

        QString conds() { return m_conditions; }
        virtual void setConds( QString conds ){;}
        void updateConds( QString conds );

        virtual void channelChanged( int ch, QString name ) { m_channel[ch]->m_chTunnel = name; }

        PlotDisplay* display() { return m_display; }

        QColor getColor( int c ) { return m_color[c]; }

        void conditonMet( int ch, cond_t cond );

        virtual void paint( QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* widget );

    protected:
        int m_bufferSize;
        int m_trigger;

        bool m_expand;

        int m_screenSizeX;
        int m_screenSizeY;
        int m_baSizeX;
        int m_baSizeY;

        double m_dataSize;

        uint64_t m_timeDiv;
        uint64_t m_risEdge;

        int m_numChannels;
        DataChannel* m_channel[8];

        QString m_conditions;
        QScriptProgram m_condProgram;
        QScriptEngine m_engine;

        QHash<QString, QString> m_condTo;

        QColor m_color[5];

        PlotDisplay* m_display;

        QGraphicsProxyWidget* m_proxy;
};

#endif
