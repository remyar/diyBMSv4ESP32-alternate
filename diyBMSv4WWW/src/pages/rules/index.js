import React, { useState, useEffect } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import actionsBms from '../../actions/bms';

import RelayState from '../../components/relayState';

function Rules(props) {

    const _rules = props.globalState.rules || {};

    const [defaultrelay , setDefaultRelay ] = useState(_rules.relaydefault);
    const [relaytype , setRelaytype ] = useState(_rules.relaytype);
    const [rules , setRules ] = useState(_rules.rules);

    return <div className="page" id="rulesPage">
        <h1>Rules</h1>
        <div className="region">
            <p id="rt1">DIYBMS supports relay modules to safely disconnect chargers, contactors or consumers. The rules
                allow you
                to configure the relays for your situation.</p>
            <p id="rt2">Rules are processed from lowest priorty to highest (bottom up). Control the relays using the
                options. A
                value of "X" means
                don't care/leave at value defined by lower priority rules.</p>
            <p id="rt3">Rules are triggered when the relevant value meets or exceeds the 'trigger' value. The rule will
                only
                disable
                when the value then passes the 'reset' value. This can help prevent relay clatter and rules firing
                on/off
                rapidly.</p>
            <p id="rt4">'Timer 1' and 'Timer 2' rules allow timed operation, this rule is active when the number of
                minutes past
                midnight has been reached, for instance setting 'Timer 1' trigger to 495 and reset to 555 would switch
                on at 8:15am and off at 9:15am. This only works if connected to internet for regular time updates.</p>
            <p id="rt5">Minutes since midnight now is: <span id='minutesnow'>{_rules.timenow}</span></p>
            <p id="rt6">Emergency stop is triggered by connector J1, once triggered controller needs to be reset to
                disable.</p>
        </div>
        <div className="region">
            <div id="form">
                <div className="settings">
                    <table>
                        <thead>
                            <tr>
                                <th id="rf1">Rule</th>
                                <th id="rf2">Trigger value</th>
                                <th id="rf3">Reset value</th>
                                <th id="rf4">Relay state</th>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td><label for="rule0value">Emergency stop</label></td>
                                <td><input type="hidden" name="rule0value" id="rule0value" defaultValue={rules[0].value} /></td>
                                <td><input type="hidden" name="rule0hysteresis" id="rule0hysteresis" defaultValue={rules[0].hysteresis} /></td>
                                <td className="relayset">   
                                    <RelayState ruleId={0} relayId={0} rules={rules}/>
                                    <RelayState ruleId={0} relayId={1} rules={rules}/>
                                    <RelayState ruleId={0} relayId={2} rules={rules}/>
                                    <RelayState ruleId={0} relayId={3} rules={rules}/>
                                </td>
                            </tr>
                            <tr>
                                <td><label for="rule1value">Internal BMS error</label></td>
                                <td><input type="hidden" name="rule1value" id="rule1value" defaultValue={rules[1].value} /></td>
                                <td><input type="hidden" name="rule1hysteresis" id="rule1hysteresis" defaultValue={rules[1].hysteresis} /></td>
                                <td className="relayset">
                                    <RelayState ruleId={1} relayId={0} rules={rules}/>
                                    <RelayState ruleId={1} relayId={1} rules={rules}/>
                                    <RelayState ruleId={1} relayId={2} rules={rules}/>
                                    <RelayState ruleId={1} relayId={3} rules={rules}/>
                                </td>
                            </tr>

                            <tr>
                                <td><label for="rule2value">Current monitoring over current (Amps)</label></td>
                                <td><input type="number" min="0" max="5000" step="1" name="rule2value" id="rule2value"
                                    defaultValue={rules[2].value} required="" /></td>
                                <td><input type="number" min="0" max="5000" step="1" name="rule2hysteresis"
                                    id="rule2hysteresis" defaultValue={rules[2].hysteresis} required="" /></td>
                                <td className="relayset">
                                    <RelayState ruleId={2} relayId={0} rules={rules}/>
                                    <RelayState ruleId={2} relayId={1} rules={rules}/>
                                    <RelayState ruleId={2} relayId={2} rules={rules}/>
                                    <RelayState ruleId={2} relayId={3} rules={rules}/>
                                </td>
                            </tr>

                            <tr>
                                <td><label for="rule3value">Individual cell over voltage (mV)</label></td>
                                <td><input type="number" min="1800" max="4500" step="10" name="rule3value"
                                    id="rule3value" defaultValue={rules[3].value} required="" /></td>
                                <td><input type="number" min="1800" max="4500" step="10" name="rule3hysteresis"
                                    id="rule3hysteresis" defaultValue={rules[3].hysteresis} required="" /></td>
                                <td className="relayset">
                                    <RelayState ruleId={3} relayId={0} rules={rules}/>
                                    <RelayState ruleId={3} relayId={1} rules={rules}/>
                                    <RelayState ruleId={3} relayId={2} rules={rules}/>
                                    <RelayState ruleId={3} relayId={3} rules={rules}/>
                                </td>
                            </tr>
                            <tr>
                                <td><label for="rule4value">Cell under voltage (mV)</label></td>
                                <td><input type="number" min="1800" max="4500" step="10" name="rule4value"
                                    id="rule4value" defaultValue={rules[4].value} required="" /></td>
                                <td><input type="number" min="1800" max="4500" step="10" name="rule4hysteresis"
                                    id="rule4hysteresis" defaultValue={rules[4].hysteresis} required="" /></td>
                                <td className="relayset">
                                    <RelayState ruleId={4} relayId={0} rules={rules}/>
                                    <RelayState ruleId={4} relayId={1} rules={rules}/>
                                    <RelayState ruleId={4} relayId={2} rules={rules}/>
                                    <RelayState ruleId={4} relayId={3} rules={rules}/>
                                </td>
                            </tr>
                            <tr>
                                <td><label for="rule5value">Module over temperature (internal) &deg;C</label></td>
                                <td><input type="number" min="0" max="90" step="1" name="rule5value" id="rule5value"
                                    defaultValue={rules[5].value} required="" /></td>
                                <td><input type="number" min="0" max="90" step="1" name="rule5hysteresis"
                                    id="rule5hysteresis" defaultValue={rules[5].hysteresis} required="" /></td>
                                <td className="relayset">
                                    <RelayState ruleId={5} relayId={0} rules={rules}/>
                                    <RelayState ruleId={5} relayId={1} rules={rules}/>
                                    <RelayState ruleId={5} relayId={2} rules={rules}/>
                                    <RelayState ruleId={5} relayId={3} rules={rules}/>
                                </td>
                            </tr>
                            <tr>
                                <td><label for="rule6value">Module under temperature (internal) &deg;C</label></td>
                                <td><input type="number" min="0" max="90" step="1" name="rule6value" id="rule6value"
                                    defaultValue={rules[6].value} required="" /></td>
                                <td><input type="number" min="0" max="90" step="1" name="rule6hysteresis"
                                    id="rule6hysteresis" defaultValue={rules[6].hysteresis} required="" /></td>
                                <td className="relayset">
                                    <RelayState ruleId={6} relayId={0} rules={rules}/>
                                    <RelayState ruleId={6} relayId={1} rules={rules}/>
                                    <RelayState ruleId={6} relayId={2} rules={rules}/>
                                    <RelayState ruleId={6} relayId={3} rules={rules}/>
                                </td>
                            </tr>
                            <tr>
                                <td><label for="rule7value">Cell over temperature (external) &deg;C</label></td>
                                <td><input type="number" min="0" max="90" step="1" name="rule7value" id="rule7value"
                                    defaultValue={rules[7].value} required="" /></td>
                                <td><input type="number" min="0" max="90" step="1" name="rule7hysteresis"
                                    id="rule7hysteresis" defaultValue={rules[7].hysteresis} required="" /></td>
                                <td className="relayset">
                                    <RelayState ruleId={7} relayId={0} rules={rules}/>
                                    <RelayState ruleId={7} relayId={1} rules={rules}/>
                                    <RelayState ruleId={7} relayId={2} rules={rules}/>
                                    <RelayState ruleId={7} relayId={3} rules={rules}/>
                                </td>
                            </tr>
                            <tr>
                                <td><label for="rule8value">Cell under temperature (external) &deg;C</label></td>
                                <td><input type="number" min="0" max="90" step="1" name="rule8value" id="rule8value"
                                    defaultValue={rules[8].value} required="" /></td>
                                <td><input type="number" min="0" max="90" step="1" name="rule8hysteresis"
                                    id="rule8hysteresis" defaultValue={rules[8].hysteresis}  required="" /></td>
                                <td className="relayset">
                                    <RelayState ruleId={8} relayId={0} rules={rules}/>
                                    <RelayState ruleId={8} relayId={1} rules={rules}/>
                                    <RelayState ruleId={8} relayId={2} rules={rules}/>
                                    <RelayState ruleId={8} relayId={3} rules={rules}/>
                                </td>
                            </tr>
                            <tr>
                                <td><label for="rule9value">Current monitor over voltage (mV)</label></td>
                                <td><input type="number" min="1000" max="99999999" step="100" name="rule9value"
                                    id="rule9value" defaultValue={rules[9].value}  required="" /></td>
                                <td><input type="number" min="1000" max="99999999" step="100" name="rule9hysteresis"
                                    id="rule9hysteresis" defaultValue={rules[9].hysteresis} required="" /></td>
                                <td className="relayset">
                                    <RelayState ruleId={9} relayId={0} rules={rules}/>
                                    <RelayState ruleId={9} relayId={1} rules={rules}/>
                                    <RelayState ruleId={9} relayId={2} rules={rules}/>
                                    <RelayState ruleId={9} relayId={3} rules={rules}/>
                                </td>
                            </tr>
                            <tr>
                                <td><label for="rule10value">Current monitor under voltage (mV)</label></td>
                                <td><input type="number" min="1000" max="99999999" step="100" name="rule10value"
                                    id="rule10value" defaultValue={rules[10].value} required="" /></td>
                                <td><input type="number" min="1000" max="99999999" step="100" name="rule10hysteresis"
                                    id="rule10hysteresis" defaultValue={rules[10].hysteresis} required="" /></td>
                                <td className="relayset">
                                    <RelayState ruleId={10} relayId={0} rules={rules}/>
                                    <RelayState ruleId={10} relayId={1} rules={rules}/>
                                    <RelayState ruleId={10} relayId={2} rules={rules}/>
                                    <RelayState ruleId={10} relayId={3} rules={rules}/>
                                </td>
                            </tr>

                            <tr>
                                <td><label for="rule11value">Pack over voltage (mV)</label></td>
                                <td><input type="number" min="1000" max="99999999" step="100" name="rule11value"
                                    id="rule11value" defaultValue={rules[11].value} required="" /></td>
                                <td><input type="number" min="1000" max="99999999" step="100" name="rule11hysteresis"
                                    id="rule11hysteresis" defaultValue={rules[11].hysteresis} required="" /></td>
                                <td className="relayset">
                                    <RelayState ruleId={11} relayId={0} rules={rules}/>
                                    <RelayState ruleId={11} relayId={1} rules={rules}/>
                                    <RelayState ruleId={11} relayId={2} rules={rules}/>
                                    <RelayState ruleId={11} relayId={3} rules={rules}/>
                                </td>
                            </tr>
                            <tr>
                                <td><label for="rule12value">Pack under voltage (mV)</label></td>
                                <td><input type="number" min="1000" max="99999999" step="100" name="rule12value"
                                    id="rule12value" defaultValue={rules[12].value} required="" /></td>
                                <td><input type="number" min="1000" max="99999999" step="100" name="rule12hysteresis"
                                    id="rule12hysteresis" defaultValue={rules[12].hysteresis} required="" /></td>
                                <td className="relayset">
                                    <RelayState ruleId={12} relayId={0} rules={rules}/>
                                    <RelayState ruleId={12} relayId={1} rules={rules}/>
                                    <RelayState ruleId={12} relayId={2} rules={rules}/>
                                    <RelayState ruleId={12} relayId={3} rules={rules}/>
                                </td>
                            </tr>
                            <tr>
                                <td><label for="rule13value">Timer 2</label></td>
                                <td><input type="number" min="0" max="1440" step="1" name="rule13value" id="rule13value"
                                    defaultValue={rules[13].value} required="" /></td>
                                <td><input type="number" min="0" max="1440" step="1" name="rule13hysteresis"
                                    id="rule13hysteresis" defaultValue={rules[13].hysteresis}  required="" /></td>
                                <td className="relayset">
                                    <RelayState ruleId={13} relayId={0} rules={rules}/>
                                    <RelayState ruleId={13} relayId={1} rules={rules}/>
                                    <RelayState ruleId={13} relayId={2} rules={rules}/>
                                    <RelayState ruleId={13} relayId={3} rules={rules}/>
                                </td>
                            </tr>
                            <tr>
                                <td><label for="rule14value">Timer 1</label></td>
                                <td><input type="number" min="0" max="1440" step="1" name="rule14value" id="rule14value"
                                    defaultValue={rules[14].value} required="" /></td>
                                <td><input type="number" min="0" max="1440" step="1" name="rule14hysteresis"
                                    id="rule14hysteresis" defaultValue={rules[14].hysteresis} required="" /></td>
                                <td className="relayset">
                                    <RelayState ruleId={14} relayId={0} rules={rules}/>
                                    <RelayState ruleId={14} relayId={1} rules={rules}/>
                                    <RelayState ruleId={14} relayId={2} rules={rules}/>
                                    <RelayState ruleId={14} relayId={3} rules={rules}/>
                                </td>
                            </tr>
                            <tr>
                                <td><label for="defaultvalue">Relay default</label></td>
                                <td>&nbsp;</td>
                                <td>&nbsp;</td>
                                <td className="relayset">
                                    <select id="defaultrelay1" name="defaultrelay1" defaultValue={(()=>{
                                        var relay_value = "X";
                                        if (defaultrelay[0] === true) { relay_value = "On"; }
                                        if (defaultrelay[0] === false) { relay_value = "Off"; }
                                        return relay_value;
                                    })()}>
                                        <option>On</option>
                                        <option>Off</option>
                                    </select>
                                    <select id="defaultrelay2" name="defaultrelay2" defaultValue={(()=>{
                                        var relay_value = "X";
                                        if (defaultrelay[1] === true) { relay_value = "On"; }
                                        if (defaultrelay[1] === false) { relay_value = "Off"; }
                                        return relay_value;
                                    })()}>
                                        <option>On</option>
                                        <option>Off</option>
                                    </select>
                                    <select id="defaultrelay3" name="defaultrelay3" defaultValue={(()=>{
                                        var relay_value = "X";
                                        if (defaultrelay[2] === true) { relay_value = "On"; }
                                        if (defaultrelay[2] === false) { relay_value = "Off"; }
                                        return relay_value;
                                    })()}>
                                        <option>On</option>
                                        <option>Off</option>
                                    </select>
                                    <select id="defaultrelay4" name="defaultrelay4" defaultValue={(()=>{
                                        var relay_value = "X";
                                        if (defaultrelay[3] === true) { relay_value = "On"; }
                                        if (defaultrelay[3] === false) { relay_value = "Off"; }
                                        return relay_value;
                                    })()}>
                                        <option>On</option>
                                        <option>Off</option>
                                    </select></td>
                            </tr>
                            <tr>
                                <td><label for="relaytype">Relay type</label></td>
                                <td>&nbsp;</td>
                                <td>&nbsp;</td>
                                <td className="relayset">
                                    <select id="relaytype1" name="relaytype1" defaultValue={relaytype[0]}>
                                        <option>Std</option>
                                        <option>Pulse</option>
                                    </select>
                                    <select id="relaytype2" name="relaytype2" defaultValue={relaytype[1]}>
                                        <option>Std</option>
                                        <option>Pulse</option>
                                    </select>
                                    <select id="relaytype3" name="relaytype3" defaultValue={relaytype[2]}>
                                        <option>Std</option>
                                        <option>Pulse</option>
                                    </select>
                                    <select id="relaytype4" name="relaytype4" defaultValue={relaytype[3]}>
                                        <option>Std</option>
                                        <option>Pulse</option>
                                    </select>
                                </td>
                            </tr>
                        </tbody>
                    </table>
                </div>
                <button style={{cursor : 'pointer'}} onClick={()=>{

                }}>Save rules</button>
            </div>
        </div>
    </div>
}
export default withStoreProvider(withNavigation(injectIntl(Rules)));