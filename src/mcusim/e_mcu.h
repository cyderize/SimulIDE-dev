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

#ifndef EMCU_H
#define EMCU_H

#include "e-element.h"
#include "itemlibrary.h"
#include "mcuport.h"
#include "mcutimer.h"
#include "mcuuart.h"
#include "mcuinterrupts.h"
#include "mcudataspace.h"
#include "e-element.h"

class McuCore;
class McuTimer;
class McuWdt;
class McuSleep;

enum{
    R_READ = 0,
    R_WRITE,
};

enum mcuState_t{
    mcuStopped=0,
    mcuRunning,
    mcuSleeping
};

class McuVref;

class MAINMODULE_EXPORT eMcu : public DataSpace, public eElement
{
        friend class McuCreator;
        friend class McuCore;
        friend class Mcu;

    public:
        eMcu( QString id );
        ~eMcu();

 static eMcu* self() { return m_pSelf; }

        virtual void initialize() override;
        virtual void runEvent() override;

        void stepCpu();
        //void stepDebug();
        //void stepFromLine( int line );

        void setDebugger( BaseDebugger* deb );
        void setDebugging( bool d ) { m_debugging = d; }

        uint16_t getFlashValue( int address ) { return m_progMem[address]; }
        void     setFlashValue( int address, uint16_t value ) { m_progMem[address] = value; }
        uint32_t flashSize(){ return m_flashSize; }
        uint32_t wordSize() { return m_wordSize; }

        virtual QVector<int>* eeprom() { return &m_eeprom; }
        virtual void setEeprom( QVector<int>* eep );
        uint32_t romSize()  { return m_romSize; }
        uint8_t  getRomValue( int address ) { return m_eeprom[address]; }
        void     setRomValue( int address, uint8_t value ) { m_eeprom[address] = value; }


        uint32_t getStack();
        int status();
        int pc();
        uint64_t cycle(){ return m_cycle; }


        void cpuReset( bool r );
        void sleep( bool s );

        QString getFileName() { return m_firmware; }

        double freqMHz() { return m_freq*1e-6; }
        void setFreq( double freq );
        uint64_t simCycPI() { return m_simCycPI; }  // Simulation cycles per instruction cycle
        //double cpi() { return m_cPerInst; }       // Clock ticks per Instruction Cycle

        //McuPin* getPin( QString name ) { return m_ports.getPin( name ); }
        //QHash<QString, McuPort*> getPorts() { return m_ports.getPorts(); }

        McuTimer* getTimer( QString name ) { return m_timers.getTimer( name ); }

        McuWdt* watchDog() { return m_wdt; }
        McuVref* vrefModule();
        //McuSleep* sleepModule();

        void wdr();

        void enableInterrupts( uint8_t en );

        bool setCfgWord( uint16_t addr, uint16_t data );
        uint16_t getCfgWord( uint16_t addr=0 );

        McuCore* cpu;
        int cyclesDone;

        void setMain() { m_pSelf = this; }

    protected:
 static eMcu* m_pSelf;

        void reset();

        QString m_firmware;     // firmware file loaded
        QString m_device;

        mcuState_t m_state;

        uint64_t m_cycle;
        std::vector<uint16_t> m_progMem;  // Program memory
        uint32_t m_flashSize;
        uint8_t  m_wordSize; // Size of Program memory word in bytes

        QHash<QString, int>   m_regsTable;   // int max 32 bits

        uint32_t m_romSize;
        QVector<int> m_eeprom;

        QHash<uint16_t, uint16_t> m_cfgWords; // Config words

        Interrupts m_interrupts;
        McuTimers  m_timers;
        std::vector<McuModule*> m_modules;
        std::vector<McuUsart*> m_usarts;

        McuSleep* m_sleepModule;
        McuVref* m_vrefModule;
        McuWdt* m_wdt;

        //bool m_resetState;

        double m_freq;           // Clock Frequency in MegaHerzs
        double m_cPerInst;       // Clock ticks per Instruction Cycle
        uint64_t m_simCycPI;     // Simulation cycles per Instruction Cycle

        // Debugger:
        BaseDebugger* m_debugger;

        bool m_debugging;
        //bool m_debugStep;
        //int  m_prevLine;
};

#endif
