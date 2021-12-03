import React, { useState, useEffect } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import actionsBms from '../../actions/bms';

import RelayState from '../../components/relayState';
import RuleSettings from '../../components/ruleSettings';

function Rules(props) {

    const _rules = props.globalState.rules || {};

    const [defaultrelay, setDefaultRelay] = useState(_rules.relaydefault);
    const [relaytype, setRelaytype] = useState(_rules.relaytype);
    const [rules, setRules] = useState(_rules.rules);

    const rulesTab = [
        { label: "Emergency stop", hidden: true, },
        { label: "Internal BMS error", hidden: true },
        { label: "Current monitoring over current (Amps)", min: 0, max: 5000, step: 1 },
        { label: "Individual cell over voltage (mV)", min: 1800, max: 4500, step: 10 },
        { label: "Cell under voltage (mV)", min: 1800, max: 4500, step: 10 },
        { label: "Module over temperature (internal) °C", min: 0, max: 90, step: 1 },
        { label: "Module under temperature (internal) °C", min: 0, max: 90, step: 1 },
        { label: "Cell over temperature (external) °C", min: 0, max: 90, step: 1 },
        { label: "Cell under temperature (external) °C", min: 0, max: 90, step: 1 },
        { label: "Current monitor over voltage (mV)", min: 1000, step: 100 },
        { label: "Current monitor under voltage (mV)", min: 1000, step: 100 },
        { label: "Pack over voltage (mV)", min: 1000, step: 100 },
        { label: "Pack under voltage (mV)", min: 1000, step: 100 },
        { label: "Timer 2", min: 0, max: 1440, step: 1 },
        { label: "Timer 1", min: 0, max: 1440, step: 1 },
    ];

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
                            {rulesTab.map((_r, idx) => {
                                return <RuleSettings
                                    id={idx}
                                    label={_r.label}
                                    hidden={_r.hidden && _r.hidden ? true : false}
                                    min={_r.min}
                                    max={_r.max}
                                    step={_r.step}
                                    rule={rules[idx]}
                                    onChange={(rule) => {
                                        rules[idx] = rule;
                                    }}
                                />
                            })}
                            <tr>
                                <td><label for="defaultvalue">Relay default</label></td>
                                <td>&nbsp;</td>
                                <td>&nbsp;</td>
                                <td className="relayset">
                                    <select id="defaultrelay1" name="defaultrelay1" defaultValue={(() => {
                                        var relay_value = "X";
                                        if (defaultrelay[0] === true) { relay_value = "On"; }
                                        if (defaultrelay[0] === false) { relay_value = "Off"; }
                                        return relay_value;
                                    })()}
                                        onChange={(event) => {
                                            defaultrelay[0] = event.target.value == "On" ? true : false;
                                            setDefaultRelay(defaultrelay);
                                        }}
                                    >
                                        <option>On</option>
                                        <option>Off</option>
                                    </select>
                                    <select id="defaultrelay2" name="defaultrelay2" defaultValue={(() => {
                                        var relay_value = "X";
                                        if (defaultrelay[1] === true) { relay_value = "On"; }
                                        if (defaultrelay[1] === false) { relay_value = "Off"; }
                                        return relay_value;
                                    })()}
                                        onChange={(event) => {
                                            defaultrelay[1] = event.target.value == "On" ? true : false;
                                            setDefaultRelay(defaultrelay);
                                        }}
                                    >
                                        <option >On</option>
                                        <option >Off</option>
                                    </select>
                                    <select id="defaultrelay3" name="defaultrelay3" defaultValue={(() => {
                                        var relay_value = "X";
                                        if (defaultrelay[2] === true) { relay_value = "On"; }
                                        if (defaultrelay[2] === false) { relay_value = "Off"; }
                                        return relay_value;
                                    })()}
                                        onChange={(event) => {
                                            defaultrelay[2] = event.target.value == "On" ? true : false;
                                            setDefaultRelay(defaultrelay);
                                        }}>
                                        <option>On</option>
                                        <option>Off</option>
                                    </select>
                                    <select id="defaultrelay4" name="defaultrelay4" defaultValue={(() => {
                                        var relay_value = "X";
                                        if (defaultrelay[3] === true) { relay_value = "On"; }
                                        if (defaultrelay[3] === false) { relay_value = "Off"; }
                                        return relay_value;
                                    })()}
                                        onChange={(event) => {
                                            defaultrelay[3] = event.target.value == "On" ? true : false;
                                            setDefaultRelay(defaultrelay);
                                        }}>
                                        <option>On</option>
                                        <option>Off</option>
                                    </select></td>
                            </tr>
                            <tr>
                                <td><label for="relaytype">Relay type</label></td>
                                <td>&nbsp;</td>
                                <td>&nbsp;</td>
                                <td className="relayset">
                                    <select id="relaytype1" name="relaytype1" defaultValue={relaytype[0]}
                                        onChange={(event) => {
                                            relaytype[0] = event.target.value;
                                            setRelaytype(relaytype);
                                        }}>
                                        <option>Std</option>
                                        <option>Pulse</option>
                                    </select>
                                    <select id="relaytype2" name="relaytype2" defaultValue={relaytype[1]}
                                        onChange={(event) => {
                                            relaytype[1] = event.target.value;
                                            setRelaytype(relaytype);
                                        }}>
                                        <option>Std</option>
                                        <option>Pulse</option>
                                    </select>
                                    <select id="relaytype3" name="relaytype3" defaultValue={relaytype[2]}
                                        onChange={(event) => {
                                            relaytype[2] = event.target.value;
                                            setRelaytype(relaytype);
                                        }}>
                                        <option>Std</option>
                                        <option>Pulse</option>
                                    </select>
                                    <select id="relaytype4" name="relaytype4" defaultValue={relaytype[3]}
                                        onChange={(event) => {
                                            relaytype[3] = event.target.value;
                                            setRelaytype(relaytype);
                                        }}>
                                        <option>Std</option>
                                        <option>Pulse</option>
                                    </select>
                                </td>
                            </tr>
                        </tbody>
                    </table>
                </div>
                <button style={{ cursor: 'pointer' }} onClick={() => {
                    let obj = {};
                    rules.forEach((rule , idx) => {
                        obj["rule" + idx + "value"] = rule.value;
                        obj["rule" + idx + "hysteresis"] = rule.hysteresis;
                        rule.relays.forEach((relay , _idx) => {
                            var relay_value = "X";
                            if (relay === "On") { relay_value = "On"; }
                            if (relay === "Off") { relay_value = "Off"; }
                            obj["rule" + idx + "relay" + (_idx+1)] = relay_value;
                        });

                        relaytype.forEach((_relaiType , idx) => {
                            obj["relaytype" + (idx+1)] = _relaiType;
                        });

                        defaultrelay.forEach((_defaultrelay , idx) => {
                            obj["defaultrelay" + (idx+1)] = _defaultrelay;
                        });

                    });
                    props.dispatch(actionsBms.setRules(obj));
                }}>Save rules</button>
            </div>
        </div>
    </div>
}
export default withStoreProvider(withNavigation(injectIntl(Rules)));