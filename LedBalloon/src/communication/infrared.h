/**
 * ** GNU General Public License Usage
 ** This file may be used under the terms of the GNU
 ** General Public License version 3 or (at your option) any later version
 ** approved by the KDE Free Qt Foundation. The licenses are as published by
 ** the Free Software Foundation and appearing in the file LICENSE.GPL3
 ** included in the packaging of this file. Please review the following
 ** information to ensure the GNU General Public License requirements will
 ** be met: https://www.gnu.org/licenses/gpl-3.0.html.* 
 * 
 * description     :Handles the connection via infrared via receiver / sender
 *                  It uses the wonderfull IRLremote library
 * 
 * variables       :
 *                 tbd
 * 
 * methods         :
 *                 tbd
 *                  
 * author          :Philipp Wuerfel
 * date            :2020-05-06
 * version         :1.0
 * notes           :Still running tests in prototype environment
 * language        :C++
*/

#ifndef INFRARED_H
#define INFRARED_H

#include "commands.h"

#include "IRLremote.h"                                        // https://github.com/NicoHood/IRLremote  Use 2.0.2
#if IRL_VERSION < 202
#error "Requires IRLRemote 2.0.2 or later; check github for latest code."
#endif

#define PIN_IR 2 // PinInterrupt pin of your Arduino board for infrared sensor
#define IRL_BLOCKING true

class Infrared{
    private:
        CNec IRLremote;
    public:
        Infrared();
        ~Infrared();

        int currentmode;

        bool begin();
        bool receiving();
        void change_mode(int newmode);
        void irlcall();
};

#endif