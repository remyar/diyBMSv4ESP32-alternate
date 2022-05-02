import React, { useState, useEffect } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import Infos from '../../components/infos';
import Graph from '../../components/graph';

import utils from '../../utils';

function Home(props) {

    const settings = props.globalState.settings || {};
    const monitor2 = props.globalState.monitor2 || {};

    let infosTab = [];

    if (monitor2.sent && monitor2.sent > 0) {
        infosTab.push(<div id="sent" className="stat"><span className="x t">Packets sent:</span><span className="x v">{monitor2.sent}</span></div>);
    }
    if (monitor2.received && monitor2.received > 0) {
        infosTab.push(<div id="received" className="stat"><span className="x t">Packets rec'd:</span><span className="x v">{monitor2.received}</span></div>);
    }
    if (monitor2.roundtrip && monitor2.roundtrip > 0) {
        infosTab.push(<div id="roundtrip" className="stat"><span className="x t">Roundtrip (ms):</span><span className="x v">{monitor2.roundtrip}</span></div>);
    }
    if (monitor2.uptime && monitor2.uptime > 0) {
        infosTab.push(<div id="uptime" className="stat"><span className="x t">Uptime:</span><span className="x v">{utils.secondsToHms(monitor2.uptime)}</span></div>);
    }
    if (monitor2.can_sent && monitor2.can_sent > 0) {
        infosTab.push(<div id="cansent" className="stat"><span className="x t">CAN sent:</span><span className="x v">{monitor2.can_sent}</span></div>);
    }
    if (monitor2.can_rec && monitor2.can_rec > 0) {
        infosTab.push(<div id="canrecd" className="stat"><span className="x t">CAN received:</span><span className="x v">{monitor2.can_rec}</span></div>);
    }

    // Need one color for each pack, could make it colourful I suppose :-)
    const colours = [
        '#55a1ea', '#33628f', '#55a1ea', '#33628f',
        '#55a1ea', '#33628f', '#55a1ea', '#33628f',
        '#55a1ea', '#33628f', '#55a1ea', '#33628f',
        '#55a1ea', '#33628f', '#55a1ea', '#33628f',
    ];

    const red = '#B44247';

    return <div>
        {(() => {
            let comp = [];
            for (let i = 0; i < settings.totalControllers; i++) {

                let bankNumber = 0;
                let cellsInBank = 0;

                let minVoltage = 2.9;
                let maxVoltage = 4.3;

                let bank = [];
                let labels = [];

                let voltages = [];
                let voltagesmin = [];
                let voltagesmax = [];
                let tempint = [];
                let tempext = [];
                let pwm = [];

                let markLineData = [];

                markLineData.push({ name: 'avg', type: 'average', lineStyle: { color: '#ddd', width: 2, type: 'dotted', opacity: 0.3 }, label: { distance: [10, 0], position: 'start', color: "#eeeeee", textBorderColor: '#313131', textBorderWidth: 2 } });
                markLineData.push({ name: 'min', type: 'min', lineStyle: { color: '#ddd', width: 2, type: 'dotted', opacity: 0.3 }, label: { distance: [10, 0], position: 'start', color: "#eeeeee", textBorderColor: '#313131', textBorderWidth: 2 } });
                markLineData.push({ name: 'max', type: 'max', lineStyle: { color: '#ddd', width: 2, type: 'dotted', opacity: 0.3 }, label: { distance: [10, 0], position: 'start', color: "#eeeeee", textBorderColor: '#313131', textBorderWidth: 2 } });
            
                let xAxis = 0;
                 for (let index = 0; index < (monitor2?.controllers && monitor2?.controllers[i] && monitor2?.controllers[i].banks) ; index++) {
                    markLineData.push({ name: "Bank " + index, xAxis: xAxis, lineStyle: { color: colours[index], width: 4, type: 'dashed', opacity: 0.5 }, label: { show: true, distance: [0, 0], formatter: '{b}', color: '#eeeeee', textBorderColor: colours[index], textBorderWidth: 2 } });
                    xAxis += parseInt(monitor2.controllers[i].seriesmodules);
                }

                if (monitor2 && monitor2?.controllers && monitor2?.controllers[i]) {
                    for (let j = 0; j < monitor2.controllers[i].voltages.length; j++) {
                        labels.push(bankNumber + "/" + cellsInBank);

                        // Make different banks different colours (stripes)
                        let stdcolor = colours[bankNumber];
                        // Red
                        let color = monitor2?.controllers[i].bypass[j] == 1 ? red : stdcolor;

                        var v = (parseFloat(monitor2.controllers[i].voltages[j]) / 1000.0);
                        voltages.push({ value: v, itemStyle: { color: color } });

                        //Auto scale graph is outside of normal bounds
                        if (v > maxVoltage) { maxVoltage = v; }
                        if (v < minVoltage) { minVoltage = v; }

                        if (monitor2.controllers[i].minvoltages) {
                            voltagesmin.push((parseFloat(monitor2.controllers[i].minvoltages[j]) / 1000.0));
                        }
                        if (monitor2.maxvoltages) {
                            voltagesmax.push((parseFloat(monitor2.controllers[i].maxvoltages[j]) / 1000.0));
                        }

                        bank.push(bankNumber);

                        cellsInBank++;
                        if (cellsInBank == monitor2.controllers[i].seriesmodules) {
                            cellsInBank = 0;
                            bankNumber++;
                        }

                        color = monitor2.controllers[i].bypasshot[j] == 1 ? red : stdcolor;
                        tempint.push({ value: monitor2.controllers[i].inttemp[j], itemStyle: { color: color } });
                        tempext.push({ value: (monitor2.controllers[i].exttemp[j] == -40 ? 0 : monitor2.controllers[i].exttemp[j]), itemStyle: { color: stdcolor } });
                        pwm.push({ value: monitor2.controllers[i].bypasspwm[j] == 0 ? null : Math.trunc(monitor2.controllers[i].bypasspwm[j] / 255 * 100) });
                    }
                }

                comp.push(<div className="graphs">
                    <Graph id={"graph" + i}
                        markLine={markLineData}
                        xAxis={{ data: labels }}
                        yAxis={[{ gridIndex: 0, min: minVoltage, max: maxVoltage }]}
                        series={[{ name: 'Voltage', data: voltages }
                            , { name: 'Min V', data: voltagesmin }
                            , { name: 'Max V', data: voltagesmax }
                            , { name: 'Bypass', data: pwm }
                            , { name: 'BypassTemperature', data: tempint }
                            , { name: 'CellTemperature', data: tempext }]}
                    />
                </div>)
            }
            return comp;
        })()}



        <Infos infosTab={infosTab} />
    </div>;
}

export default withStoreProvider(withNavigation(injectIntl(Home)));