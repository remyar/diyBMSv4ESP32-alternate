import React, { useState, useEffect } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';
import { withSnackBar } from '../../providers/snackBar';

import actionsBms from '../../actions/bms';

import RelayState from '../../components/relayState';
import RuleSettings from '../../components/ruleSettings';

function Rules(props) {

    const _settings = props.globalState.settings || {};
    const rules = props.globalState.rules || {};

    /*
    const [defaultrelay, setDefaultRelay] = useState(_rules.relaydefault);
    const [relaytype, setRelaytype] = useState(_rules.relaytype);*/

    const [_rules, setRules] = useState(rules);

    const rulesTab = [
        //{ label: "Emergency stop", hidden: true, },
        //{ label: "Internal BMS error", hidden: true },
        //{ label: "Current monitoring over current (Amps)", min: 0, max: 5000, step: 1 },
        { label: "Individual cell over voltage (mV)", min: 1800, max: 4500, step: 10 },
        { label: "Cell under voltage (mV)", min: 1800, max: 4500, step: 10 },
        { label: "Module over temperature (internal) °C", min: 0, max: 90, step: 1 },
        { label: "Module under temperature (internal) °C", min: 0, max: 90, step: 1 },
        { label: "Cell over temperature (external) °C", min: 0, max: 90, step: 1 },
        { label: "Cell under temperature (external) °C", min: 0, max: 90, step: 1 },
        //{ label: "Current monitor over voltage (mV)", min: 1000, step: 100 },
        //{ label: "Current monitor under voltage (mV)", min: 1000, step: 100 },
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
            <p id="rt5">Minutes since midnight now is: <span id='minutesnow'>{rules.timenow}</span></p>
            <p id="rt6">Emergency stop is triggered by connector J1, once triggered controller needs to be reset to
                disable.</p>
        </div>
        <div className="region">
            <div id="form">
                {(() => {
                    let options = [];
                    for (let n = 0; n < _settings.totalControllers; n++) {
                        options.push(<div className="settings">
                            <h2 id="mb1">Controller N° {n + 1}</h2>
                            <table style={{ paddingTop: '5px' }}>
                                <thead>
                                    <tr>
                                        <th id="rf1">Rule</th>
                                        <th id="rf2">Trigger value</th>
                                        <th id="rf3">Reset value</th>
                                        <th id="rf4">Relay state</th>
                                    </tr>
                                </thead>
                                <tbody >
                                    {rulesTab.map((_r, idx) => {
                                        return <RuleSettings
                                            id={idx}
                                            label={_r.label}
                                            hidden={_r.hidden && _r.hidden ? true : false}
                                            min={_r.min}
                                            max={_r.max}
                                            step={_r.step}
                                            rule={_rules["rules_" + n][idx]}
                                            triggered={rules["rules_" + n][idx].triggered}
                                            onChange={(rule) => {
                                                _rules["rules_" + n][idx] = rule;
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
                                                if (_rules["relaydefault_"+n][0] === 0xFF || _rules["relaydefault_"+n][0] === true || _rules["relaydefault_"+n][0] === 'On') { relay_value = "On"; }
                                                if (_rules["relaydefault_"+n][0] === 0x99 || _rules["relaydefault_"+n][0] === false || _rules["relaydefault_"+n][0] === 'Off') { relay_value = "Off"; }
                                                return relay_value;
                                            })()}
                                                onChange={(event) => {
                                                    _rules["relaydefault_"+n][0] = event.target.value == "On" ? true : false;
                                                    //setDefaultRelay(defaultrelay);
                                                }}
                                            >
                                                <option>On</option>
                                                <option>Off</option>
                                            </select>
                                            <select id="defaultrelay2" name="defaultrelay2" defaultValue={(() => {
                                                var relay_value = "X";
                                                if (_rules["relaydefault_"+n][1] === 0xFF || _rules["relaydefault_"+n][1] === true || _rules["relaydefault_"+n][1] === 'On') { relay_value = "On"; }
                                                if (_rules["relaydefault_"+n][1] === 0x99 || _rules["relaydefault_"+n][1] === false || _rules["relaydefault_"+n][1] === 'Off') { relay_value = "Off"; }
                                                return relay_value;
                                            })()}
                                                onChange={(event) => {
                                                    _rules["relaydefault_"+n][1] = event.target.value == "On" ? true : false;
                                                   // setDefaultRelay(defaultrelay);
                                                }}
                                            >
                                                <option >On</option>
                                                <option >Off</option>
                                            </select>
                                            <select id="defaultrelay3" name="defaultrelay3" defaultValue={(() => {
                                                var relay_value = "X";
                                                if (_rules["relaydefault_"+n][2] === 0xFF || _rules["relaydefault_"+n][2] === true || _rules["relaydefault_"+n][2] === 'On') { relay_value = "On"; }
                                                if (_rules["relaydefault_"+n][2] === 0x99 || _rules["relaydefault_"+n][2] === false|| _rules["relaydefault_"+n][2] === 'Off') { relay_value = "Off"; }
                                                return relay_value;
                                            })()}
                                                onChange={(event) => {
                                                    _rules["relaydefault_"+n][2] = event.target.value == "On" ? true : false;
                                                 //   setDefaultRelay(defaultrelay);
                                                }}>
                                                <option>On</option>
                                                <option>Off</option>
                                            </select>
                                           {/* <select id="defaultrelay4" name="defaultrelay4" defaultValue={(() => {
                                                var relay_value = "X";
                                                if (_rules["relaydefault_"+n][3] === true|| _rules["relaydefault_"+n][3] === 'On') { relay_value = "On"; }
                                                if (_rules["relaydefault_"+n][3] === false|| _rules["relaydefault_"+n][3] === 'Off') { relay_value = "Off"; }
                                                return relay_value;
                                            })()}
                                                onChange={(event) => {
                                                    _rules["relaydefault_"+n][3] = event.target.value == "On" ? true : false;
                                                    //setDefaultRelay(defaultrelay);
                                                }}>
                                                <option>On</option>
                                                <option>Off</option>
                                            </select>*/}
                                        </td>
                                            </tr>
                                </tbody>
                            </table>
                        </div>);
                    }
                    return options;
                })()}


                <button style={{ cursor: 'pointer' }} onClick={async () => {
                    let obj = {};
                    for (let n = 0; n < _settings.totalControllers; n++) {
                        let rules = _rules["rules_" + n];
                        rules.forEach((rule, idx) => {
                            obj["rule_" + n + "_" + idx + "_value"] = rule.value;
                            obj["rule_" + n + "_" + idx + "_hysteresis"] = rule.hysteresis;
                            rule.relays.forEach((relay, _idx) => {
                                let relay_value = "X";
                                if ((relay === 0xFF) || (relay === "On") || (relay === true)) { relay_value = "On"; }
                                if ((relay === 0x99) || (relay === "Off") || (relay === false)) { relay_value = "Off"; }
                                obj["rule_" + n + "_" + idx + "_relay_" + (_idx + 1)] = relay_value;
                            });
                        });

                        let defaultRelay = _rules["relaydefault_" + n];
                        defaultRelay.forEach((__relay , idx) => {
                            obj["relaydefault_" + n + "_" + idx] = __relay === true ||  __relay === 'On' ? "On" : "Off";
                        });
                    }
                    let response = await props.dispatch(actionsBms.setRules(obj));
                    if (response === true) {
                        props.snackbar.success("Saved");
                    } else {
                        props.snackbar.error("An Error was occured");
                    }
                }}>Save rules</button>
            </div>
        </div>
    </div>
}
export default withStoreProvider(withNavigation(withSnackBar(injectIntl(Rules))));