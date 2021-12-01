import React, { useState, useEffect } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import actionsBms from '../../actions/bms';

import GlobalSettings from '../../components/globalSettings';
import ModuleConfig from '../../components/moduleConfig';

function Modules(props) {

    const monitor2 = props.globalState.monitor2 || {};
    const monitor3 = props.globalState.monitor3 || {};

    const [displayConfiguration, setDislayConfiguration] = useState({ display: false, cellid: 0 });

    let cells = [];
    let bank = [];
    let voltages = [];
    let voltagesmin = [];
    let voltagesmax = [];
    let tempint = [];
    let tempext = [];
    let pwm = [];

    let bankNumber = 0;
    let cellsInBank = 0;

    if (monitor2.voltages) {
        for (let i = 0; i < monitor2.voltages.length; i++) {

            var v = (parseFloat(monitor2.voltages[i]) / 1000.0);
            voltages.push({ value: v });

            if (monitor2.minvoltages) {
                voltagesmin.push((parseFloat(monitor2.minvoltages[i]) / 1000.0));
            }
            if (monitor2.maxvoltages) {
                voltagesmax.push((parseFloat(monitor2.maxvoltages[i]) / 1000.0));
            }

            bank.push(bankNumber);

            cells.push(i);

            cellsInBank++;
            if (cellsInBank == monitor2.seriesmodules) {
                cellsInBank = 0;
                bankNumber++;
            }

            tempint.push({ value: monitor2.inttemp[i] });
            tempext.push({ value: (monitor2.exttemp[i] == -40 ? 0 : monitor2.exttemp[i]) });
            pwm.push({ value: monitor2.bypasspwm[i] == 0 ? null : Math.trunc(monitor2.bypasspwm[i] / 255 * 100) });

        }
    }

    return <div>
        <h1>Modules</h1>
        <div className="region wide">
            <table id="modulesTable">
                <thead>
                    <tr>
                        <th id="mpBank">Bank</th>
                        <th id="mpModule">Module</th>
                        <th id="mpVoltage">Voltage</th>
                        <th id="mpvmin" className='hide'>V.Min</th>
                        <th id="mpvmax" className='hide'>V.Max</th>
                        <th id="mptint" className='hide'>Temp Int &deg; C</th>
                        <th id="mptext" className='hide'>Temp Ext &deg; C</th>
                        <th id="mpbypass" className='hide'>Bypass PWM %</th>
                        <th id="mpbpc" className='hide'>Bad packet count</th>
                        <th id="mppktr" className='hide'>Packets received</th>
                        <th id="mpbal" className='hide'>Balance energy used (mAh)</th>
                        <th></th>
                    </tr>
                </thead>
                <tbody className="rows" id="modulesRows">
                    {cells.map((index, value) => {
                        return <tr>
                            <td>
                                {bank[index]}
                            </td>
                            <td>
                                {value}
                            </td>
                            <td>
                                {voltages[index].value.toFixed(3)}
                            </td>
                            <td>
                                {voltagesmin.length > 0 ? voltagesmin[index].toFixed(3) : "n/a"}
                            </td>
                            <td>
                                {voltagesmax.length > 0 ? voltagesmax[index].toFixed(3) : "n/a"}
                            </td>
                            <td>
                                {tempint[index].value}
                            </td>
                            <td>
                                {tempext[index].value}
                            </td>
                            <td>
                                {pwm[index].value}
                            </td>
                            <td>
                                {monitor3.badpacket && monitor3.badpacket[index]}
                            </td>
                            <td>
                                {monitor3.pktrecvd && monitor3.pktrecvd[index]}
                            </td>
                            <td>
                                {monitor3.balcurrent && monitor3.balcurrent[index]}
                            </td>
                            <td>
                                <button style={{ cursor: "pointer" }} onClick={() => {
                                    props.dispatch(actionsBms.identifyModule(index));
                                }}>Identify</button>
                                <button style={{ cursor: "pointer" }} onClick={async () => {
                                    setDislayConfiguration({ display: false, cellid: index });
                                    await props.dispatch(actionsBms.getModule(index));
                                    setDislayConfiguration({ display: true, cellid: index });
                                }}>Configure</button>
                            </td>
                        </tr>
                    })}
                </tbody>
            </table>
            {displayConfiguration.display && <ModuleConfig cellid={displayConfiguration.cellid} />}
        </div>
        <GlobalSettings />
    </div>;
}

export default withStoreProvider(withNavigation(injectIntl(Modules)));